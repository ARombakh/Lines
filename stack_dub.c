#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "structs.h"

// Global variable for top of the stack
struct Node* top_2 = NULL;

// Initialization of empty stack
void init_2() {
    top_2 = NULL;
}

// Check if stack is empty
int isEmpty_2()
{
    return top_2 == NULL;
}

// Adding element into stack (push)
void push_2(char x_val, char y_val)
{
    // Creating new node
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    if(newNode == NULL)
    {
        printf("Memory allocation error\n");
        exit(EXIT_FAILURE);
    }

    newNode->p.x = x_val;
    newNode->p.y = y_val;
    newNode->next = top_2;
    top_2 = newNode;
}

struct Point pop_2()
{
    if (isEmpty_2())
    {
        printf("Stack is empty\n");
        // Returning {-1, -1} in case of error
        struct Point ret = {-1, -1};
        return ret;
    }

    struct Node* temp = top_2;
    struct Point poppedValues = temp->p;
    top_2 = top_2->next;
    free(temp);
    return poppedValues;
}

// Getting value of upper stack element (peek)
struct Point peek_2()
{
    if (isEmpty_2())
    {
        printf("Stack is empty\n");
        // Returning {-1, -1} in case of error
        struct Point ret = {-1, -1};
        return ret;
    }

    return top_2->p;
};

void clearStack_2()
{
    while (!isEmpty_2())
    {
        pop_2();
    }
}

// Return pointer to the previous element of stack
bool isInStack_2(char x_val, char y_val)
{
    struct Node* temp = top_2;

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
    init_2();

    bool instack;

    push_2(3, 5);
    push_2(8, 5);
    push_2(9, 0);
    push_2(4, 1);

    struct Point pnt = peek_2();
    
    printf("Upper element: %d, %d\n", pnt.x, pnt.y);

    pnt = pop_2();
    printf("Next upper element: %d, %d\n", pnt.x, pnt.y);

    pnt = pop_2();
    printf("Yet next upper element: %d, %d\n", pnt.x, pnt.y);

    instack = isInStack_2(8, 5);
    printf("8, 5 is in the stack is %d\n", instack);

    clearStack_2();

    return 0;
}
    */