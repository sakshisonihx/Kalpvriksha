#include <stdio.h>
#include <stdlib.h>
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
    hashTable = (HashNode *)calloc(size, sizeof(HashNode));
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
}