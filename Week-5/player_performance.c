#include "Players_data.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NAME_LENGTH 51
#define PLAYERS_PER_TEAM 50
#define MAX_TEAMS 10
#define MAX_ID 1500

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
    // helper fields
    float strikeRateSum;
    int totalStrikers;
    PlayerNode *allBatsmen;
    PlayerNode *allBowlers;
    PlayerNode *allAllRounders;
} TeamNode;

static TeamNode teamsList[MAX_TEAMS] = {0};
static PlayerNode *maxHeap[MAX_TEAMS];
static int totalHeapElements = 0;

PlayerNode *initializePlayer(Player);
void initializeTeams();
void displayMenu();
void addPlayerToTeam();
void displayPlayersOfTeam();
void displayTeamsByStrikeRate();
void displayTopNPlayers();
void displayPlayersByRole();
// helper functions
void insertInRoleList(PlayerNode *, PlayerNode **);
TeamNode *findTeam(int, int, int);
void printPlayers(TeamNode *, char *, int);
void heapifyUp(int);
void heapifyDown(int);
PlayerNode *extractMax();
void insertInHeap(PlayerNode *);

int main()
{
    initializeTeams();
    displayMenu();
    return 0;
}

// inserting players in lists according to their role
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

// initializing teams array
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

// finding team in array (binary search)
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

// printing player details
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
        printf("\n%-5d %-25s %-15s %-10d %-15.2f %-15.2f %-10d %-15.2f %-15.2f", temp->playerData.playerId, temp->playerData.playerName, temp->playerData.role, temp->playerData.totalRuns, temp->playerData.battingAverage, temp->playerData.strikeRate, temp->playerData.wickets, temp->playerData.economyRate, temp->playerData.performanceIndex);
        temp = temp->next;
        count++;
    }
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

void heapifyUp(int index)
{
    int parent = (index - 1) / 2;
    if (index != 0 && maxHeap[index]->playerData.performanceIndex > maxHeap[parent]->playerData.performanceIndex)
    {
        PlayerNode *temp = maxHeap[index];
        maxHeap[index] = maxHeap[parent];
        maxHeap[parent] = temp;
        heapifyUp(parent);
    }
}

void heapifyDown(int index)
{
    int leftChild = 2 * index + 1;
    int rightChild = 2 * index + 2;
    int largest = index;
    if (leftChild < totalHeapElements && maxHeap[leftChild]->playerData.performanceIndex > maxHeap[largest]->playerData.performanceIndex)
    {
        largest = leftChild;
    }
    if (rightChild < totalHeapElements && maxHeap[rightChild]->playerData.performanceIndex > maxHeap[largest]->playerData.performanceIndex)
    {
        largest = rightChild;
    }
    if (largest != index)
    {
        PlayerNode *temp = maxHeap[largest];
        maxHeap[largest] = maxHeap[index];
        maxHeap[index] = temp;
        heapifyDown(largest);
    }
}

PlayerNode *extractMax()
{
    if (totalHeapElements <= 0)
    {
        return NULL;
    }
    PlayerNode *element = maxHeap[0];
    maxHeap[0] = maxHeap[totalHeapElements - 1];
    totalHeapElements--;
    heapifyDown(0);
    return element;
}

void insertInHeap(PlayerNode *element)
{
    if (totalHeapElements >= MAX_TEAMS)
    {
        return;
    }
    maxHeap[totalHeapElements] = element;
    heapifyUp(totalHeapElements);
    totalHeapElements++;
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
        printf("\n%-5d %-25s %-15s %-15s %-10d %-15.2f %-15.2f %-10d %-15.2f %-15.2f", bestPlayer->playerData.playerId, bestPlayer->playerData.playerName, bestPlayer->playerData.teamName, bestPlayer->playerData.role, bestPlayer->playerData.totalRuns, bestPlayer->playerData.battingAverage, bestPlayer->playerData.strikeRate, bestPlayer->playerData.wickets, bestPlayer->playerData.economyRate, bestPlayer->playerData.performanceIndex);

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