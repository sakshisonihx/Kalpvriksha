#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>

int main()
{
    int socket, choice, amount, balance;
    struct sockaddr_in server;

    socket = socket(AF_INET, SOCK_STREAM, 0);
    server.sin_family = AF_INET;
    server.sin_port = htons(8080);
    server.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(socket, (struct sockaddr *)&server, sizeof(server));

    printf("1.Withdraw\n2.Deposit\n3.Display Balance\n4.Exit\n");
    scanf("%d", &choice);

    write(socket, &choice, sizeof(choice));

    if (choice == 1 || choice == 2)
    {
        printf("Enter amount: ");
        scanf("%d", &amount);
        write(socket, &amount, sizeof(amount));
    }

    if (choice != 4)
    {
        read(socket, &balance, sizeof(balance));
        if (balance == -1)
            printf("Insufficient Balance\n");
        else
            printf("Current Balance: %d\n", balance);
    }

    close(socket);
    return 0;
}
