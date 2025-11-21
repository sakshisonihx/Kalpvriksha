#include "Data_structures.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int totalHeapElements = 0;
PlayerNode *maxHeap[MAX_TEAMS] = {0};
TeamNode teamsList[MAX_TEAMS] = {0};

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
        printf("\n%-5d %-25s %-15s %-10d %-15.2f %-15.2f %-10d %-15.2f %-15.2f", temp->playerData.playerId,
               temp->playerData.playerName, temp->playerData.role, temp->playerData.totalRuns, temp->playerData.battingAverage,
               temp->playerData.strikeRate, temp->playerData.wickets, temp->playerData.economyRate, temp->playerData.performanceIndex);
        temp = temp->next;
        count++;
    }
}

// sorting teams according to Avg Bat Strike Rate
void insertionSort(TeamNode array[])
{
    for (int i = 1; i < MAX_TEAMS; i++)
    {
        TeamNode currentTeam = array[i];
        int j = i - 1;
        while (j >= 0 && array[j].teamData.averageBattingStrikeRate < currentTeam.teamData.averageBattingStrikeRate)
        {
            array[j + 1] = array[j];
            j--;
        }
        array[j + 1] = currentTeam;
    }
}

// heapify array upwards (child to parent)
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

// heapify array downwards (parent to child)
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

// extract max element from heap (root)
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

// insert element in heap
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