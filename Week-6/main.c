#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "data_structures.h"
#include <ctype.h>

#define INPUT_SIZE 500
int cacheInitialized = 0;

void takeInput(char *);
int checkInput(char *);
void createCache(int);
void get(int);
void put(int, char *);

int main()
{
    char inputBuffer[INPUT_SIZE];
    while (1)
    {
        printf("\n");
        if (fgets(inputBuffer, INPUT_SIZE, stdin) == NULL)
            break;
        inputBuffer[strcspn(inputBuffer, "\n")] = '\0';
        if (strcmp(inputBuffer, "exit") == 0)
        {
            break;
        }
        takeInput(inputBuffer);
    }
    return 0;
}

void takeInput(char *line)
{
    char *function = strtok(line, " \n");
    char *arg1 = strtok(NULL, " \n");
    char *arg2 = NULL;
    if (function == NULL)
    {
        return;
    }

    if (strcmp(function, "put") == 0 && arg1 != NULL)
    {
        arg2 = strtok(NULL, " \n");
    }

    if (strtok(NULL, " \n") != NULL)
    {
        printf("\nToo many arguments given.");
        return;
    }

    if (cacheInitialized == 0)
    {
        if (strcmp(function, "createCache") == 0)
        {
            if (arg1 == NULL)
            {
                printf("\ncreateCache needs <size> input.");
                return;
            }
            else if (checkInput(arg1))
            {
                createCache(atoi(arg1));
            }
            else
            {
                printf("\nError. Enter a number");
                return;
            }
        }
        else
        {
            printf("\nError. Cache not initialized.");
            return;
        }
    }
    else // cache initialized!
    {
        if (strcmp(function, "createCache") == 0)
        {
            printf("Error. Cache already initialized");
            return;
        }
        else if (strcmp(function, "get") == 0)
        {
            if (arg1 == NULL)
            {
                printf("\nget needs <key> input.");
                return;
            }
            else if (checkInput(arg1))
            {
                get(atoi(arg1));
            }
            else
            {
                printf("\nError. Enter a number");
                return;
            }
        }
        else if (strcmp(function, "put") == 0)
        {
            if (arg1 && arg2)
            {
                if (checkInput(arg1))
                {
                    put(atoi(arg1), arg2);
                }
                else
                {
                    printf("\nError. Enter a number for key");
                    return;
                }
            }
            else
            {
                printf("\nput needs <key> and <value> input.");
                return;
            }
        }
        else
        {
            printf("\nInvalid input\n");
        }
    }
}

int checkInput(char *string)
{
    char *temp = string;
    while (*temp != '\0')
    {
        if (!isdigit(*temp))
        {
            return 0;
        }
        temp += 1;
    }
    return 1;
}

void createCache(int size)
{
    if (size < 1 || size > 1000)
    {
        printf("\nSize should be a number between 1 and 1000");
        return;
    }

    LRUCapacity = size;
    hashTable = (HashNode **)calloc(size, sizeof(HashNode *));
    if (hashTable == NULL)
    {
        printf("\nMemory allocation failed.");
        return;
    }
    cacheInitialized = 1;
}

void get(int key)
{
    int index = getHashIndex(key);
    HashNode *currentHashNode = hashTable[index];
    while (currentHashNode != NULL)
    {
        if (currentHashNode->storedDataPtr->key == key)
        {
            printf("\n%s", currentHashNode->storedDataPtr->data);
            updatePositionInCache(currentHashNode->storedDataPtr);
            return;
        }
        currentHashNode = currentHashNode->next;
    }
    // key not found
    printf("\nNULL");
}

void put(int key, char *value)
{
    if (LRUCache.totalElements == LRUCapacity)
    {
        removeFromHashTable(LRUCache.rear->key);
        removeFromCache();
    }
    int index = getHashIndex(key);
    HashNode *currentHashNode = hashTable[index];

    // key present
    int newStringSize = strlen(value);
    while (currentHashNode != NULL)
    {
        if (currentHashNode->storedDataPtr->key == key)
        {
            int oldStringSize = strlen(currentHashNode->storedDataPtr->data);
            if (newStringSize > oldStringSize)
            {
                currentHashNode->storedDataPtr->data = (char *)realloc(currentHashNode->storedDataPtr->data,
                                                                       (newStringSize + 1) * sizeof(char));
                if (currentHashNode->storedDataPtr->data == NULL)
                {
                    printf("\nMemory allocation failed.");
                    return;
                }
            }
            strcpy(currentHashNode->storedDataPtr->data, value);
            updatePositionInCache(currentHashNode->storedDataPtr);
            return;
        }
        currentHashNode = currentHashNode->next;
    }

    // key not present
    HashNode *newHashNode = (HashNode *)malloc(sizeof(HashNode));
    if (newHashNode == NULL)
    {
        printf("\nMemory allocation failed.");
        return;
    }
    newHashNode->storedDataPtr = insertInCache(key, value);
    if (newHashNode->storedDataPtr == NULL)
    {
        printf("\nMemory allocation failed.");
        return;
    }

    newHashNode->next = hashTable[index];
    hashTable[index] = newHashNode;
}