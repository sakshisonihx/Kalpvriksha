#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void applySmoothingFilter(int **, int);
void printMatrix(int **, int);
void rotateMatrix(int **, int);

int main()
{
    int matrixSize;
    printf("Enter matrix size (2-10): ");
    int inputCheck = scanf("%d", &matrixSize);
    if (inputCheck != 1)
    {
        printf("Invalid size input");
        return 1;
    }

    int **sonarIntensity = (int **)malloc(matrixSize * sizeof(int *));
    for (int i = 0; i < matrixSize; i++)
    {
        sonarIntensity[i] = (int *)malloc(matrixSize * sizeof(int));
    }
    // updating seed for rand() so that a random matrix is generated everytime
    srand(time(NULL));
    for (int rowIndex = 0; rowIndex < matrixSize; rowIndex++)
    {
        for (int columnIndex = 0; columnIndex < matrixSize; columnIndex++)
        {
            *(*(sonarIntensity + rowIndex) + columnIndex) = rand() % 256;
        }
    }

    printf("Original randomly generated matrix:\n");
    printMatrix(sonarIntensity, matrixSize);

    rotateMatrix(sonarIntensity, matrixSize);

    printf("\nMatrix after 90 degrees Clockwise Rotation:\n");
    printMatrix(sonarIntensity, matrixSize);

    applySmoothingFilter(sonarIntensity, matrixSize);

    printf("\nMatrix after Applying 3x3 Smoothing Filter:\n");
    printMatrix(sonarIntensity, matrixSize);

    for (int i = 0; i < matrixSize; i++)
    {
        free(sonarIntensity[i]);
    }
    free(sonarIntensity);
    return 0;
}

void printMatrix(int **matrix, int size)
{
    for (int rowIndex = 0; rowIndex < size; rowIndex++)
    {
        for (int columnIndex = 0; columnIndex < size; columnIndex++)
        {
            printf("%d ", *(*(matrix + rowIndex) + columnIndex));
        }
        printf("\n");
    }
}

void rotateMatrix(int **matrix, int size)
{
    // transposing matrix
    for (int rowIndex = 0; rowIndex < size; rowIndex++)
    {
        for (int columnIndex = rowIndex + 1; columnIndex < size; columnIndex++)
        {
            int temp = *(*(matrix + rowIndex) + columnIndex);
            *(*(matrix + rowIndex) + columnIndex) = *(*(matrix + columnIndex) + rowIndex);
            *(*(matrix + columnIndex) + rowIndex) = temp;
        }
    }
    // reversing every row of matrix
    for (int columnIndex = 0; columnIndex < size / 2; columnIndex++)
    {
        for (int rowIndex = 0; rowIndex < size; rowIndex++)
        {
            int temp = *(*(matrix + rowIndex) + columnIndex);
            *(*(matrix + rowIndex) + columnIndex) = *(*(matrix + rowIndex) + size - columnIndex - 1);
            *(*(matrix + rowIndex) + size - columnIndex - 1) = temp;
        }
    }
}

void applySmoothingFilter(int **matrix, int size)
{
    // array to store smoothed values of last 2 rows from the current row
    int *previousTwoRows = (int *)malloc(sizeof(int) * size * 2);
    for (int rowIndex = 0; rowIndex < size; rowIndex++)
    {
        for (int columnIndex = 0; columnIndex < size; columnIndex++)
        {
            int rowStart = (rowIndex == 0) ? 0 : -1;
            int columnStart = (columnIndex == 0) ? 0 : -1;
            int rowEnd = (rowIndex == size - 1) ? 1 : 2;
            int columnEnd = (columnIndex == size - 1) ? 1 : 2;
            int sum = 0, count = 0;

            for (int currentRow = rowStart; currentRow < rowEnd; currentRow++)
            {
                for (int currentColumn = columnStart; currentColumn < columnEnd; currentColumn++)
                {
                    sum += *(*(matrix + rowIndex + currentRow) + columnIndex + currentColumn);
                    count++;
                }
            }
            int smoothedValue = sum / count;
            // replacing the matrix value of row with smoothed value. If we are on 3rd row for example, we
            // don't need 1st row to calculate smoothed value. Thus we can safely replace matrix value of
            // 1st row with its smoothed value that was earlier stored in previousTwoRows array.
            if (rowIndex > 1)
            {
                *(*(matrix + rowIndex - 2) + columnIndex) = previousTwoRows[(rowIndex % 2) * size + columnIndex];
            }
            // storing smoothed value of current row
            previousTwoRows[(rowIndex % 2) * size + columnIndex] = smoothedValue;
        }
    }
    // replacing matrix value of last two rows with their smoothed value
    for (int rowIndex = size - 2; rowIndex < size; rowIndex++)
    {
        for (int columnIndex = 0; columnIndex < size; columnIndex++)
        {
            *(*(matrix + rowIndex) + columnIndex) = previousTwoRows[(rowIndex % 2) * size + columnIndex];
        }
    }
    free(previousTwoRows);
}