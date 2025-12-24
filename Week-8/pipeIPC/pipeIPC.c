#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

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
    int pipe1[2]; // parent → child
    int pipe2[2]; // child → parent

    pipe(pipe1);
    pipe(pipe2);

    int size;
    printf("Enter number of elements: ");
    scanf("%d", &size);

    int array[size];
    printf("Enter elements:\n");
    for (int currInx = 0; currInx < size; currInx++)
        scanf("%d", &array[currInx]);

    printf("Before sorting:\n");
    printArray(array, size);

    if (fork() == 0) // child (process 1)
    {
        close(pipe1[1]);
        close(pipe2[0]);

        read(pipe1[0], array, sizeof(array));
        sort(array, size);
        write(pipe2[1], array, sizeof(array));
    }
    else // parent (process 2)
    {
        close(pipe1[0]);
        close(pipe2[1]);

        write(pipe1[1], array, sizeof(array));
        wait(NULL);
        read(pipe2[0], array, sizeof(array));

        printf("After sorting:\n");
        printArray(array, size);
    }

    return 0;
}
