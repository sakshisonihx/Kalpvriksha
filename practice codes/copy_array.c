#include <stdio.h>

int main()
{
    int size;
    printf("Enter size of array A: ");
    if (scanf("%d", &size) != 1 || size <= 0)
    {
        printf("Invalid size input");
        return 1;
    }

    int arrayA[size], arrayB[size];
    printf("Enter array elements:\n");
    for (int i = 0; i < size; i++)
    {
        if (scanf("%d", arrayA + i) != 1)
        {
            printf("Invalid element input. Enter integer.");
            return 1;
        }
    }

    printf("Array A:\n");
    for (int i = 0; i < size; i++)
    {
        printf("%d ", *(arrayA + i));
    }

    for (int i = 0; i < size; i++)
    {
        *(arrayB + i) = *(arrayA + i);
    }

    printf("\nArray B after copying values from A:\n");
    for (int i = 0; i < size; i++)
    {
        printf("%d ", *(arrayB + i));
    }
    return 0;
}