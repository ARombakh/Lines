#ifndef STRUCTS
#define STRUCTS

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

#endif