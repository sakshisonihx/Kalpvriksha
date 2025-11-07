#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NO_OF_BLOCKS 2000
#define BLOCK_SIZE 64
#define NAME_MAX_LENGTH 50
#define MAX_BLOCKS_NUMBER 50

typedef struct FreeBlockNode
{
    int index;
    struct FreeBlockNode *previous;
    struct FreeBlockNode *next;
} FreeBlockNode;

typedef enum
{
    DATAFILE,
    DIRECTORY
} NodeType;

typedef struct FileNode
{
    char name[NAME_MAX_LENGTH + 1];
    NodeType type;
    struct FileNode *previous;
    struct FileNode *next;
    struct FileNode *parent;
    union
    {
        // data if file
        struct
        {
            int allocatedBlocksArray[MAX_BLOCKS_NUMBER];
            int noOfBlocks;
            int sizeOfContent;
        } fileData;

        // data if directory
        struct FileNode *child;
    };
} FileNode;

char virtualDisk[NO_OF_BLOCKS][BLOCK_SIZE];
FileNode *root = NULL;
FileNode *currentDirectory = NULL;
int totalUsedBlocks = 0;
FreeBlockNode *headBlock = NULL;
FreeBlockNode *tailBlock = NULL;

void exitProgram();
void printPath(FileNode *);
void deallocateBlock(int);
FileNode *findFile(char *);
int allocateBlock();
void addFileNode(FileNode *);

void initializeVFS()
{
    // creating free blocks list
    for (int currIndex = 0; currIndex < NO_OF_BLOCKS; currIndex++)
    {
        FreeBlockNode *newNode = (FreeBlockNode *)malloc(sizeof(FreeBlockNode));
        if (newNode == NULL)
        {
            printf("Error. Can't allocate free blocks\n");
            return;
        }

        newNode->index = currIndex;
        if (headBlock == NULL) // first block
        {
            newNode->next = NULL;
            newNode->previous = NULL;
            headBlock = newNode;
            tailBlock = newNode;
        }
        else
        {
            tailBlock->next = newNode;
            newNode->previous = tailBlock;
            newNode->next = NULL;
            tailBlock = newNode;
        }
    }

    // initializing root directory
    root = (FileNode *)malloc(sizeof(FileNode));
    if (root == NULL)
    {
        printf("Failed to allocate memory\n");
        return;
    }
    strcpy(root->name, "/");
    root->type = DIRECTORY;
    root->parent = NULL;
    root->next = root;
    root->previous = root;
    root->child = NULL;
    currentDirectory = root;
}

void changeDirectory(char *path)
{
    if (path == NULL || strlen(path) == 0)
    {
        return;
    }

    if (strcmp(path, "..") == 0)
    {
        if (currentDirectory->parent != NULL)
        {
            currentDirectory = currentDirectory->parent;
            printf("Moved to ");
            printPath(currentDirectory);
            printf("\n");
        }
        else
        {
            printf("Already in root directory ('/').\n");
        }
        return;
    }

    FileNode *target = findFile(path);

    if (target == NULL || target->type != DIRECTORY)
    {
        printf("Error. Directory '%s' not found.\n", path);
        return;
    }

    currentDirectory = target;
    printf("Moved to ");
    printPath(currentDirectory);
    printf("\n");
}

void createFile(char *name)
{
    // safety check
    if (currentDirectory == NULL || currentDirectory->type != DIRECTORY)
    {
        printf("Error. Not inside a directory\n");
        return;
    }

    FileNode *dublicateFile = findFile(name);
    if (dublicateFile != NULL)
    {
        printf("Error. Dublicate name.\n");
        return;
    }

    FileNode *newNode = (FileNode *)malloc(sizeof(FileNode));
    strcpy(newNode->name, name);
    newNode->type = DATAFILE;
    newNode->parent = currentDirectory;
    newNode->fileData.noOfBlocks = 0;
    newNode->fileData.sizeOfContent = 0;
    newNode->next = NULL;
    newNode->previous = NULL;
    addFileNode(newNode);
    printf("File \'%s\' created successfully.\n", name);
}

void createDirectory(char *name)
{
    // safety check
    if (currentDirectory == NULL || currentDirectory->type != DIRECTORY)
    {
        printf("Error. Not inside a directory\n");
        return;
    }

    FileNode *dublicateFile = findFile(name);
    if (dublicateFile != NULL)
    {
        printf("Error. Dublicate name.\n");
        return;
    }

    FileNode *newNode = (FileNode *)malloc(sizeof(FileNode));
    strcpy(newNode->name, name);
    newNode->type = DIRECTORY;
    newNode->parent = currentDirectory;
    newNode->child = NULL;
    newNode->next = NULL;
    newNode->previous = NULL;
    addFileNode(newNode);
    printf("Directory \'%s\' created successfully.\n", name);
}

void addFileNode(FileNode *newFileNode)
{
    if (currentDirectory->type != DIRECTORY)
    {
        printf("Error. Adding FileNode failed.\n");
    }

    if (currentDirectory->child == NULL) // first file
    {
        currentDirectory->child = newFileNode;
        newFileNode->next = newFileNode;
        newFileNode->previous = newFileNode;
    }
    else
    {
        FileNode *head = currentDirectory->child;
        newFileNode->next = head;
        newFileNode->previous = head->previous;
        head->previous->next = newFileNode;
        head->previous = newFileNode;
    }
}

void writeOperation(char *fileName, char *fileData)
{
    FileNode *file = findFile(fileName);
    if (file == NULL || file->type != DATAFILE)
    {
        printf("Error. File not found\n");
        return;
    }

    if (file->fileData.noOfBlocks > 0)
    {
        for (int i = 0; i < file->fileData.noOfBlocks; i++)
        {
            deallocateBlock(file->fileData.allocatedBlocksArray[i]);
            file->fileData.allocatedBlocksArray[i] = -1;
        }
        file->fileData.noOfBlocks = 0;
        file->fileData.sizeOfContent = 0;
    }

    int dataLength = strlen(fileData);
    // smallest integer greater than or equal to division result. One space left for null termination
    int blocksNeeded = (dataLength + BLOCK_SIZE - 2) / (BLOCK_SIZE - 1);
    if (blocksNeeded > MAX_BLOCKS_NUMBER)
    {
        printf("Error. Content exceeds max blocks that can be used by file\n");
        return;
    }

    long remainingLength = dataLength;
    char *currentPtr = fileData;

    for (int i = 0; i < blocksNeeded; i++)
    {
        int blockIndex = allocateBlock();

        file->fileData.allocatedBlocksArray[i] = blockIndex;
        int lengthToCopy = (remainingLength > BLOCK_SIZE - 1) ? BLOCK_SIZE - 1 : remainingLength;

        strncpy(virtualDisk[blockIndex], currentPtr, lengthToCopy);
        virtualDisk[blockIndex][lengthToCopy] = '\0';
        currentPtr += lengthToCopy;
        remainingLength -= lengthToCopy;
    }

    file->fileData.noOfBlocks = blocksNeeded;
    file->fileData.sizeOfContent = dataLength;
    printf("Data written successfully (size = %d bytes).\n", file->fileData.sizeOfContent);
}

FileNode *findFile(char *name)
{
    if (currentDirectory == NULL || currentDirectory->type != DIRECTORY)
    {
        return NULL;
    }

    FileNode *head = currentDirectory->child;
    if (head == NULL)
    {
        return NULL;
    }
    FileNode *current = head;

    do
    {
        if (strcmp(current->name, name) == 0)
        {
            return current;
        }
        current = current->next;
    } while (current != head);
    return NULL;
}

void readOperation(char *fileName)
{
    FileNode *file = findFile(fileName);
    if (file == NULL || file->type != DATAFILE)
    {
        printf("Error. File named %s not found\n", fileName);
        return;
    }

    if (file->fileData.noOfBlocks == 0)
    {
        printf("(empty)\n");
        return;
    }

    for (int i = 0; i < file->fileData.noOfBlocks; i++)
    {
        int blockIndex = file->fileData.allocatedBlocksArray[i];
        printf("%s", virtualDisk[blockIndex]);
    }
    printf("\n");
}

int allocateBlock()
{
    if (headBlock == NULL)
    {
        return -1;
    }
    int blockIndex = headBlock->index;
    FreeBlockNode *temp = headBlock;
    headBlock = headBlock->next;
    if (headBlock != NULL)
    {
        headBlock->previous = NULL;
    }
    else
    {
        tailBlock = NULL;
    }
    totalUsedBlocks++;
    free(temp);
    return blockIndex;
}

void deallocateBlock(int freedIndex)
{
    FreeBlockNode *newNode = (FreeBlockNode *)malloc(sizeof(FreeBlockNode));
    if (newNode == NULL)
    {
        printf("Error. Failed to allocate memory for FreeBlock");
        return;
    }
    newNode->index = freedIndex;
    newNode->next = NULL;

    if (tailBlock == NULL)
    {
        headBlock = newNode;
        tailBlock = newNode;
        newNode->previous = NULL;
    }
    else
    {
        tailBlock->next = newNode;
        newNode->previous = tailBlock;
        tailBlock = newNode;
    }
    totalUsedBlocks--;
}

void deleteFile(char *fileName)
{
    FileNode *file = findFile(fileName);
    if (file == NULL || file->type != DATAFILE)
    {
        printf("Error. File named %s not found\n", fileName);
        return;
    }

    if (file->fileData.noOfBlocks > 0)
    {
        for (int i = 0; i < file->fileData.noOfBlocks; i++)
        {
            deallocateBlock(file->fileData.allocatedBlocksArray[i]);
        }
    }

    if (file->next == file) // single file in directory
    {
        currentDirectory->child = NULL;
    }
    else
    {
        FileNode *prevNode = file->previous;
        FileNode *nextNode = file->next;

        prevNode->next = nextNode;
        nextNode->previous = prevNode;
        if (currentDirectory->child == file) // change file to which cwd points if the pointed file deleted
        {
            currentDirectory->child = nextNode;
        }
    }

    free(file);
    printf("File deleted successfully.\n");
}

void deleteDirectory(char *directoryName)
{
    FileNode *directory = findFile(directoryName);
    if (directory == NULL || directory->type != DIRECTORY)
    {
        printf("Error. Directory named %s not found\n", directoryName);
        return;
    }

    if (directory->child != NULL)
    {
        printf("Error. Directory not empty\n");
        return;
    }

    if (directory->next == directory)
    {
        currentDirectory->child = NULL;
    }
    else
    {
        FileNode *prevNode = directory->previous;
        FileNode *nextNode = directory->next;

        prevNode->next = nextNode;
        nextNode->previous = prevNode;
        if (currentDirectory->child == directory)
        {
            currentDirectory->child = nextNode;
        }
    }

    free(directory);
    printf("Directory removed successfully.\n");
}

void printPath(FileNode *current)
{
    if (current == root)
    {
        printf("/");
        return;
    }

    printPath(current->parent);
    printf("%s/", current->name);
}

void listFiles()
{
    FileNode *head = currentDirectory->child;

    if (head == NULL)
    {
        printf("(empty)\n");
        return;
    }

    FileNode *current = head;
    do
    {
        if (current->type == DIRECTORY)
        {
            printf(" %s/\n", current->name);
        }
        else
        {
            printf(" %s\n", current->name);
        }
        current = current->next;
    } while (current != head);
}

void displayDiskUsage()
{
    int freeBlocks = NO_OF_BLOCKS - totalUsedBlocks;
    double usagePercent = (double)totalUsedBlocks / NO_OF_BLOCKS * 100.0;

    printf("Total Blocks: %d\n", NO_OF_BLOCKS);
    printf("Used Blocks: %d\n", totalUsedBlocks);
    printf("Free Blocks: %d\n", freeBlocks);
    printf("Disk Usage: %.2f%%\n", usagePercent);
}

void takeInput(char *line)
{
    if (line == NULL || strlen(line) == 0)
        return;

    char tempLine[2000]; // temporary buffer to safely copy and modify the input
    strncpy(tempLine, line, sizeof(tempLine) - 1);
    tempLine[sizeof(tempLine) - 1] = '\0';

    char *command = strtok(tempLine, " \n");
    char *arg1 = strtok(NULL, " \n");
    char *arg2 = NULL;

    if (command != NULL && strcmp(command, "write") == 0)
    {
        if (arg1 != NULL)
        {
            char *dataStart = strstr(line, arg1);
            if (dataStart != NULL)
            {
                dataStart += strlen(arg1) + 1;

                while (*dataStart != '\0' && (*dataStart == ' ' || *dataStart == '"'))
                {
                    dataStart++;
                }

                arg2 = dataStart;
                char *dataEnd = strrchr(arg2, '"');

                if (dataEnd != NULL)
                {
                    *dataEnd = '\0'; // Null-terminating the string right before the closing quote
                }
            }
        }
    }

    if (strcmp(command, "mkdir") == 0)
    {
        if (arg1)
            createDirectory(arg1);
    }
    else if (strcmp(command, "create") == 0)
    {
        if (arg1)
            createFile(arg1);
    }
    else if (strcmp(command, "cd") == 0)
    {
        if (arg1)
            changeDirectory(arg1);
    }
    else if (strcmp(command, "ls") == 0)
    {
        listFiles();
    }
    else if (strcmp(command, "pwd") == 0)
    {
        printPath(currentDirectory);
        printf("\n");
    }
    else if (strcmp(command, "df") == 0)
    {
        displayDiskUsage();
    }
    else if (strcmp(command, "rmdir") == 0)
    {
        if (arg1)
            deleteDirectory(arg1);
    }
    else if (strcmp(command, "delete") == 0)
    {
        if (arg1)
            deleteFile(arg1);
    }
    else if (strcmp(command, "write") == 0)
    {
        if (arg1 && arg2)
            writeOperation(arg1, arg2);
    }
    else if (strcmp(command, "read") == 0)
    {
        if (arg1)
            readOperation(arg1);
    }
    else if (strcmp(command, "exit") == 0)
    {
        exitProgram();
    }
    else
    {
        printf("Invalid command\n");
    }
}

void releaseMemoryFiles(FileNode *node)
{
    if (node == NULL)
        return;

    if (node->type == DIRECTORY && node->child != NULL)
    {
        FileNode *head = node->child;
        FileNode *current = head;

        do
        {
            FileNode *next_node = current->next;
            releaseMemoryFiles(current);
            current = next_node;
        } while (current != head);
    }
    free(node);
}

void exitProgram()
{
    FreeBlockNode *current = headBlock;
    FreeBlockNode *temp;

    while (current != NULL)
    {
        temp = current;
        current = current->next;
        free(temp);
    }
    headBlock = NULL;
    tailBlock = NULL;
    releaseMemoryFiles(root);
    root = NULL;
    currentDirectory = NULL;
    printf("Memory released. Exiting program...\n");
}

int main()
{
    initializeVFS();
    printf("Compact VFS - ready. Type 'exit' to quit.\n");
    char inputBuffer[2000];
    while (1)
    {
        if (currentDirectory == root)
        {
            printf("/> ");
        }
        else
        {
            printf("/%s > ", currentDirectory->name);
        }

        if (fgets(inputBuffer, sizeof(inputBuffer), stdin) == NULL)
        {
            break;
        }
        inputBuffer[strcspn(inputBuffer, "\n")] = '\0';
        if (strcmp(inputBuffer, "exit") == 0)
        {
            exitProgram();
            return 0;
        }

        takeInput(inputBuffer);
    }
    return 0;
}