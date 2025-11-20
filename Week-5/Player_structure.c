#include "Player_structure.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int totalHeapElements = 0;
PlayerNode *maxHeap[MAX_TEAMS] = {0};

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