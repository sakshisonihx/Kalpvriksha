#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

pthread_mutex_t mutexLock;

void *handleClient(void *argument)
{
    int socket = *(int *)argument;
    int choice, amount, balance;
    FILE *filePointer;

    read(socket, &choice, sizeof(choice));

    pthread_mutex_lock(&mutexLock);
    filePointer = fopen("../resource/accountDB.txt", "r");
    fscanf(filePointer, "%d", &balance);
    fclose(filePointer);

    if (choice == 1)
    {
        read(socket, &amount, sizeof(amount));
        if (amount <= balance)
        {
            balance -= amount;
            write(socket, &balance, sizeof(balance));
        }
        else
        {
            balance = -1;
            write(socket, &balance, sizeof(balance));
        }
    }
    else if (choice == 2)
    {
        read(socket, &amount, sizeof(amount));
        balance += amount;
        write(socket, &balance, sizeof(balance));
    }
    else if (choice == 3)
    {
        write(socket, &balance, sizeof(balance));
    }

    filePointer = fopen("../resource/accountDB.txt", "w");
    fprintf(filePointer, "%d", balance);
    fclose(filePointer);

    pthread_mutex_unlock(&mutexLock);
    close(socket);
    return NULL;
}

int main()
{
    int serverFD, clientFD;
    struct sockaddr_in adder;
    pthread_mutex_init(&mutexLock, NULL);

    serverFD = socket(AF_INET, SOCK_STREAM, 0);
    adder.sin_family = AF_INET;
    adder.sin_port = htons(8080);
    adder.sin_addr.s_addr = INADDR_ANY;

    bind(serverFD, (struct sockaddr *)&adder, sizeof(adder));
    listen(serverFD, 5);

    while (1)
    {
        clientFD = accept(serverFD, NULL, NULL);
        pthread_t t;
        pthread_create(&t, NULL, handleClient, &clientFD);
    }
}
