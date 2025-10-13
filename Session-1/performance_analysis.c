#include <stdio.h>
#include <stdlib.h>

typedef struct
{
    int rollNo;
    char name[50];
    int marksOfSubject1;
    int marksOfSubject2;
    int marksOfSubject3;
} StudentDetails;

int calculateTotal(StudentDetails);
char calculateGrade(float);
void printPerformance(float);
void printRollNo(const StudentDetails* , int, int);
int compareRollNo(const void*, const void*);

int main()
{
    int numberOfStudents;
    printf("\nEnter the input in following format\n\n");
    printf("First line: An integer N (number of students)\nNext N lines: (Each line contains the following data separated by spaces)\nRoll_Number Name Marks1 Marks2 Marks3\n\n");
    scanf("%d", &numberOfStudents);

    StudentDetails *studentArray = (StudentDetails *)malloc(numberOfStudents * sizeof(StudentDetails));
    for (int i = 0; i < numberOfStudents; i++)
    {
        // storing number of items successfully matched and assigned (must be 5)
        int match = scanf("%d %49s %d %d %d", &studentArray[i].rollNo, studentArray[i].name, &studentArray[i].marksOfSubject1, &studentArray[i].marksOfSubject2, &studentArray[i].marksOfSubject3);
        if (match != 5)
        {
            fprintf(stderr, "\nERROR: Invalid input format detected for Student %d.\n", i + 1);
            return 1;
        }
    }

    for (int i = 0; i < numberOfStudents; i++)
    {
        int total = calculateTotal(studentArray[i]);
        float average = (float)total / 3;
        char grade = calculateGrade(average);
        printf("\nRoll: %d\nName: %s\nTotal: %d\nAverage: %.2f\nGrade: %c\n", studentArray[i].rollNo, studentArray[i].name, total, average, grade);
        if (average < 35)
        {
            continue;
        }
        printf("Performance: ");
        printPerformance(average);
    }

    // sorting array by roll number
    qsort(studentArray, numberOfStudents, sizeof(StudentDetails), compareRollNo);

    printf("List of Roll Numbers(via recursion): ");
    printRollNo(studentArray, numberOfStudents, 0);

    studentArray = NULL;
    free(studentArray);
    return 0;
}

int calculateTotal(StudentDetails student)
{
    return student.marksOfSubject1 + student.marksOfSubject2 + student.marksOfSubject3;
}

char calculateGrade(float average)
{
    if (average >= 85)
    {
        return 'A';
    }
    else if (average >= 70)
    {
        return 'B';
    }
    else if (average >= 50)
    {
        return 'C';
    }
    else if (average >= 35)
    {
        return 'D';
    }
    else
    {
        return 'F';
    }
}

void printPerformance(float average)
{
    if (average >= 85)
    {
        printf("*****\n");
    }
    else if (average >= 70)
    {
        printf("****\n");
    }
    else if (average >= 50)
    {
        printf("***\n");
    }
    else
    {
        printf("**\n");
    }
}

void printRollNo(const StudentDetails *listOfStudents, int numberOfStudents, int index)
{
    // print \n after all roll numbers
    if (index >= numberOfStudents)
    {
        printf("\n");
        return;
    }
    printf("%d", listOfStudents[index].rollNo);

    // print space if not on last index
    if (index < numberOfStudents - 1)
    {
        printf(" ");
    }
    printRollNo(listOfStudents, numberOfStudents, index + 1);
}

int compareRollNo(const void *a, const void *b)
{
    StudentDetails *studentA = (StudentDetails *)a;
    StudentDetails *studentB = (StudentDetails *)b;

    // positive value if studentA->rollNo > studentB->rollNo
    return studentA->rollNo - studentB->rollNo;
}