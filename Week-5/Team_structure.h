#ifndef TEAM_STRUCTURE_H
#define TEAM_STRUCTURE_H

#include "Player_structure.h"
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

//global variable
extern TeamNode teamsList[MAX_TEAMS];

//function for structure manipulation
void initializeTeams();
TeamNode *findTeam(int, int, int);
void printPlayers(TeamNode *, char *, int);

#endif