#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

int isOperator(const char *token)
{
    if (token == NULL || strlen(token) != 1)
    {
        return 0;
    }
    const char operators[] = "+-*/";
    return strchr(operators, *token) != NULL;
}

int isNumber(const char *token)
{
    if (token == NULL || *token == '\0')
    {
        return 0;
    }
    if (*token == '-')
        token++;
    // Must have at least one digit after '-'
    if (!isdigit((unsigned char)*token))
        return 0;
    while (*token)
    {
        if (!isdigit((unsigned char)*token))
        {
            return 0;
        }
        token++;
    }
    return 1;
}

// higher number means higher precedence
int precedence(char operator)
{
    if (operator == '/' || operator == '*')
        return 2;
    if (operator == '+' || operator == '-')
        return 1;
    return 0;
}

// Removing trailing and leading spaces
void trim(char *inputString)
{
    int startIndex = 0;
    int endIndex = strlen(inputString) - 1;

    while (endIndex >= 0 && isspace((unsigned char)inputString[endIndex]))
    {
        endIndex--;
    }
    inputString[endIndex + 1] = '\0';
    while (startIndex <= endIndex && isspace((unsigned char)inputString[startIndex]))
    {
        startIndex++;
    }

    if (startIndex > 0)
    {
        int currentIndex;
        for (currentIndex = 0; currentIndex <= endIndex - startIndex; currentIndex++)
        {
            inputString[currentIndex] = inputString[startIndex + currentIndex];
        }
        inputString[currentIndex] = '\0';
    }
}

bool calculate(char *expression)
{
    trim(expression); // needed for tokenization
    char *tokens[128];
    int tokenCount = -1;

    // strtok to get the first token
    char *token = strtok(expression, " ");

    while (token != NULL)
    {
        tokens[++tokenCount] = token; // Store token pointer
        token = strtok(NULL, " ");
    }

    if (tokenCount < 0)
    {
        printf("Error: Invalid Expression (empty input)");
        return false;
    }

    // Postfix expression making

    // Using struct + union to store multiple datatypes in output array
    typedef enum
    {
        TYPE_INT,
        TYPE_CHAR
    } ElementType;
    typedef union
    {
        int integerValue;
        char charValue;
    } ElementData;
    typedef struct
    {
        ElementType type;
        ElementData data;
    } ArrayElement;

    ArrayElement outputString[96];
    char operatorStack[32];
    int operatorTop = -1, outputIndex = 0;
    for (int i = 0; i <= tokenCount; i++)
    {
        if (isOperator(tokens[i]))
        {
            char currentOperator = *tokens[i];
            if (i == 0 || isOperator(tokens[i - 1]) || i == tokenCount || isOperator(tokens[i + 1]))
            {
                printf("Error: Invalid Expression (unexpected or consecutive operator)");
                return false;
            }
            while (operatorTop > -1 && precedence(operatorStack[operatorTop]) >= precedence(currentOperator))
            {
                outputString[outputIndex].type = TYPE_CHAR;
                outputString[outputIndex++].data.charValue = operatorStack[operatorTop]; // storing first char of the operator string
                operatorStack[operatorTop--] = '\0';
            }
            operatorStack[++operatorTop] = *tokens[i];
        }
        else if (isNumber(tokens[i])) // put number in output directly
        {
            if ((i == 0 && isNumber(tokens[i + 1])) ||
                (i == tokenCount && isNumber(tokens[i - 1])) ||
                (i > 0 && isNumber(tokens[i - 1])) ||
                (i < tokenCount && isNumber(tokens[i + 1])))
            {
                printf("Error: Invalid Expression (consecutive numbers)");
                return false;
            }
            outputString[outputIndex].type = TYPE_INT;
            outputString[outputIndex++].data.integerValue = atoi(tokens[i]);
        }
        else
        {
            printf("Error: Invalid Expression (wrong input format or string)");
            return false;
        }
    }
    // Push remaining operators from operator stack to output
    while (operatorTop > -1)
    {
        outputString[outputIndex].type = TYPE_CHAR;
        outputString[outputIndex++].data.charValue = operatorStack[operatorTop--];
    }

    // final result
    int result[64], resultIndex = -1;
    for (int i = 0; i < outputIndex; i++)
    {
        if (outputString[i].type == TYPE_INT)
        {
            result[++resultIndex] = outputString[i].data.integerValue;
        }
        else
        {
            int rightOperand = result[resultIndex];
            result[resultIndex--] = NULL;
            int leftOperand = result[resultIndex];
            result[resultIndex--] = NULL;
            if (outputString[i].data.charValue == '+')
            {
                result[++resultIndex] = leftOperand + rightOperand;
            }
            else if (outputString[i].data.charValue == '-')
            {
                result[++resultIndex] = leftOperand - rightOperand;
            }
            else if (outputString[i].data.charValue == '*')
            {
                result[++resultIndex] = leftOperand * rightOperand;
            }
            else if (outputString[i].data.charValue == '/')
            {
                if (rightOperand == 0)
                {
                    printf("Error: Division by zero");
                    return false;
                }
                result[++resultIndex] = leftOperand / rightOperand;
            }
        }
    }
    printf("Result: %d", result[resultIndex]);
    return 1;
}

int main()
{
    char inputString[128];
    printf("Enter input string: ");
    scanf("%[^\n]s", inputString);
    int answer = calculate(inputString);
    if (!answer)
    {
        return 1;
    }
    return 0;
}