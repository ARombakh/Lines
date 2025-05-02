#ifndef STACK_FUNCTIONS
#define STACK_FUNCTIONS

#include <stdbool.h>

struct Stack* create_stack();
void push(struct Stack* stack, char x, char y);
bool is_empty(struct Stack* stack);
struct Point pop(struct Stack *stack);
void empty_stack(struct Stack* stack);
bool search_stack(struct Stack* stack, char x, char y);

#endif