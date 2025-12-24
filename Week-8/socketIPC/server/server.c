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
    free(argument);
    int choice, amount, balance;
    FILE *filePointer;

    if (read(socket, &choice, sizeof(choice)) <= 0)
    {
        close(socket);
        return NULL;
    }

    pthread_mutex_lock(&mutexLock);
    filePointer = fopen("../resource/accountDB.txt", "r");
    fscanf(filePointer, "%d", &balance);
    fclose(filePointer);
    int newBalance = balance;
    if (choice == 1)
    {
        read(socket, &amount, sizeof(amount));
        if (amount <= balance)
        {
            newBalance = balance - amount;
        }
        else
        {
            newBalance = -1;
        }
    }
    else if (choice == 2)
    {
        read(socket, &amount, sizeof(amount));
        newBalance = balance + amount;
    }
    else if (choice == 3)
    {
        write(socket, &balance, sizeof(balance));
        pthread_mutex_unlock(&mutexLock);
        close(socket);
        return NULL;
    }
    else if (choice == 4)
    {
        pthread_mutex_unlock(&mutexLock);
        close(socket);
        return NULL;
    }

    if (newBalance != -1)
    {
        filePointer = fopen("../resource/accountDB.txt", "w");
        fprintf(filePointer, "%d", balance);
        fclose(filePointer);
    }

    write(socket, &newBalance, sizeof(newBalance));

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
        int *newSock = malloc(sizeof(int));
        *newSock = accept(server_fd, NULL, NULL);
        pthread_t thread;
        pthread_create(&thread, NULL, handle_client, newSock);
    }
}
