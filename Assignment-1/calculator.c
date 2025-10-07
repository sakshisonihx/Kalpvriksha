#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

int is_operator(const char *token)
{
    if (token == NULL || strlen(token) != 1)
    {
        return 0;
    }
    const char operators[] = "+-*/";
    return strchr(operators, *token) != NULL;
}

int is_number(const char *token)
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
    int start = 0;
    int end = strlen(str) - 1;

    while (end >= 0 && isspace((unsigned char)str[end]))
    {
        end--;
    }
    str[end + 1] = '\0';
    while (start <= end && isspace((unsigned char)str[start]))
    {
        start++;
    }

    if (start > 0)
    {
        int i;
        for (i = 0; i <= end - start; i++)
        {
            str[i] = str[start + i];
        }
        str[i] = '\0';
    }
}

bool calculate(char *expr)
{
    trim(expr);
    char *tokens[128];
    int token_count = -1;

    // strtok to get the first token
    char *token = strtok(expr, " ");

    while (token != NULL)
    {
        tokens[++token_count] = token; // Store token pointer
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
        int integer_val;
        char char_val;
    } ElementData;
    typedef struct
    {
        ElementType type;
        ElementData data;
    } ArrayElement;

    ArrayElement outputstr[96];
    char operatorstack[32];
    int oper_inx = -1, output_inx = 0;
    for (int i = 0; i <= token_count; i++)
    {
        if (is_operator(tokens[i]))
        {
            char curr_op = *tokens[i];
            if (i == 0 || is_operator(tokens[i - 1]) || i == token_count || is_operator(tokens[i + 1]))
            {
                printf("Error: Invalid Expression");
                return 0;
            }
            while (oper_inx > -1 && precedence(operatorstack[oper_inx]) >= precedence(curr_op))
            {
                outputstr[output_inx].type = TYPE_CHAR;
                outputstr[output_inx++].data.char_val = operatorstack[oper_inx]; // storing first char of the operator string
                // pop operator from stack;
                operatorstack[oper_inx--] = '\0';
            }
            operatorstack[++oper_inx] = *tokens[i];
        }
        else if (is_number(tokens[i])) // put number in output directly
        {
            if (i == 0 && token_count > 0 && !is_operator(tokens[i + 1]))
            {
                printf("Error: Invalid Expression");
                return 0;
            }
            else if (i == token_count && !is_operator(tokens[i - 1]))
            {
                printf("Error: Invalid Expression");
                return 0;
            }
            else if (!is_operator(tokens[i - 1]) || !is_operator(tokens[i + 1]))
            {
                printf("Error: Invalid Expression");
                return 0;
            }
            outputstr[output_inx].type = TYPE_INT;
            outputstr[output_inx++].data.integer_val = atoi(tokens[i]);
        }
        else
        {
            printf("Error: Invalid Expression");
            return 0;
        }
        // printf("Token: %s\n", tokens[i]);
    }
    // Push remaining operators from operator stack to output
    while (oper_inx > -1)
    {
        outputstr[output_inx].type = TYPE_CHAR;
        outputstr[output_inx++].data.char_val = operatorstack[oper_inx--];
    }

    // final result
    int result[64], result_inx = -1;
    for (int i = 0; i < output_inx; i++)
    {
        if (outputstr[i].type == TYPE_INT)
        {
            result[++result_inx] = outputstr[i].data.integer_val;
        }
        else
        {
            int right = result[result_inx];
            result[result_inx--] = NULL;
            int left = result[result_inx];
            result[result_inx--] = NULL;
            if (outputstr[i].data.char_val == '+')
            {
                result[++result_inx] = left + right;
            }
            else if (outputstr[i].data.char_val == '-')
            {
                result[++result_inx] = left - right;
            }
            else if (outputstr[i].data.char_val == '*')
            {
                result[++result_inx] = left * right;
            }
            else if (outputstr[i].data.char_val == '/')
            {
                if (right == 0)
                {
                    printf("Error: Division by zero");
                    return 0;
                }
                result[++result_inx] = left / right;
            }
        }
    }
    printf("Result: %d", result[result_inx]);
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