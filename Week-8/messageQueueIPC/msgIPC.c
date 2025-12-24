#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>

typedef struct
{
    long type;
    int array[50];
    int noOfElements;
} Message;

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
    key_t key = ftok("msgq", 65);
    int msgID = msgget(key, 0666 | IPC_CREAT);
    Message message;

    printf("Enter number of elements: ");
    scanf("%d", &message.noOfElements);

    printf("Enter elements:\n");
    for (int currInx = 0; currInx < message.noOfElements; currInx++)
        scanf("%d", &message.array[currInx]);

    printf("Before sorting:\n");
    printArray(message.array, message.noOfElements);

    message.type = 1;
    msgsnd(msgID, &message, sizeof(message), 0);

    if (fork() == 0)
    {
        msgrcv(msgID, &message, sizeof(message), 1, 0);
        sort(message.array, message.noOfElements);
        message.type = 2;
        msgsnd(msgID, &message, sizeof(message), 0);
    }
    else
    {
        msgrcv(msgID, &message, sizeof(message), 2, 0);
        printf("After sorting:\n");
        printArray(message.array, message.noOfElements);
        msgctl(msgID, IPC_RMID, NULL);
    }
    return 0;
}
