#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "data_structures.h"

int main()
{
    int size;
    printf("Enter size: ");
    if (scanf("%d", size) != 1)
    {
        printf("\nError. Enter number.");
        clearBuffer();
        return;
    };
    if (size < 1 || size > 1000)
    {
        printf("\nSize should be between 1 and 1000");
        return;
    }
    return 0;
    createCache(size);
}

// clearing buffer
void clearBuffer()
{
    int character;
    while ((character = getchar()) != '\n' && character != EOF)
        ;
}

void createCache(int size)
{
    LRUCapacity = size;
    hashTable = (HashNode **)calloc(size, sizeof(HashNode *));
    if (hashTable == NULL)
    {
        printf("\nMemory allocation failed.");
        return;
    }
}

char *get(int key)
{
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
    newHashNode->next = hashTable[index];
    hashTable[index] = newHashNode;
}