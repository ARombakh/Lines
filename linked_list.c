#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "structs.h"

struct Stack* create_stack()
{
    struct Stack* new_stack = (struct Stack*) malloc(sizeof(struct Stack));
    if (!new_stack)
    {
        printf("Error: cannot allocate memory for stack\n");
        exit(EXIT_FAILURE);
    }
    new_stack->head = NULL;
    return new_stack;
}

void push(struct Stack* stack, char x, char y)
{
    struct Node *new_node = (struct Node*) malloc(sizeof(struct Node));
    if (!new_node)
    {
        printf("Error: cannot allocate memory for new node\n");
        exit(EXIT_FAILURE);
    }
    new_node->p.x = x;
    new_node->p.y = y;
    new_node->next = stack->head;
    stack->head = new_node;
}

bool is_empty(struct Stack* stack)
{
    return stack->head == NULL;
}

struct Point pop(struct Stack *stack)
{
    struct Node *upper = stack->head;
    if (upper == NULL)
    {
        printf("The stack is empty\n");
        exit(EXIT_FAILURE);
    }
    struct Point popped_value = stack->head->p;
    stack->head = upper->next;
    free(upper);

    return popped_value;
}

void empty_stack(struct Stack* stack)
{
    while (stack->head != NULL)
    {
        struct Node *temp = stack->head;
        stack->head = stack->head->next;
        free(temp);
    }
    free(stack);
}

bool search_stack(struct Stack* stack, char x, char y)
{    
    struct Node* pointer = stack->head;

    while (pointer)
    {
        if (pointer->p.x == x && pointer->p.y == y)
        {
            return true;
        }
        
        pointer = pointer->next;
    }

    return false;
}
/*
int main()
{
    bool is_in_stack;
    struct Stack* first = create_stack();
    // push(first, 1, 2);

    is_in_stack = search_stack(first, 1, 2);

    printf("The entry is in stack %d\n", is_in_stack);

    push(first, 1, 2);

    is_in_stack = search_stack(first, 1, 2);

    printf("The entry is in stack %d\n", is_in_stack);

    empty_stack(first);

    is_in_stack = search_stack(first, 1, 2);

    return 0;
}
*/