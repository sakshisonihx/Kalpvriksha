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
        newElement->next = LRUCache.front->next;
        LRUCache.front->previous = newElement;
        LRUCache.front = newElement;
    }
    LRUCache.totalElements++;
    return LRUCache.front;
}