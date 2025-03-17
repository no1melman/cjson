#include "stack.h"
#include <stdlib.h>
#include <stdbool.h>

#define INITIAL_CAPACITY 16

static bool resize_stack(stack *the_stack);

stack *create_stack() {
    stack *the_stack = malloc(sizeof(stack));
    if (the_stack == NULL) {
        return NULL;
    }

    the_stack->capacity = INITIAL_CAPACITY;
    the_stack->size = 0;

    the_stack->stack = calloc(the_stack->capacity, sizeof(stack_item));
    if (the_stack->stack == NULL) {
        free(the_stack);
        return NULL;
    }

    return the_stack;
}

void push_stack(stack *the_stack, void *item) {
    if (the_stack->size == the_stack->capacity) {
        // resize the stack
        resize_stack(the_stack);
    }

    the_stack->stack[the_stack->size].value = item;
    the_stack->size++;
}

void *pop_stack(stack *the_stack) {
    void* item = the_stack->stack[the_stack->size - 1].value;
    the_stack->size--;
    return item;
}

void destroy_stack(stack *the_stack) {
    free(the_stack->stack);
    free(the_stack);
}

static bool resize_stack(stack *the_stack) {
    float multiplier = 2;
    // slow down the rate of expansion
    // unless you blow past 256 in which case doubling it
    // is probably more optimal then reallocating every 32 elements
    if (the_stack->size >= 128 && the_stack->size < 256) {
        multiplier = 1.25f; // at 128to 256 you still get 32 to 64 element increase
    } else if (the_stack->size < 128) {
        multiplier = 1.5f; // this results up to 63 element increase
        // so fairly aggressive expansion at the start, slow down to 256, then just
        // completely blast past later... seems like a decent profile.
    }
    size_t new_capacity = (size_t) ((float) the_stack->capacity * multiplier);
    if (the_stack->capacity < the_stack->size) {
        return false;
    }

    stack_item *new_stack_item = calloc(new_capacity, sizeof(stack_item));
    if (new_stack_item == NULL) {
        return false;
    }

    for (size_t i = 0; i < the_stack->size; i++) {
        stack_item item = the_stack->stack[i];
        if (item.value != NULL) {
            new_stack_item[i] = item;
        }
    }
    free(the_stack->stack);
    the_stack->stack = new_stack_item;
    the_stack->capacity = new_capacity;
    return true;
}
