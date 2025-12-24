/*This program will not work in native windows. It will work in linux/unix environment that can be simulated in virtual machines or virtual boxes*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

void sort(int array[], int size)
{
    for (int first = 0; first < size - 1; first++)
        for (int second = 0; second < size - first - 1; second++)
            if (array[second] > array[second + 1])
            {
                int t = array[second];
                array[second] = array[second + 1];
                array[second + 1] = t;
            }
}

void printArray(int array[], int size)
{
    for (int currInx = 0; currInx < size; currInx++)
        printf("%d ", array[currInx]);
    printf("\n");
}

int main()
{
    int noOfElements;
    printf("Enter number of elements: ");
    scanf("%d", &noOfElements);

    int array[noOfElements];
    printf("Enter elements:\n");
    for (int currInx = 0; currInx < noOfElements; currInx++)
        scanf("%d", &array[currInx]);

    FILE *filePointer = fopen("data.txt", "w");
    for (int currInx = 0; currInx < noOfElements; currInx++)
        fprintf(filePointer, "%d ", array[currInx]);
    fclose(filePointer);

    printf("Before sorting:\n");
    printArray(array, noOfElements);

    if (fork() == 0) // Process 1
    {
        filePointer = fopen("data.txt", "r");
        for (int currInx = 0; currInx < noOfElements; currInx++)
            fscanf(filePointer, "%d", &array[currInx]);
        fclose(filePointer);

        sort(array, noOfElements);

        filePointer = fopen("data.txt", "w");
        for (int currInx = 0; currInx < noOfElements; currInx++)
            fprintf(filePointer, "%d ", array[currInx]);
        fclose(filePointer);
    }
    else // Process 2
    {
        wait(NULL);
        filePointer = fopen("data.txt", "r");
        for (int currInx = 0; currInx < noOfElements; currInx++)
            fscanf(filePointer, "%d", &array[currInx]);
        fclose(filePointer);

        printf("After sorting:\n");
        printArray(array, noOfElements);
    }
    return 0;
}
