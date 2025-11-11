#include <stdio.h>

int removeDublicate(int array[], int size)
{
    if (size == 1)
        return size;
    int *currentElement = array, *uniqueArrayEnd = array;
    for (int i = 0; i < size; i++, currentElement++)
    {
        int *checkElement = array;
        int isDublicate = 0;
        while (checkElement < uniqueArrayEnd)
        {
            if (*currentElement == *checkElement)
            {
                isDublicate = 1;
                break;
            }
            checkElement++;
        }
        if (!isDublicate)
        {
            *uniqueArrayEnd = *currentElement;
            uniqueArrayEnd++;
        }
    }
    return (int)(uniqueArrayEnd - array);
}

int main()
{
    int size;
    printf("Enter size of array: ");
    if (scanf("%d", &size) != 1 || size <= 0)
    {
        printf("Invalid size input");
        return 1;
    }

    int array[size];
    printf("Enter array elements:\n");
    for (int i = 0; i < size; i++)
    {
        if (scanf("%d", array + i) != 1)
        {
            printf("Invalid element input. Enter integer.");
            return 1;
        }
    }

    int newSize = removeDublicate(array, size);
    printf("New array is: ");
    for (int i = 0; i < newSize; i++)
    {
        printf("%d ", array[i]);
    }
    printf("\nNew size is %d", newSize);
    return 0;
}