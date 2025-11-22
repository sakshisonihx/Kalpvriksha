#include "data_structures.h"
#include <stdio.h>

HashNode *hashTable = NULL;
Queue LRUCache = {.front = NULL, .rear = NULL};
int LRUCapacity = 0;

int getHashIndex(int key)
{
    return key % LRUCapacity;
}