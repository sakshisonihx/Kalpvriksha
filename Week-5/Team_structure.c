#include "Team_structure.h"
#include <stdio.h>
#include <string.h>

TeamNode teamsList[MAX_TEAMS] = {0};

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