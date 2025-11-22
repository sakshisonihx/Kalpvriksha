#ifndef DATA_STRUCTURES_H
#define DATA_STRUCTURES_H

typedef struct DataElement
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
    int totalElements;
} Queue;

typedef struct HashNode
{
    DataElement *storedDataPtr;
    struct HashNode *next;
} HashNode;

extern HashNode *hashTable;
extern Queue LRUCache;
extern int LRUCapacity;

// hash table manipulation functions
int getHashIndex(int);

// queue manipulation functions
DataElement *insertInCache(int, char *);
void updatePositionInCache(DataElement *);
void removeFromCache();

#endif