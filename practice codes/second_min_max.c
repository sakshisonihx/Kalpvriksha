#include <stdio.h>
#include <limits.h>

void insertionSort(int array[], int size)
{
    if (size == 1)
        return;

    for (int currentIndex = 1; currentIndex < size; currentIndex++)
    {
        int sortedIndex = currentIndex - 1;
        int element = array[currentIndex];
        while (sortedIndex > -1 && array[sortedIndex] > element)
        {
            array[sortedIndex + 1] = array[sortedIndex];
            sortedIndex--;
        }
        array[sortedIndex + 1] = element;
    }
}

int main(int argc, char const *argv[])
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

    insertionSort(array, size);

    int maxElement = array[size - 1], minElement = array[0];
    if (maxElement == minElement)
    {
        printf("Second smallest element is %d\n", minElement);
        printf("Second largest element is %d", maxElement);
        return 0;
    }

    int startIndex = 0, endIndex = size - 1;
    while (startIndex < size)
    {
        if (array[startIndex] > minElement)
        {
            printf("Second smallest element is %d\n", array[startIndex]);
            break;
        }
        startIndex++;
    }
    while (endIndex > -1)
    {
        if (array[endIndex] < maxElement)
        {
            printf("Second largest element is %d", array[endIndex]);
            break;
        }
        endIndex--;
    }

    return 0;
}