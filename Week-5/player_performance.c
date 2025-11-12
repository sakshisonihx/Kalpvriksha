#include "Players_data.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct PlayerNode
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
    struct PlayerNode *next;
    struct PlayerNode *previous;
} PlayerNode;

typedef struct TeamNode
{
    int teamId;
    const char *teamName;
    int totalPlayers;
    float averageBattingStrikeRate;
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
    printf("\n%d %d", teamListHead->teamId, teamListTail->teamId);
    printf("\n%d %d", playerListHead->playerId, playerListTail->playerId);
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
        newPlayer->playerId = players[currentInx].id;
        newPlayer->playerName = players[currentInx].name;
        newPlayer->teamName = players[currentInx].team;
        newPlayer->role = players[currentInx].role;
        newPlayer->totalRuns = players[currentInx].totalRuns;
        newPlayer->battingAverage = players[currentInx].battingAverage;
        newPlayer->strikeRate = players[currentInx].strikeRate;
        newPlayer->wickets = players[currentInx].wickets;
        newPlayer->economyRate = players[currentInx].economyRate;
        if (strcmp(players[currentInx].role, "Batsman") == 0)
        {
            newPlayer->performanceIndex = (players[currentInx].battingAverage * players[currentInx].strikeRate) / 100;
        }
        else if (strcmp(players[currentInx].role, "Bowler") == 0)
        {
            newPlayer->performanceIndex = (players[currentInx].wickets * 2) + (100 - players[currentInx].economyRate);
        }
        else if (strcmp(players[currentInx].role, "All-rounder") == 0)
        {
            newPlayer->performanceIndex = ((players[currentInx].battingAverage * players[currentInx].strikeRate) / 100) + (players[currentInx].wickets * 2);
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
        newTeam->teamId = currentInx + 1;
        newTeam->teamName = teams[0];
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
        newTeam->totalPlayers = count;
        newTeam->averageBattingStrikeRate = strikeTotal / strikerCount;

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

void addPlayerToTeam() {}

void displayPlayersOfTeam() {}

void displayTeamsByStrikeRate() {}

void displayTopNPlayers() {}

void displayPlayersByRole() {}