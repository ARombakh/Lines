#ifndef STACK_FUNCTIONS_2
#define STACK_FUNCTIONS_2

#include <stdbool.h>

void init_2();
int isEmpty_2();
void push_2(char x_val, char y_val);
struct Point pop_2();
struct Point peek_2();
void clearStack_2();
bool isInStack_2(char x_val, char y_val);

#endif