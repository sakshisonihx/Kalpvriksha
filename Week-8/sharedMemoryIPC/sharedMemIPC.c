#include <stdio.h>
#include <sys/shm.h>
#include <unistd.h>

void sort(int array[], int size)
{
    for (int first = 0; first < size - 1; first++)
        for (int second = 0; second < size - first - 1; second++)
            if (array[second] > array[second + 1])
            {
                int temp = array[second];
                array[second] = array[second + 1];
                array[second + 1] = temp;
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
    int sharedMemoryID = shmget(IPC_PRIVATE, sizeof(int) * 50, IPC_CREAT | 0666);
    int *array = shmat(sharedMemoryID, NULL, 0);

    int size;
    printf("Enter number of elements: ");
    scanf("%d", &size);
    array[0] = size;

    printf("Enter elements:\n");
    for (int i = 1; i <= size; i++)
        scanf("%d", &array[i]);

    printf("Before sorting:\n");
    printArray(array, size);

    if (fork() == 0)
    {
        sort(array + 1, array[0]);
    }
    else
    {
        sleep(1);
        printf("After sorting:\n");
        printArray(array + 1, size);
        shmdt(array);
        shmctl(sharedMemoryID, IPC_RMID, NULL);
    }
    return 0;
}
