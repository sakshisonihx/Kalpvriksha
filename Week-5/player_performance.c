#include "Players_data.h"
#include "Player_structure.h"
#include "Team_structure.h"
#include "Sorting_functions.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NAME_LENGTH 51
#define PLAYERS_PER_TEAM 50
#define MAX_TEAMS 10
#define MAX_ID 1500

void displayMenu();
void addPlayerToTeam();
void displayPlayersOfTeam();
void displayTeamsByStrikeRate();
void displayTopNPlayers();
void displayPlayersByRole();

int main()
{
    initializeTeams();
    displayMenu();
    return 0;
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
        if (scanf("%d", &choice) != 1)
        {
            printf("\nError. Enter number");
            return;
        }
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

// add new player
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
    if (scanf("%d", &id) != 1)
    {
        printf("\nError. Enter Number");
        return;
    }
    if (id > MAX_ID || id < 1)
    {
        printf("Enter a value between 1 and %d", MAX_ID);
        return;
    }

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
    if (scanf("%d", &newPlayer->playerData.playerId) != 1)
    {
        printf("\nError. Enter number");
        return;
    }

    int clearInput;
    while ((clearInput = getchar()) != '\n' && clearInput != EOF);
    printf("\nName: ");
    char name[NAME_LENGTH];
    fgets(name, NAME_LENGTH, stdin);
    name[strcspn(name, "\n")] = '\0';
    newPlayer->playerData.playerName = strdup(name);

    printf("\nTotal Runs: ");
    if (scanf("%d", &newPlayer->playerData.totalRuns) != 1)
    {
        printf("\nError. Enter number");
        return;
    }

    printf("\nBatting Average: ");
    if (scanf("%f", &newPlayer->playerData.battingAverage) != 1)
    {
        printf("\nError. Enter number(floating point/decimal)");
        return;
    }

    printf("\nStrike Rate: ");
    if (scanf("%f", &newPlayer->playerData.strikeRate) != 1)
    {
        printf("\nError. Enter number(floating point/decimal)");
        return;
    }

    printf("\nWickets: ");
    if (scanf("%d", &newPlayer->playerData.wickets) != 1)
    {
        printf("\nError. Enter number");
        return;
    }

    printf("\nEconomy Rate: ");
    if (scanf("%f", &newPlayer->playerData.economyRate) != 1)
    {
        printf("\nError. Enter number(floating point/decimal)");
        return;
    }

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
        newPlayer->playerData.performanceIndex = ((newPlayer->playerData.battingAverage * newPlayer->playerData.strikeRate) / 100)
        + (newPlayer->playerData.wickets * 2);
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

// display all playrs of team
void displayPlayersOfTeam()
{
    int id;
    printf("\nEnter Team ID: ");
    if (scanf("%d", &id) != 1)
    {
        printf("\nError. Enter number");
        return;
    }
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

// display teams by average batting strike rate
void displayTeamsByStrikeRate()
{
    TeamNode temp[MAX_TEAMS];
    for (int currInx = 0; currInx < MAX_TEAMS; currInx++)
    {
        temp[currInx] = teamsList[currInx];
    }
    insertionSort(temp);

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

// display N players of a team by performance index
void displayTopNPlayers()
{
    int id, noOfPlayers, choice;
    printf("\nEnter team ID: ");
    if (scanf("%d", &id) != 1)
    {
        printf("\nError. Enter number");
        return;
    }
    TeamNode *foundTeam = findTeam(0, MAX_TEAMS - 1, id);
    if (foundTeam == NULL)
    {
        printf("\nTeam with id %d not found.", id);
        return;
    }

    printf("\nEnter role (1-Batsman, 2-Bowler, 3-All-rounder): ");
    scanf("%d", &choice);
    if (choice > 3 || choice < 1)
    {
        printf("\nInvalid choice");
        return;
    }
    printf("\nEnter number of players: ");
    if (scanf("%d", &noOfPlayers) != 1)
    {
        printf("\nError. Enter number");
        return;
    }

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

// display all players of a role across the teams
void displayPlayersByRole()
{
    int choice;
    printf("\nEnter role (1-Batsman, 2-Bowler, 3-All-rounder): ");
    scanf("%d", &choice);
    if (choice > 3 || choice < 1)
    {
        printf("\nInvalid choice");
        return;
    }

    printf("%s of all teams: ", (choice == 1) ? "Batsmen" : (choice == 2) ? "Bowlers" : "All-rounders");
    totalHeapElements = 0;
    for (int currentInx = 0; currentInx < MAX_TEAMS; currentInx++)
    {
        PlayerNode *head = NULL;
        if (choice == 1)
        {
            head = teamsList[currentInx].allBatsmen;
        }
        else if (choice == 2)
        {
            head = teamsList[currentInx].allBowlers;
        }
        if (choice == 3)
        {
            head = teamsList[currentInx].allAllRounders;
        }
        insertInHeap(head);
    }

    printf("\n");
    for (int i = 0; i < 145; i++)
    {
        printf("%c", '=');
    }
    printf("\n%-4s %-25s %-15s %-15s %-10s %-15s %-15s %-10s %-15s %-15s", "ID", "Name", "Team", "Role", "Runs", "Batting Avg", "Strike Rate", "Wickets", "Economy Rate", "Perf. Index");
    printf("\n");
    for (int i = 0; i < 145; i++)
    {
        printf("%c", '=');
    }

    while (totalHeapElements > 0)
    {
        PlayerNode *bestPlayer = extractMax();
        printf("\n%-5d %-25s %-15s %-15s %-10d %-15.2f %-15.2f %-10d %-15.2f %-15.2f", bestPlayer->playerData.playerId,
            bestPlayer->playerData.playerName, bestPlayer->playerData.teamName, bestPlayer->playerData.role,
            bestPlayer->playerData.totalRuns, bestPlayer->playerData.battingAverage, bestPlayer->playerData.strikeRate,
            bestPlayer->playerData.wickets, bestPlayer->playerData.economyRate, bestPlayer->playerData.performanceIndex);

        PlayerNode *nextPlayer = bestPlayer->next;
        if (nextPlayer != NULL)
        {
            insertInHeap(nextPlayer);
        }
    }
    printf("\n");
    for (int i = 0; i < 145; i++)
    {
        printf("%c", '=');
    }
}