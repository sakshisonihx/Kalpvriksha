#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>

int main()
{
    int choice, amount, balance;
    while (1)
    {
        int Csocket = socket(AF_INET, SOCK_STREAM, 0);
        struct sockaddr_in server;

        server.sin_family = AF_INET;
        server.sin_port = htons(8080);
        server.sin_addr.s_addr = inet_addr("127.0.0.1");

        connect(Csocket, (struct sockaddr *)&server, sizeof(server));

        printf("1.Withdraw\n2.Deposit\n3.Display Balance\n4.Exit\n");
        scanf("%d", &choice);

        write(Csocket, &choice, sizeof(choice));
        if (choice == 4)
        {
            close(Csocket);
            break;
        }

        if (choice == 1 || choice == 2)
        {
            printf("Enter amount: ");
            scanf("%d", &amount);
            write(Csocket, &amount, sizeof(amount));
        }

        read(Csocket, &balance, sizeof(balance));
        if (balance == -1)
            printf("Insufficient Balance\n");
        else
            printf("Current Balance: %d\n", balance);

        close(Csocket);
    }
    return 0;
}
