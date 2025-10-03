#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define FILENAME "users.txt"
#define MAX_NAME_LEN 50
#define MAX_LINE_LEN 256

typedef struct
{
    int id;
    char name[MAX_NAME_LEN];
    int age;
} User;

int getNextId()
{
    FILE *file = fopen(FILENAME, "r");
    int maxId = 0;
    User user;

    if (file == NULL)
    {
        return 1; // Start from 1 if file doesn't exist
    }

    // Find the maximum
    while (fscanf(file, "%d %s %d\n", &user.id, user.name, &user.age) == 3)
    {
        if (user.id > maxId)
        {
            maxId = user.id;
        }
    }
    fclose(file);
    return maxId + 1;
}

// Replacing \n with \0 in input
void get_safe_input(char *buffer, int size)
{
    if (fgets(buffer, size, stdin) != NULL)
    {
        // Remove trailing newline character added by fgets
        buffer[strcspn(buffer, "\n")] = 0;
    }
}

void createUser()
{
    FILE *file = fopen(FILENAME, "a");
    if (file == NULL)
    {
        perror("Error opening file for writing");
        return;
    }

    User newUser;
    newUser.id = getNextId();

    printf("\n--- Add new user ---\n");
    printf("Generated ID: %d\n", newUser.id);

    printf("Enter name (max %d chars): ", MAX_NAME_LEN - 1);
    get_safe_input(newUser.name, MAX_NAME_LEN);

    printf("Enter age: ");
    // check for invalid input
    if (scanf("%d", &newUser.age) != 1)
    {
        printf("Invalid age input.\n");
        fclose(file);
        // Clearing stdin buffer after failed scanf
        int c;
        while ((c = getchar()) != '\n' && c != EOF)
            ;
        return;
    }
    // Clearing stdin buffer after successful scanf
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;

    fprintf(file, "%d %s %d\n", newUser.id, newUser.name, newUser.age);
    printf("User created successfully: ID %d\n", newUser.id);

    fclose(file);
}

void readUsers()
{
    FILE *file = fopen(FILENAME, "r");
    if (file == NULL)
    {
        printf("\n--- Display users ---\n");
        printf("The user file (%s) does not exist or is empty.\n", FILENAME);
        return;
    }

    User user;
    int recordsFound = 0;

    printf("\n--- Users in %s ---\n", FILENAME);
    printf("---------------------------------------\n");
    printf("| %-4s | %-20s | %-4s |\n", "ID", "Name", "Age");
    printf("---------------------------------------\n");

    while (fscanf(file, "%d %s %d\n", &user.id, user.name, &user.age) == 3)
    {
        printf("| %-4d | %-20s | %-4d |\n", user.id, user.name, user.age);
        recordsFound++;
    }
    printf("---------------------------------------\n");

    if (recordsFound == 0)
    {
        printf("No valid records found.\n");
    }

    fclose(file);
}

void updateUser()
{
    int targetID;
    char newName[MAX_NAME_LEN];
    int newAge;

    printf("\n--- Update user ---\n");
    printf("Enter ID of user to update: ");
    if (scanf("%d", &targetID) != 1)
    {
        printf("Invalid ID input.\n");
        int c;
        while ((c = getchar()) != '\n' && c != EOF)
            ;
        return;
    }
    // Consuming the rest of the line after reading ID
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;

    printf("Enter new name: ");
    get_safe_input(newName, MAX_NAME_LEN);

    printf("Enter new age: ");
    if (scanf("%d", &newAge) != 1)
    {
        printf("Invalid age input.\n");
        while ((c = getchar()) != '\n' && c != EOF)
            ;
        return;
    }
    while ((c = getchar()) != '\n' && c != EOF)
        ;

    FILE *original = fopen(FILENAME, "r");
    if (original == NULL)
    {
        printf("File not found.\n");
        return;
    }

    FILE *temp = fopen("temp.txt", "w");
    if (temp == NULL)
    {
        perror("Error creating temporary file");
        fclose(original);
        return;
    }

    User user;
    int found = 0;

    while (fscanf(original, "%d %s %d\n", &user.id, user.name, &user.age) == 3)
    {
        if (user.id == targetID)
        {
            // Found the target user so write updated data
            fprintf(temp, "%d %s %d\n", targetID, newName, newAge);
            found = 1;
        }
        else // copy original data
        {
            fprintf(temp, "%d %s %d\n", user.id, user.name, user.age);
        }
    }

    fclose(original);
    fclose(temp);

    if (found)
    {
        remove(FILENAME);
        rename("temp.txt", FILENAME);
        printf("User ID %d updated successfully.\n", targetID);
    }
    else
    {
        remove("temp.txt");
        printf("Error: User with ID %d not found.\n", targetID);
    }
}

void deleteUser()
{
    int targetID;
    printf("\n--- Delete User ---\n");
    printf("Enter ID of user to delete: ");
    if (scanf("%d", &targetID) != 1)
    {
        printf("Invalid ID input.\n");
        int c;
        while ((c = getchar()) != '\n' && c != EOF)
            ;
        return;
    }
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;

    FILE *original = fopen(FILENAME, "r");
    if (original == NULL)
    {
        printf("File not found.\n");
        return;
    }

    FILE *temp = fopen("temp.txt", "w");
    if (temp == NULL)
    {
        perror("Error creating temporary file");
        fclose(original);
        return;
    }

    User user;
    int found = 0;

    while (fscanf(original, "%d %s %d\n", &user.id, user.name, &user.age) == 3)
    {
        if (user.id == targetID)
        {
            // if found, do not write the data, effectively deleting it
            found = 1;
        }
        else
        {
            fprintf(temp, "%d %s %d\n", user.id, user.name, user.age);
        }
    }

    fclose(original);
    fclose(temp);

    if (found)
    {
        remove(FILENAME);
        rename("temp.txt", FILENAME);
        printf("User ID %d deleted successfully.\n", targetID);
    }
    else
    {
        remove("temp.txt");
        printf("Error: User with ID %d not found.\n", targetID);
    }
}

void displayMenu()
{
    printf("----------------------------------\n");
    printf("1. Create user\n");
    printf("2. Read All users\n");
    printf("3. Update user\n");
    printf("4. Delete user\n");
    printf("5. Exit\n");
    printf("----------------------------------\n");
    printf("Enter your choice: ");
}

int main()
{
    int choice;
    while (1)
    {
        displayMenu();
        if (scanf("%d", &choice) != 1)
        {
            printf("\nInvalid input: Enter a number.\n");
            int c;
            while ((c = getchar()) != '\n' && c != EOF)
                ;
            continue;
        }
        // Consume \n left by scanf
        int c;
        while ((c = getchar()) != '\n' && c != EOF)
            ;

        switch (choice)
        {
        case 1:
            createUser();
            break;
        case 2:
            readUsers();
            break;
        case 3:
            updateUser();
            break;
        case 4:
            deleteUser();
            break;
        case 5:
            printf("\nExiting program.\n");
            return 0;
        default:
            printf("\nInvalid choice. Enter a number between 1 and 5.\n");
        }
    }

    return 0;
}
