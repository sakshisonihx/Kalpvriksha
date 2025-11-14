#include "Players_data.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NAME_LENGTH 51
#define PLAYERS_PER_TEAM 50
#define MAX_TEAMS 10

typedef struct
{
    int playerId;
    const char *playerName;
    const char *teamName;
    const char *role;
    int totalRuns;
    float battingAverage;
    float strikeRate;
    int wickets;
    float economyRate;
    float performanceIndex;
} PlayerDetails;

typedef struct PlayerNode
{
    PlayerDetails playerData;
    struct PlayerNode *next;
    struct PlayerNode *previous;
} PlayerNode;

typedef struct
{
    int teamId;
    const char *teamName;
    int totalPlayers;
    float averageBattingStrikeRate;
} TeamDetails;

typedef struct TeamNode
{
    TeamDetails teamData;
    // helper fields to update avg strike rate in O(1) when we add a new player
    float strikeRateSum;
    int totalStrikers;
    PlayerNode *allBatsmen;
    PlayerNode *allBowlers;
    PlayerNode *allAllRounders;
} TeamNode;

static TeamNode teamsList[MAX_TEAMS] = {0};

PlayerNode *initializePlayer(Player);
void initializeTeams();
void displayMenu();
void addPlayerToTeam();
void displayPlayersOfTeam();
void displayTeamsByStrikeRate();
void displayTopNPlayers();
void displayPlayersByRole();

int main()
{
    initializeTeams();
    printf("\n%d %d", teamsList[0].teamData.teamId, teamsList[MAX_TEAMS - 1].teamData.teamId);
    displayMenu();
    return 0;
}

void insertInRoleList(PlayerNode *player, PlayerNode **listHead)
{
    if (*listHead == NULL)
    {
        *listHead = player;
        return;
    }
    else if ((*listHead)->playerData.performanceIndex < player->playerData.performanceIndex)
    {
        player->next = *listHead;
        player->previous = NULL;
        (*listHead)->previous = player;
        *listHead = player;
        return;
    }
    PlayerNode *temp = *listHead;
    while (temp->next != NULL && temp->next->playerData.performanceIndex > player->playerData.performanceIndex)
    {
        temp = temp->next;
    }
    player->next = temp->next;
    player->previous = temp;
    if (temp->next != NULL)
    {
        temp->next->previous = player;
    }
    temp->next = player;
}

// initializing teams list
void initializeTeams()
{
    for (int currentInx = 0; currentInx < MAX_TEAMS; currentInx++)
    {
        teamsList[currentInx].teamData.teamId = currentInx + 1;
        teamsList[currentInx].teamData.teamName = teams[currentInx];
        teamsList[currentInx].allAllRounders = NULL;
        teamsList[currentInx].allBatsmen = NULL;
        teamsList[currentInx].allBowlers = NULL;

        int count = 0, strikerCount = 0;
        float strikeTotal = 0;
        for (int currentPlayer = 0; currentPlayer < playerCount; currentPlayer++)
        {
            // member of the team
            if (strcmp(teams[currentInx], players[currentPlayer].team) == 0)
            {
                count++;

                PlayerNode *newPlayer = initializePlayer(players[currentPlayer]);

                if (strcmp(players[currentPlayer].role, "All-rounder") == 0 ||
                    strcmp(players[currentPlayer].role, "Batsman") == 0)
                {
                    strikerCount++;
                    strikeTotal += players[currentPlayer].strikeRate;
                    if (strcmp(players[currentPlayer].role, "All-rounder") == 0)
                    {
                        insertInRoleList(newPlayer, &teamsList[currentInx].allAllRounders);
                    }
                    else
                    {
                        insertInRoleList(newPlayer, &teamsList[currentInx].allBatsmen);
                    }
                }
                else
                {
                    insertInRoleList(newPlayer, &teamsList[currentInx].allBowlers);
                }
            }
        }
        teamsList[currentInx].teamData.totalPlayers = count;
        teamsList[currentInx].strikeRateSum = strikeTotal;
        teamsList[currentInx].totalStrikers = strikerCount;
        teamsList[currentInx].teamData.averageBattingStrikeRate = strikeTotal / strikerCount;
    }
}

// initializing players
PlayerNode *initializePlayer(Player currentPlayer)
{
    PlayerNode *newPlayer = (PlayerNode *)malloc(sizeof(PlayerNode));
    if (newPlayer == NULL)
    {
        printf("\nMemory allocation failed.");
        exit(1);
    }

    newPlayer->next = NULL;
    newPlayer->previous = NULL;
    newPlayer->playerData.playerId = currentPlayer.id;
    newPlayer->playerData.playerName = currentPlayer.name;
    newPlayer->playerData.teamName = currentPlayer.team;
    newPlayer->playerData.role = currentPlayer.role;
    newPlayer->playerData.totalRuns = currentPlayer.totalRuns;
    newPlayer->playerData.battingAverage = currentPlayer.battingAverage;
    newPlayer->playerData.strikeRate = currentPlayer.strikeRate;
    newPlayer->playerData.wickets = currentPlayer.wickets;
    newPlayer->playerData.economyRate = currentPlayer.economyRate;
    if (strcmp(currentPlayer.role, "Batsman") == 0)
    {
        newPlayer->playerData.performanceIndex = (currentPlayer.battingAverage * currentPlayer.strikeRate) / 100;
    }
    else if (strcmp(currentPlayer.role, "Bowler") == 0)
    {
        newPlayer->playerData.performanceIndex = (currentPlayer.wickets * 2) + (100 - currentPlayer.economyRate);
    }
    else if (strcmp(currentPlayer.role, "All-rounder") == 0)
    {
        newPlayer->playerData.performanceIndex = ((currentPlayer.battingAverage * currentPlayer.strikeRate) / 100) + (currentPlayer.wickets * 2);
    }
    return newPlayer;
}

void displayMenu()
{
    int choice;
    do
    {
        printf("\n\n===================================");
        printf("\nICC ODI Player Performance Analyzer");
        printf("\n===================================");
        printf("\n1. Add player to team.");
        printf("\n2. Display players of a team by teamId.");
        printf("\n3. Display teams by average batting strike rate.");
        printf("\n4. Display top n players of a specific team by role.");
        printf("\n5. Display all players of specific role across all teams by performance index.");
        printf("\n6. Exit the program.");
        printf("\nEnter choice: ");
        scanf("%d", &choice);
        switch (choice)
        {
        case 1:
            addPlayerToTeam();
            break;
        case 2:
            displayPlayersOfTeam();
            break;
        case 3:
            displayTeamsByStrikeRate(teamsList);
            break;
        case 4:
            displayTopNPlayers();
            break;
        case 5:
            displayPlayersByRole();
            break;
        case 6:
            exit(0);
        default:
            printf("\nInvalid choice");
            break;
        }
    } while (choice != 6);
}

TeamNode *findTeam(int start, int end, int id)
{
    while (start <= end)
    {
        int mid = start + (end - start) / 2;
        if (teamsList[mid].teamData.teamId == id)
        {
            return &teamsList[mid];
        }
        else if (teamsList[mid].teamData.teamId > id)
        {
            end = mid - 1;
        }
        else
        {
            start = mid + 1;
        }
    }
    return NULL;
}

void addPlayerToTeam()
{
    PlayerNode *newPlayer = (PlayerNode *)malloc(sizeof(PlayerNode));
    if (newPlayer == NULL)
    {
        printf("\nMemory allocation failed");
    }

    newPlayer->next = NULL;
    int id;
    printf("\nEnter Team ID to add player: ");
    scanf("%d", &id);
    TeamNode *foundTeam;
    foundTeam = findTeam(0, MAX_TEAMS - 1, id);
    if (foundTeam == NULL)
    {
        printf("\nTeam with id %d not found.", id);
        return;
    }
    foundTeam->teamData.totalPlayers++;
    newPlayer->playerData.teamName = foundTeam->teamData.teamName;

    printf("\nEnter Player Details:\n");
    printf("\nPlayer ID: ");
    scanf("%d", &newPlayer->playerData.playerId);

    int clearInput;
    while ((clearInput = getchar()) != '\n' && clearInput != EOF)
        ;
    printf("\nName: ");
    char name[NAME_LENGTH];
    fgets(name, NAME_LENGTH, stdin);
    name[strcspn(name, "\n")] = '\0';
    newPlayer->playerData.playerName = strdup(name);

    printf("\nTotal Runs: ");
    scanf("%d", &newPlayer->playerData.totalRuns);

    printf("\nBatting Average: ");
    scanf("%f", &newPlayer->playerData.battingAverage);

    printf("\nStrike Rate: ");
    scanf("%f", &newPlayer->playerData.strikeRate);

    printf("\nWickets: ");
    scanf("%d", &newPlayer->playerData.wickets);

    printf("\nEconomy Rate: ");
    scanf("%f", &newPlayer->playerData.economyRate);

    printf("\nRole (1-Batsman, 2-Bowler, 3-All-rounder): ");
    int option;
    scanf("%d", &option);
    if (option == 1)
    {
        newPlayer->playerData.role = "Batsman";
        newPlayer->playerData.performanceIndex = (newPlayer->playerData.battingAverage * newPlayer->playerData.strikeRate) / 100;
        foundTeam->strikeRateSum += newPlayer->playerData.strikeRate;
        foundTeam->totalStrikers++;
        foundTeam->teamData.averageBattingStrikeRate = foundTeam->strikeRateSum / foundTeam->totalStrikers;
        insertInRoleList(newPlayer, &foundTeam->allBatsmen);
    }
    else if (option == 2)
    {
        newPlayer->playerData.role = "Bowler";
        newPlayer->playerData.performanceIndex = (newPlayer->playerData.wickets * 2) + (100 - newPlayer->playerData.economyRate);
        insertInRoleList(newPlayer, &foundTeam->allBowlers);
    }
    else if (option == 3)
    {
        newPlayer->playerData.role = "All-rounder";
        newPlayer->playerData.performanceIndex = ((newPlayer->playerData.battingAverage * newPlayer->playerData.strikeRate) / 100) + (newPlayer->playerData.wickets * 2);
        foundTeam->strikeRateSum += newPlayer->playerData.strikeRate;
        foundTeam->totalStrikers++;
        foundTeam->teamData.averageBattingStrikeRate = foundTeam->strikeRateSum / foundTeam->totalStrikers;
        insertInRoleList(newPlayer, &foundTeam->allAllRounders);
    }
    else
    {
        printf("\nInvalid option");
        return;
    }

    printf("\nPlayer added successfully to Team %s!", foundTeam->teamData.teamName);
}

void printPlayers(TeamNode *currentTeam, char *role, int playerCount)
{
    PlayerNode *temp = NULL;
    int count = 0;

    if (strcmp(role, "Batsman") == 0)
    {
        temp = currentTeam->allBatsmen;
    }
    else if (strcmp(role, "Bowler") == 0)
    {
        temp = currentTeam->allBowlers;
    }
    else if (strcmp(role, "All-rounder") == 0)
    {
        temp = currentTeam->allAllRounders;
    }

    while (temp != NULL && count < playerCount)
    {
        printf("\n%-4d %-25s %-15s %-10d %-15.2f %-15.2f %-10d %-15.2f %-15.2f", temp->playerData.playerId, temp->playerData.playerName, temp->playerData.role, temp->playerData.totalRuns, temp->playerData.battingAverage, temp->playerData.strikeRate, temp->playerData.wickets, temp->playerData.economyRate, temp->playerData.performanceIndex);
        temp = temp->next;
        count++;
    }
}

void displayPlayersOfTeam()
{
    int id;
    printf("\nEnter Team ID: ");
    scanf("%d", &id);
    TeamNode *foundTeam;
    foundTeam = findTeam(0, MAX_TEAMS - 1, id);
    if (foundTeam == NULL)
    {
        printf("\nTeam with id %d not found.", id);
        return;
    }

    printf("\nPlayers of team %s: \n", foundTeam->teamData.teamName);
    printf("\n");
    for (int i = 0; i < 130; i++)
    {
        printf("%c", '=');
    }
    printf("\n%-4s %-25s %-15s %-10s %-15s %-15s %-10s %-15s %-15s", "ID", "Name", "Role", "Runs", "Batting Avg", "Strike Rate", "Wickets", "Economy Rate", "Perf. Index");
    printf("\n");
    for (int i = 0; i < 130; i++)
    {
        printf("%c", '=');
    }

    printPlayers(foundTeam, "Batsman", foundTeam->teamData.totalPlayers);
    printPlayers(foundTeam, "Bowler", foundTeam->teamData.totalPlayers);
    printPlayers(foundTeam, "All-rounder", foundTeam->teamData.totalPlayers);

    printf("\n");
    for (int i = 0; i < 130; i++)
    {
        printf("%c", '=');
    }
    printf("\n\nTotal Players: %d", foundTeam->teamData.totalPlayers);
    printf("\nAverage Batting Strike Rate: %.2f", foundTeam->teamData.averageBattingStrikeRate);
    printf("\n");
}

void displayTeamsByStrikeRate(TeamNode team[])
{
    TeamNode temp[MAX_TEAMS];
    for (int currInx = 0; currInx < MAX_TEAMS; currInx++)
    {
        temp[currInx] = team[currInx];
    }
    for (int i = 1; i < MAX_TEAMS; i++)
    {
        TeamNode currentTeam = temp[i];
        int j = i - 1;
        while (j >= 0 && temp[j].teamData.averageBattingStrikeRate < currentTeam.teamData.averageBattingStrikeRate)
        {
            temp[j + 1] = temp[j];
            j--;
        }
        temp[j + 1] = currentTeam;
    }

    printf("\nTeams sorted by average batting strike rate:\n");
    printf("\n");
    for (int i = 0; i < 80; i++)
    {
        printf("%c", '=');
    }
    printf("\n%-5s %-15s %-15s %-15s", "ID", "Team Name", "Avg Bat SR", "Total Players");
    printf("\n");
    for (int i = 0; i < 80; i++)
    {
        printf("%c", '=');
    }
    for (int current = 0; current < MAX_TEAMS; current++)
    {
        printf("\n%-5d %-15s %-15.2f %-15d", temp[current].teamData.teamId, temp[current].teamData.teamName, temp[current].teamData.averageBattingStrikeRate, temp[current].teamData.totalPlayers);
    }
    printf("\n");
    for (int i = 0; i < 80; i++)
    {
        printf("%c", '=');
    }
}

void displayTopNPlayers()
{
    int id, noOfPlayers, choice;
    printf("\nEnter team ID: ");
    scanf("%d", &id);
    TeamNode *foundTeam = findTeam(0, MAX_TEAMS - 1, id);
    if (foundTeam == NULL)
    {
        printf("\nTeam with id %d not found.", id);
        return;
    }

    printf("\nEnter role(1-Batsman, 2-Bowler, 3-All-rounder): ");
    scanf("%d", &choice);
    printf("\nEnter number of players: ");
    scanf("%d", &noOfPlayers);

    printf("\n");
    for (int i = 0; i < 130; i++)
    {
        printf("%c", '=');
    }
    printf("\n%-4s %-25s %-15s %-10s %-15s %-15s %-10s %-15s %-15s", "ID", "Name", "Role", "Runs", "Batting Avg", "Strike Rate", "Wickets", "Economy Rate", "Perf. Index");
    printf("\n");
    for (int i = 0; i < 130; i++)
    {
        printf("%c", '=');
    }

    if (choice == 1)
    {
        printPlayers(foundTeam, "Batsman", noOfPlayers);
    }
    else if (choice == 2)
    {
        printPlayers(foundTeam, "Bowler", noOfPlayers);
    }
    else if (choice == 3)
    {
        printPlayers(foundTeam, "All-rounder", noOfPlayers);
    }
    else
    {
        printf("\nInvalid choice");
        return;
    }
    printf("\n");
    for (int i = 0; i < 130; i++)
    {
        printf("%c", '=');
    }
}

void displayPlayersByRole() {}