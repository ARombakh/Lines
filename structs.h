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
    struct Point p;
    struct Node* next; 
};

// Stack definition
struct Stack {
    struct Node* head;      // upper element of stack
};

#endif