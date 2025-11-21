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

// global variables
extern PlayerNode *maxHeap[MAX_TEAMS];
extern int totalHeapElements;
extern TeamNode teamsList[MAX_TEAMS];

// function for player structure manipulation
PlayerNode *initializePlayer(Player);
void insertInRoleList(PlayerNode *, PlayerNode **);

// function for team structure manipulation
void initializeTeams();
TeamNode *findTeam(int, int, int);
void printPlayers(TeamNode *, char *, int);

// function for sorting
void insertionSort(TeamNode[]);
void heapifyUp(int);
void heapifyDown(int);
PlayerNode *extractMax();
void insertInHeap(PlayerNode *);

#endif