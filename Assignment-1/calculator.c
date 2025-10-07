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
int precedence(char op)
{
    if (op == '/' || op == '*')
        return 2;
    if (op == '+' || op == '-')
        return 1;
    return 0;
}

// Removing trailing and leading spaces
void trim(char *str)
{
    int startInx = 0;
    int endInx = strlen(str) - 1;

    while (endInx >= 0 && isspace((unsigned char)str[endInx]))
    {
        endInx--;
    }
    str[endInx + 1] = '\0';
    while (startInx <= endInx && isspace((unsigned char)str[startInx]))
    {
        startInx++;
    }

    if (startInx > 0)
    {
        int currInx;
        for (currInx = 0; currInx <= endInx - startInx; currInx++)
        {
            str[currInx] = str[startInx + currInx];
        }
        str[currInx] = '\0';
    }
}

bool calculate(char *expr)
{
    trim(expr); // needed for tokenization
    char *tokens[128];
    int tokenCount = -1;

    // strtok to get the first token
    char *token = strtok(expr, " ");

    while (token != NULL)
    {
        tokens[++tokenCount] = token; // Store token pointer
        token = strtok(NULL, " ");
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
        int integerVal;
        char charVal;
    } ElementData;
    typedef struct
    {
        ElementType type;
        ElementData data;
    } ArrayElement;

    ArrayElement outputStr[96];
    char operatorStack[32];
    int operatorInx = -1, outputInx = 0;
    for (int i = 0; i <= tokenCount; i++)
    {
        if (isOperator(tokens[i]))
        {
            char currOperator = *tokens[i];
            if (i == 0 || isOperator(tokens[i - 1]) || i == tokenCount || isOperator(tokens[i + 1]))
            {
                printf("Error: Invalid Expression");
                return 0;
            }
            while (operatorInx > -1 && precedence(operatorStack[operatorInx]) >= precedence(currOperator))
            {
                outputStr[outputInx].type = TYPE_CHAR;
                outputStr[outputInx++].data.charVal = operatorStack[operatorInx]; // storing first char of the operator string
                // pop operator from stack;
                operatorStack[operatorInx--] = '\0';
            }
            operatorStack[++operatorInx] = *tokens[i];
        }
        else if (isNumber(tokens[i])) // put number in output directly
        {
            if (i == 0 && tokenCount > 0 && !isOperator(tokens[i + 1]))
            {
                printf("Error: Invalid Expression");
                return 0;
            }
            else if (i == tokenCount && !isOperator(tokens[i - 1]))
            {
                printf("Error: Invalid Expression");
                return 0;
            }
            else if (!isOperator(tokens[i - 1]) || !isOperator(tokens[i + 1]))
            {
                printf("Error: Invalid Expression");
                return 0;
            }
            outputStr[outputInx].type = TYPE_INT;
            outputStr[outputInx++].data.integerVal = atoi(tokens[i]);
        }
        else
        {
            printf("Error: Invalid Expression");
            return 0;
        }
    }
    // Push remaining operators from operator stack to output
    while (operatorInx > -1)
    {
        outputStr[outputInx].type = TYPE_CHAR;
        outputStr[outputInx++].data.charVal = operatorStack[operatorInx--];
    }

    // final result
    int result[64], resultInx = -1;
    for (int i = 0; i < outputInx; i++)
    {
        if (outputStr[i].type == TYPE_INT)
        {
            result[++resultInx] = outputStr[i].data.integerVal;
        }
        else
        {
            int rightOperand = result[resultInx];
            result[resultInx--] = NULL;
            int leftOperand = result[resultInx];
            result[resultInx--] = NULL;
            if (outputStr[i].data.charVal == '+')
            {
                result[++resultInx] = leftOperand + rightOperand;
            }
            else if (outputStr[i].data.charVal == '-')
            {
                result[++resultInx] = leftOperand - rightOperand;
            }
            else if (outputStr[i].data.charVal == '*')
            {
                result[++resultInx] = leftOperand * rightOperand;
            }
            else if (outputStr[i].data.charVal == '/')
            {
                if (rightOperand == 0)
                {
                    printf("Error: Division by zero");
                    return 0;
                }
                result[++resultInx] = leftOperand / rightOperand;
            }
        }
    }
    printf("Result: %d", result[resultInx]);
    return 1;
}

int main()
{
    char inputStr[128];
    printf("Enter input string: ");
    scanf("%[^\n]s", inputStr);
    int ans = calculate(inputStr);
    if (!ans)
    {
        return 1;
    }
    return 0;
}