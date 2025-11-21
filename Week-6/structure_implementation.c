#include "data_structures.h"
#include <stdio.h>

int getHashIndex(int key)
{
    return key % LRUCapacity;
}