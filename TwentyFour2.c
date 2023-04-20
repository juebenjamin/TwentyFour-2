/*-------------------------------------------
Program 3: TwentyFour Part Two
Course: CS 211, Spring 2023, UIC
System: Visual Studio Code on Mac
Author: Jeremiah Benjamin
------------------------------------------- */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

void selectRandomPuzzle(int difficulty, int *puzzle);
int getDifficulty();

typedef struct Node
{
    double value;
    struct Node *next;
} Node;

typedef struct Stack
{
    Node *top;
} Stack;

// Pushes a value onto the stack
void push(Stack *stack, double value)
{
    Node *newNode = (Node *)malloc(sizeof(Node));
    newNode->value = value;
    newNode->next = stack->top;
    stack->top = newNode;
}

// Pops and returns the top value from the stack
double pop(Stack *stack)
{
    double value = stack->top->value;
    Node *temp = stack->top;
    stack->top = stack->top->next;
    free(temp);
    return value;
}

// Returns true if the stack is empty, false otherwise
bool isEmpty(Stack *stack)
{
    return stack->top == NULL;
}

// Returns the value at the top of the stack without popping it
double peek(Stack *stack)
{
    return stack->top->value;
}

// Utility function to check if the character is an operator
bool isOperator(char c)
{
    return (c == '+' || c == '-' || c == '*' || c == '/');
}

// Utility function to check if the character is a digit
bool isDigit(char c)
{
    return (c >= '0' && c <= '9');
}

// Utility function to convert character digit to integer
int charToInt(char c)
{
    return c - '0';
}

// Utility function to calculate the precedence of the operators
int precedence(char op)
{
    if (op == '+' || op == '-')
    {
        return 1;
    }
    if (op == '*' || op == '/')
    {
        return 2;
    }
    return 0;
}

// Utility function to perform the operation and return the result
double performOperation(double a, double b, char op)
{
    switch (op)
    {
    case '+':
        printf("%.0lf + %.0lf = %.0lf.\n", a, b, a + b);
        return (double)(a + b);
    case '-':
        printf("%.0lf - %.0lf = %.0lf.\n", a, b, a - b);
        return (double)(a - b);
    case '*':
        printf("%.0lf * %.0lf = %.0lf.\n", a, b, a * b);
        return (double)(a * b);
    case '/':
        printf("%.0lf / %.0lf = %.0lf.\n", a, b, (double)((int)a / (int)b));
        return (double)((int)a / (int)b);
    }
    return 0;
}

// Function to evaluate the expression
bool evaluate(char *expression, int *numbersUsed, double *result)
{
    Stack numStack;
    numStack.top = NULL;
    Stack opStack;
    opStack.top = NULL;

    int openParensCount = 0;
    int closeParensCount = 0;

    for (int i = 0; expression[i] != '\0'; i++)
    {
        if (expression[i] == ' ')
        {
            continue;
        }
        // Check if the expression is a digit for computation
        if (isDigit(expression[i]))
        {
            int num = charToInt(expression[i]);
            push(&numStack, num);
            numbersUsed[num - 1]++;
        }
        // Check if the expression is a operator for computation
        else if (isOperator(expression[i]))
        {
            while (!isEmpty(&opStack) && precedence(peek(&opStack)) >= precedence(expression[i]))
            {
                char op = pop(&opStack);
                double b = pop(&numStack);
                double a = pop(&numStack);
                double result = performOperation(a, b, op);
                push(&numStack, result);
            }
            push(&opStack, expression[i]);
        }
        else if (expression[i] == '(')
        {
            openParensCount++;
            push(&opStack, expression[i]);
        }
        else if (expression[i] == ')')
        {
            // Too many closing parentheses, end and prompt user screen.
            if (isEmpty(&opStack))
            {
                printf("Error! Too many closing parentheses in the expression.\n\n");
                while (!isEmpty(&numStack))
                {
                    pop(&numStack);
                }
                return false;
            }
            closeParensCount++;
            while (!isEmpty(&opStack) && peek(&opStack) != '(')
            {
                char op = pop(&opStack);
                double b = pop(&numStack);
                double a = pop(&numStack);
                double result = performOperation(a, b, op);
                push(&numStack, result);
            }
            if (!isEmpty(&opStack))
            {
                pop(&opStack); // Pop the '('
            }
        }
    }

    if (openParensCount != closeParensCount)
    {
        // Too many closing parentheses, end and prompt user screen.
        printf("Error! Too many closing parentheses in the expression.\n\n");
        while (!isEmpty(&numStack))
        {
            pop(&numStack);
        }
        return false;
    }

    // Apply remaining operators in the stack
    while (!isEmpty(&opStack))
    {
        char op = pop(&opStack);
        double b = pop(&numStack);
        double a = pop(&numStack);
        double result = performOperation(a, b, op);
        push(&numStack, result);
    }

    *result = pop(&numStack);
    if (!isEmpty(&numStack))
    {
        // Too many values, end and prompt user screen.
        printf("Error! Too many values in the expression.\n\n");
        while (!isEmpty(&numStack))
        {
            pop(&numStack);
        }
        return false;
    }
    return true;
}

// Function to check if the expression that the user enters is valid
bool checkValidExpression(const char *expression, const int *puzzle)
{
    int numCount[10] = {0};
    int puzzleNumCount[10] = {0};

    for (int i = 0; i < 4; i++)
    {
        puzzleNumCount[puzzle[i]]++;
    }

    for (int i = 0; expression[i] != '\0'; i++)
    {
        char c = expression[i];
        if (c >= '1' && c <= '9')
        {
            numCount[c - '0']++;
        }
        else if (c != ' ' && c != '(' && c != ')' && c != '+' && c != '-' && c != '*' && c != '/')
        {
            printf("Error! Invalid symbol entered. Please try again.\n\n");
            return false;
        }
    }

    for (int i = 0; i < 10; i++)
    {
        if (numCount[i] != puzzleNumCount[i])
        {
            printf("Error! You must use all four numbers, and use each one only once. Please try again.\n\n");
            return false;
        }
    }
    return true;
}
// Function where game is played
void playGame(int difficulty)
{
    while (true)
    {
        // Make a puzzle
        int puzzle[4];
        selectRandomPuzzle(difficulty, puzzle);
        printf("The numbers to use are: %d, %d, %d, %d.\n", puzzle[0], puzzle[1], puzzle[2], puzzle[3]);

        char expression[256];
        printf("Enter your solution: ");
        scanf(" %[^\n]", expression);
        // Check if the expression is valid before computation
        if (!checkValidExpression(expression, puzzle))
        {
            continue;
        }
        // Store the numbers used, and create variable for storing result of computation to check if sum to 24 or not
        int numbersUsed[10] = {0};
        double result;
        bool evalSuccess = evaluate(expression, numbersUsed, &result);
        if (result == 24 && evalSuccess)
        {
            printf("Well done! You are a math genius.\n\n");
        }
        if (result != 24 && evalSuccess)
        {
            printf("Sorry. Your solution did not evaluate to 24.\n\n");
        }
        // Prompt user with options for playig again or exiting
        printf("Enter: \t1 to play again,\n");
        printf("\t2 to change the difficulty level and then play again, or\n");
        printf("\t3 to exit the program.\n");
        printf("Your choice --> ");
        int choice;
        scanf("%d", &choice);

        if (choice == 2)
        {
            difficulty = getDifficulty();
        }
        else if (choice == 3)
        {
            printf("\nThanks for playing!\n");
            printf("Exiting...\n");
            break;
        }
    }
}
// Function to get diffculty level to use
int getDifficulty()
{
    printf("Choose your difficulty level: E for easy, M for medium, and H for hard (default is easy).\n");
    printf("Your choice --> ");
    char level;
    scanf(" %c", &level);
    int difficulty;
    switch (level)
    {
    case 'E':
        difficulty = 1;
        break;
    case 'M':
        difficulty = 2;
        break;
    case 'H':
        difficulty = 3;
        break;
    default:
        difficulty = 1;
    }
    return difficulty;
}
void selectRandomPuzzle(int difficulty, int *puzzle)
{
    // Determine the filename based on the difficulty level
    const char *filename;
    if (difficulty == 1)
    {
        filename = "easy.txt";
    }
    else if (difficulty == 2)
    {
        filename = "medium.txt";
    }
    else
    {
        filename = "hard.txt";
    }

    // Open the file for reading
    FILE *fp = fopen(filename, "r");
    if (fp == NULL)
    {
        printf("Error: Failed to open the puzzle file.\n");
        exit(1);
    }

    // Count the number of puzzles in the file
    int numPuzzles = 0;
    char line[256];
    while (fgets(line, sizeof(line), fp) != NULL)
    {
        numPuzzles++;
    }

    // Generate a random index to select a puzzle
    int index = rand() % numPuzzles;

    // Reset the file pointer to the beginning of the file
    fseek(fp, 0, SEEK_SET);

    // Read the selected puzzle from the file
    int i = 0;
    while (fgets(line, sizeof(line), fp) != NULL)
    {
        if (i == index)
        {
            // Parse the puzzle from the line
            int num1, num2, num3, num4;
            if (sscanf(line, "%d %d %d %d", &num1, &num2, &num3, &num4) != 4)
            {
                printf("Error: Failed to parse the puzzle from the file.\n");
                exit(1);
            }
            puzzle[0] = num1;
            puzzle[1] = num2;
            puzzle[2] = num3;
            puzzle[3] = num4;
            break;
        }
        i++;
    }

    // Close the file
    fclose(fp);
}

// Main function where the game is ran and where program starts
int main()
{
    srand(1);
    printf("Welcome to the game of TwentyFour Part Two!\n");
    printf("Use each of the four numbers shown exactly once, \n");
    printf("combining them somehow with the basic mathematical operators (+,-,*,/) \n");
    printf("to yield the value twenty-four.\n");
    int difficulty = getDifficulty();
    playGame(difficulty);
    return 0;
}