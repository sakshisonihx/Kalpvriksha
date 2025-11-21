#ifndef PLAYER_STRUCTURE_H
#define PLAYER_STRUCTURE_H

#include "Players_data.h"
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

// global variables
extern PlayerNode *maxHeap[MAX_TEAMS];
extern int totalHeapElements;

// function for structure manipulation
PlayerNode *initializePlayer(Player);
void insertInRoleList(PlayerNode *, PlayerNode **);
void heapifyUp(int);
void heapifyDown(int);
PlayerNode *extractMax();
void insertInHeap(PlayerNode *);

#endif