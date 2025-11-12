#include "Players_data.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NAME_LENGTH 51

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

typedef struct
{
    int teamId;
    const char *teamName;
    int totalPlayers;
    float averageBattingStrikeRate;
} TeamDetails;

typedef struct PlayerNode
{
    PlayerDetails playerData;
    struct PlayerNode *next;
    struct PlayerNode *previous;
} PlayerNode;

typedef struct TeamNode
{
    TeamDetails teamData;
    struct TeamNode *next;
    struct TeamNode *previous;
} TeamNode;

static PlayerNode *playerListHead = NULL;
static PlayerNode *playerListTail = NULL;
static TeamNode *teamListHead = NULL;
static TeamNode *teamListTail = NULL;

void initializePlayers();
void initializeTeams();
void displayMenu();
void addPlayerToTeam();
void displayPlayersOfTeam();
void displayTeamsByStrikeRate();
void displayTopNPlayers();
void displayPlayersByRole();

int main()
{
    initializePlayers();
    initializeTeams();
    printf("\n%d %d", teamListHead->teamData.teamId, teamListTail->teamData.teamId);
    printf("\n%d %d", playerListHead->playerData.playerId, playerListTail->playerData.playerId);
    displayMenu();
    return 0;
}

// initializing players list
void initializePlayers()
{
    for (int currentInx = 0; currentInx < playerCount; currentInx++)
    {
        PlayerNode *newPlayer = (PlayerNode *)malloc(sizeof(PlayerNode));
        if (newPlayer == NULL)
        {
            printf("\nMemory allocation failed.");
            exit(1);
        }

        newPlayer->next = NULL;
        newPlayer->playerData.playerId = players[currentInx].id;
        newPlayer->playerData.playerName = players[currentInx].name;
        newPlayer->playerData.teamName = players[currentInx].team;
        newPlayer->playerData.role = players[currentInx].role;
        newPlayer->playerData.totalRuns = players[currentInx].totalRuns;
        newPlayer->playerData.battingAverage = players[currentInx].battingAverage;
        newPlayer->playerData.strikeRate = players[currentInx].strikeRate;
        newPlayer->playerData.wickets = players[currentInx].wickets;
        newPlayer->playerData.economyRate = players[currentInx].economyRate;
        if (strcmp(players[currentInx].role, "Batsman") == 0)
        {
            newPlayer->playerData.performanceIndex = (players[currentInx].battingAverage * players[currentInx].strikeRate) / 100;
        }
        else if (strcmp(players[currentInx].role, "Bowler") == 0)
        {
            newPlayer->playerData.performanceIndex = (players[currentInx].wickets * 2) + (100 - players[currentInx].economyRate);
        }
        else if (strcmp(players[currentInx].role, "All-rounder") == 0)
        {
            newPlayer->playerData.performanceIndex = ((players[currentInx].battingAverage * players[currentInx].strikeRate) / 100) + (players[currentInx].wickets * 2);
        }

        if (playerListHead == NULL)
        {
            newPlayer->previous = NULL;
            playerListHead = newPlayer;
            playerListTail = newPlayer;
        }
        else
        {
            newPlayer->previous = playerListTail;
            playerListTail->next = newPlayer;
            playerListTail = newPlayer;
        }
    }
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
        int count = 0, strikerCount = 0;
        float strikeTotal = 0;
        for (int currentPlayer = 0; currentPlayer < playerCount; currentPlayer++)
        {
            if (strcmp(teams[currentInx], players[currentPlayer].team) == 0)
            {
                count++;
                if (strcmp(players[currentPlayer].role, "All-rounder") == 0 ||
                    strcmp(players[currentPlayer].role, "Batsman") == 0)
                {
                    strikerCount++;
                    strikeTotal += players[currentPlayer].strikeRate;
                }
            }
        }
        newTeam->teamData.totalPlayers = count;
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

const char *findTeamName(TeamNode *head, TeamNode *tail, int id)
{
    while (head != tail->next)
    {
        TeamNode *mid = findMid(head, tail);
        if (mid->teamData.teamId == id)
        {
            return mid->teamData.teamName;
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
    return "";
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
    const char *team;
    team = findTeamName(teamListHead, teamListTail, id);
    if (strcmp(team, "") == 0)
    {
        printf("\nTeam with id %d not found.", id);
        return;
    }
    newPlayer->playerData.teamName = team;

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

    printf("\nRole (1-Batsman, 2-Bowler, 3-All-rounder): ");
    int option;
    scanf("%d", &option);
    if (option == 1)
    {
        newPlayer->playerData.role = "Batsman";
    }
    else if (option == 2)
    {
        newPlayer->playerData.role = "Bowler";
    }
    else if (option == 3)
    {
        newPlayer->playerData.role = "All-rounder";
    }
    else
    {
        printf("\nInvalid option");
        return;
    }

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

    newPlayer->previous = playerListTail;
    playerListTail->next = newPlayer;
    playerListTail = newPlayer;

    printf("\nPlayer added successfully to Team India!");
}

void displayPlayersOfTeam() {}

void displayTeamsByStrikeRate() {}

void displayTopNPlayers() {}

void displayPlayersByRole() {}