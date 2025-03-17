#ifndef STACK_H
#define STACK_H

#include <stdlib.h>

typedef struct {
    void* value;
} stack_item;
typedef struct {
    stack_item* stack;
    size_t size;
    size_t capacity;
} stack;

stack *create_stack();
void push_stack(stack *the_stack, void *item);
void* pop_stack(stack *the_stack);
void destroy_stack(stack *the_stack);

#endif //STACK_H
