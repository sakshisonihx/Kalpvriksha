#include <stdio.h>

void countFrequency(int array[], int size)
{
    int visited[size];
    for (int currentIndex = 0; currentIndex < size; currentIndex++)
    {
        visited[currentIndex] = 0;
    }
    for (int currentIndex = 0; currentIndex < size; currentIndex++)
    {
        if (visited[currentIndex])
            continue;
        visited[currentIndex] = 1;
        int count = 1;
        for (int compareIndex = currentIndex + 1; compareIndex < size; compareIndex++)
        {
            if (array[compareIndex] == array[currentIndex])
            {
                visited[compareIndex] = 1;
                count++;
            }
        }
        printf("Element %d appears %d times\n", array[currentIndex], count);
    }
}

int main()
{
    int size;
    printf("Enter size of array: ");
    if (scanf("%d", &size) != 1 || size <= 0)
    {
        printf("Invalid size input");
        return 1;
    }

    int array[size];
    printf("Enter array elements:\n");
    for (int i = 0; i < size; i++)
    {
        if (scanf("%d", array + i) != 1)
        {
            printf("Invalid element input. Enter integer.");
            return 1;
        }
    }
    countFrequency(array, size);
    return 0;
}