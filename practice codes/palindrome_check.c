#include <stdio.h>

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
    int startIndex = 0, endIndex = size - 1;
    while (startIndex < endIndex)
    {
        if (array[startIndex] != array[endIndex])
        {
            printf("Not a palindrome");
            return 0;
        }
        startIndex++;
        endIndex--;
    }
    printf("Palindrome");
    return 0;
}