#include "data_structures.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

HashNode *hashTable = NULL;
Queue LRUCache = {.front = NULL, .rear = NULL, .totalElements = 0};
int LRUCapacity = 0;

int getHashIndex(int key)
{
    return key % LRUCapacity;
}

DataElement *insertInCache(int key, char *value)
{
    DataElement *newElement = (DataElement *)malloc(sizeof(DataElement));
    if (newElement == NULL)
    {
        printf("\nMemory Allocation Failed");
        return;
    }

    int stringSize = strlen(value) + 1;
    newElement->data = (char *)malloc(stringSize * sizeof(char));
    if (newElement->data == NULL)
    {
        printf("\nMemory Allocation Failed");
        return;
    }

    strcpy(newElement->data, value);
    newElement->key = key;
    newElement->previous = NULL;
    if (LRUCache.front == NULL)
    {
        newElement->next = NULL;
        LRUCache.front = newElement;
        LRUCache.rear = newElement;
    }
    else
    {
        newElement->next = LRUCache.front;
        LRUCache.front->previous = newElement;
        LRUCache.front = newElement;
    }
    LRUCache.totalElements++;
    return LRUCache.front;
}

void updatePositionInCache(DataElement *cacheNode)
{
    if (LRUCache.front == cacheNode) // element already at MRU position
    {
        return;
    }
    else if (LRUCache.rear == cacheNode)
    {
        LRUCache.rear = cacheNode->previous;
        LRUCache.rear->next = NULL;
    }
    else
    {
        DataElement *previousElement = cacheNode->previous;
        previousElement->next = cacheNode->next;
        cacheNode->next->previous = previousElement;
    }
    LRUCache.front->previous = cacheNode;
    cacheNode->next = LRUCache.front;
    cacheNode->previous = NULL;
    LRUCache.front = cacheNode;
}