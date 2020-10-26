// Arup Guha
// 6/20/07
// Written in COP 3502 to illustrate an array implementation of a stack.
 //modified by Tanvir Ahmed 6/1/2020

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h> // has isDigit

#define SIZE 100
#define EMPTY -1

struct stack {

    char items[SIZE];

    int top;

};

void initialize(struct stack* stackPtr);
int full(struct stack* stackPtr);
int push(struct stack* stackPtr, char value);
int empty(struct stack* stackPtr);
char pop(struct stack* stackPtr);
int top(struct stack* stackPtr);
void display(struct stack* stackPtr);

int checkBalance(char stack[]);
char *infixToPostfix(char infix[]);

int isOperator(char ch);
int isParentheses(char ch);
int priority(char ch);

int main() {
    char exp[SIZE];
    char *postFix;
    int valid;

    printf("Enter Expression: \n");
    scanf("%[^\n]s", exp);
    printf("Your inputed expression is: %s", exp);
    valid = checkBalance(exp);
    // printf("\nValid: %d\n", valid);
    if(valid)
    {
        postFix = infixToPostfix(exp);
        printf("%s\n", postFix);
    }

}

char *infixToPostfix(char infix[])
{
    struct stack operators;
    initialize(&operators);

    int count = 0;
    char *postfix = (char*) malloc((strlen(infix + 1) * 2 * sizeof(char)));

    for(int i = 0; i < strlen(infix); i++)
    {
        if(isdigit(infix[i]))
        {
            while(isdigit(infix[i]))
            {
                postfix[count] = infix[i];
                i++;
                count++;
            }
            i--;
            postfix[count] = ' ';
            count++;
        }
        else if(isOperator(infix[i]))
        {
            if(empty(&operators))
            {
                push(&operators, infix[i]);
            }
            else if( priority(infix[i]) > priority(top(&operators)) ) 
            {
                push(&operators, infix[i]);
            }
            else
            {
                while(!empty(&operators) && priority(infix[i]) <= priority(top(&operators)) && !isParentheses(top(&operators)))
                {
                    postfix[count] = pop(&operators);
                    count++;
                    postfix[count] = ' ';
                    count++;
                }
                push(&operators, infix[i]);
            }      
        }
        else if(infix[i] == '(' || infix[i] == ')')
        {
            if(infix[i] == '(')
            {
                push(&operators, infix[i]);
            }
            else
            {
                while(top(&operators) != '(')
                {
                    postfix[count] = pop(&operators);
                    count++;
                    postfix[count] = ' ';
                    count++;
                }
                pop(&operators);
            }
        } 
    }
    while(!empty(&operators))
    {
        postfix[count] = pop(&operators);
        count++;
        postfix[count] = ' ';
        count++;
    }
    postfix[count] = '\0';

    return postfix;
}

int isOperator(char ch)
{
    if(ch == '+' || ch == '-' || ch == '/' || ch == '*' || ch == '^' || ch == '%')
        return 1;
    else
        return 0;
}

int isParentheses(char ch)
{
    if(ch == '{' || ch == '[' || ch == '(' || ch == ')' || ch == ']' || ch == '}')
        return 1;
    else
        return 0;
}

int priority(char ch)
{
    if(ch == '(' || ch == '{' || ch== '[')
        return 0;
    
    if(ch == '+' || ch == '-')
        return 1;
    
    if(ch == '*' || ch == '/' || ch == '%')
        return 2;
    
    if(ch == '^')
        return 3;

    printf("fail\n");
    return -1;
}

int checkBalance(char exp[])
{
    int valid = 1;
    struct stack mine;

    initialize(&mine);
    printf("\nChecking Balance:...\n");

    for(int i = 0; exp[i] != '\0'; i++)
    {
        if(exp[i] == '(' || exp[i] == '{' || exp[i] == '[')
        {
            push(&mine, exp[i]);
        }
        else if(exp[i] == ')')
        {
            char a  = pop(&mine);
            if(a != '(' || a == 'I')
            {
                valid = 0; 
                printf("Invalid for )!\n");
                return valid;
            }
        }
        else if(exp[i] == '}')
        {
            char a  = pop(&mine);
            if(a != '{' || a == 'I')
            {
                valid = 0; 
                printf("Invalid for }!\n");
                return valid;
            }
        }
        else if(exp[i] == ']')
        {
            char a  = pop(&mine);
            if(a != '[' || a == 'I')
            {
                valid = 0; 
                printf("Invalid for ]!\n");
                return valid;
            }
        }
    }

    if((pop(&mine) != 'I'))
    {
        valid = 0;
        printf("Invalid! You had extra open symbols!\n");
    }

    return valid;
}

void initialize(struct stack* stackPtr) {
     stackPtr->top = -1;
}

// If the push occurs, 1 is returned. If the
// stack is full and the push can't be done, 0 is
// returned.
int push(struct stack* stackPtr, char value) {

    // Check if the stack is full.
    if (full(stackPtr))
        return 0;

    // Add value to the top of the stack and adjust the value of the top.
    stackPtr->items[stackPtr->top+1] = value;
    (stackPtr->top)++;
    return 1;
}

// Returns true iff the stack pointed to by stackPtr is full.
int full(struct stack* stackPtr) {
    return (stackPtr->top == SIZE - 1);
}

// Returns true iff the stack pointed to by stackPtr is empty.
int empty(struct stack* stackPtr) {
    return (stackPtr->top == -1);
}

// Pre-condition: The stack pointed to by stackPtr is NOT empty.
// Post-condition: The value on the top of the stack is popped and returned.
// Note: If the stack pointed to by stackPtr is empty, -1 is returned.
char pop(struct stack* stackPtr) {

    char retval;

    // Check the case that the stack is empty.
    if (empty(stackPtr))
        return 'I';

    // Retrieve the item from the top of the stack, adjust the top and return
    // the item.
    retval = stackPtr->items[stackPtr->top];
    (stackPtr->top)--;
    return retval;
}

// Pre-condition: The stack pointed to by stackPtr is NOT empty.
// Post-condition: The value on the top of the stack is returned.
// Note: If the stack pointed to by stackPtr is empty, -1 is returned.
int top(struct stack* stackPtr) {

    // Take care of the empty case.
    if (empty(stackPtr))
        return EMPTY;

    // Return the desired item.
    return stackPtr->items[stackPtr->top];
}

void display(struct stack* stackPtr) {
    printf("\nPrinting the Current stack...");
    for(int i=0; i<=stackPtr->top; i++)
        printf("%d ", stackPtr->items[i]);
}
