#ifndef DATA_STRUCTURES_H
#define DATA_STRUCTURES_H

typedef struct
{
    int key;
    char *data;
    struct DataElement *next;
    struct DataElement *previous;
} DataElement;

typedef struct
{
    DataElement *front; // Most Recently Used
    DataElement *rear;  // Least Recently Used
} Queue;

typedef struct
{
    DataElement *storedDataPtr;
    struct HashNode *head;
} HashNode;

extern HashNode *hashTable;
extern Queue LRUCache;
extern int LRUCapacity;

// implementation functions
int getHashIndex(int);

#endif