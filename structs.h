#ifndef STRUCTS
#define STRUCTS

struct Point
{
    char x;
    char y;
};

// Defining structure of stack node
struct Node
{
    struct Node* next;
    struct Point p;
};

// Stack definition
struct Stack {
    struct Node* head;      // upper element of stack
};

#endif