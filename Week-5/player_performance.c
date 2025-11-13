#include "Players_data.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NAME_LENGTH 51
#define PLAYERS_PER_TEAM 50

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
    // helper fields
    float strikeRateSum;
    int totalStrikers;
    PlayerNode *headPointerToPlayers;
    PlayerNode *tailPointerToPlayers;
} TeamDetails;

typedef struct TeamNode
{
    TeamDetails teamData;
    struct TeamNode *next;
    struct TeamNode *previous;
} TeamNode;

static TeamNode *teamListHead = NULL;
static TeamNode *teamListTail = NULL;

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
    printf("\n%d %d", teamListHead->teamData.teamId, teamListTail->teamData.teamId);
    displayMenu();
    return 0;
}

// initializing teams list
void initializeTeams()
{
    for (int currentInx = 0; currentInx < teamCount; currentInx++)
    {
        TeamNode *newTeam = (TeamNode *)malloc(sizeof(TeamNode));
        if (newTeam == NULL)
        {
            printf("\nMemory allocation failed.");
            exit(1);
        }

        newTeam->next = NULL;
        newTeam->teamData.teamId = currentInx + 1;
        newTeam->teamData.teamName = teams[currentInx];
        newTeam->teamData.headPointerToPlayers = NULL;
        newTeam->teamData.tailPointerToPlayers = NULL;

        int count = 0, strikerCount = 0;
        float strikeTotal = 0;
        for (int currentPlayer = 0; currentPlayer < playerCount; currentPlayer++)
        {
            // member of the team
            if (strcmp(teams[currentInx], players[currentPlayer].team) == 0)
            {
                count++;
                if (strcmp(players[currentPlayer].role, "All-rounder") == 0 ||
                    strcmp(players[currentPlayer].role, "Batsman") == 0)
                {
                    strikerCount++;
                    strikeTotal += players[currentPlayer].strikeRate;
                }

                PlayerNode *newPlayer = initializePlayer(players[currentPlayer]);
                if (newTeam->teamData.headPointerToPlayers == NULL)
                {
                    newTeam->teamData.headPointerToPlayers = newPlayer;
                    newTeam->teamData.tailPointerToPlayers = newPlayer;
                }
                else
                {
                    newPlayer->previous = newTeam->teamData.tailPointerToPlayers;
                    newTeam->teamData.tailPointerToPlayers->next = newPlayer;
                    newTeam->teamData.tailPointerToPlayers = newPlayer;
                }
            }
        }
        newTeam->teamData.totalPlayers = count;
        newTeam->teamData.strikeRateSum = strikeTotal;
        newTeam->teamData.totalStrikers = strikerCount;
        newTeam->teamData.averageBattingStrikeRate = strikeTotal / strikerCount;

        if (teamListHead == NULL)
        {
            newTeam->previous = NULL;
            teamListHead = newTeam;
            teamListTail = newTeam;
        }
        else
        {
            newTeam->previous = teamListTail;
            teamListTail->next = newTeam;
            teamListTail = newTeam;
        }
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
        printf("\n===================================");
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
            displayTeamsByStrikeRate();
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

TeamNode *findMid(TeamNode *start, TeamNode *end)
{
    if (start == end)
        return start;
    TeamNode *first = start;
    TeamNode *second = end;
    while (first != second && first->next != second)
    {
        first = first->next;
        second = second->previous;
    }
    return first;
}

TeamNode *findTeam(TeamNode *head, TeamNode *tail, int id)
{
    while (head != tail->next)
    {
        TeamNode *mid = findMid(head, tail);
        if (mid->teamData.teamId == id)
        {
            return mid;
        }
        else if (mid->teamData.teamId > id)
        {
            tail = mid->previous;
        }
        else
        {
            head = mid->next;
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
    foundTeam = findTeam(teamListHead, teamListTail, id);
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
    newPlayer->playerData.playerName = name;

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
        foundTeam->teamData.strikeRateSum += newPlayer->playerData.strikeRate;
        foundTeam->teamData.totalStrikers++;
        foundTeam->teamData.averageBattingStrikeRate = foundTeam->teamData.strikeRateSum / foundTeam->teamData.totalStrikers;
    }
    else if (option == 2)
    {
        newPlayer->playerData.role = "Bowler";
        newPlayer->playerData.performanceIndex = (newPlayer->playerData.wickets * 2) + (100 - newPlayer->playerData.economyRate);
    }
    else if (option == 3)
    {
        newPlayer->playerData.role = "All-rounder";
        newPlayer->playerData.performanceIndex = ((newPlayer->playerData.battingAverage * newPlayer->playerData.strikeRate) / 100) + (newPlayer->playerData.wickets * 2);
        foundTeam->teamData.strikeRateSum += newPlayer->playerData.strikeRate;
        foundTeam->teamData.totalStrikers++;
        foundTeam->teamData.averageBattingStrikeRate = foundTeam->teamData.strikeRateSum / foundTeam->teamData.totalStrikers;
    }
    else
    {
        printf("\nInvalid option");
        return;
    }

    newPlayer->previous = foundTeam->teamData.tailPointerToPlayers;
    foundTeam->teamData.tailPointerToPlayers->next = newPlayer;
    foundTeam->teamData.tailPointerToPlayers = newPlayer;

    printf("\nPlayer added successfully to Team %s!", foundTeam->teamData.teamName);
}

void displayPlayersOfTeam() {}

void displayTeamsByStrikeRate() {}

void displayTopNPlayers() {}

void displayPlayersByRole() {}