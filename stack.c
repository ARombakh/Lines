#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "structs.h"

/*
struct Point
{
    char x;
    char y;
};

// Defining structure of stack node
typedef struct Node
{
    struct Point p;
    struct Node* next; 
} Node;
*/

// Global variable for top of the stack
Node* top = NULL;

// Initialization of empty stack
void init() {
    top = NULL;
}

// Check if stack is empty
int isEmpty()
{
    return top == NULL;
}

// Adding element into stack (push)
void push(char x_val, char y_val)
{
    // Creating new node
    Node* newNode = (Node*)malloc(sizeof(Node));
    if(newNode == NULL)
    {
        printf("Memory allocation error\n");
        exit(EXIT_FAILURE);
    }

    newNode->p.x = x_val;
    newNode->p.y = y_val;
    newNode->next = top;
    top = newNode;
}

struct Point pop()
{
    if (isEmpty())
    {
        printf("Stack is empty\n");
        // Returning {-1, -1} in case of error
        struct Point ret = {-1, -1};
        return ret;
    }

    Node* temp = top;
    struct Point poppedValues = temp->p;
    top = top->next;
    free(temp);
    return poppedValues;
}

// Getting value of upper stack element (peek)
struct Point peek()
{
    if (isEmpty())
    {
        printf("Stack is empty\n");
        // Returning {-1, -1} in case of error
        struct Point ret = {-1, -1};
        return ret;
    }

    return top->p;
};

void clearStack()
{
    while (!isEmpty())
    {
        pop();
    }
}

// Return pointer to the previous element of stack
bool isInStack(char x_val, char y_val)
{
    Node* temp = top;

    while (temp != NULL)
    {
        if (temp->p.x == x_val && temp->p.y == y_val)
        {
            return true;
        }
        temp = temp->next;
    }

    return false;
};

/*
// Testing stack
int main()
{
    init();

    bool instack;

    push(3, 5);
    push(8, 5);
    push(9, 0);
    push(4, 1);

    struct Point pnt = peek();
    
    printf("Upper element: %d, %d\n", pnt.x, pnt.y);

    pnt = pop();
    printf("Next upper element: %d, %d\n", pnt.x, pnt.y);

    pnt = pop();
    printf("Yet next upper element: %d, %d\n", pnt.x, pnt.y);

    instack = isInStack(8, 5);
    printf("8, 5 is in the stack is %d\n", instack);

    clearStack();

    return 0;
}
*/