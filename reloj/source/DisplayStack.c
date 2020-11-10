#include "DisplayStack.h"

void stack_init(DisplayStack * stack, OptionFunction * function_buffer)
{
    stack->capacity = STACK_MAX_CAPACITY;
    stack->top = -1;
    stack->functions = function_buffer;
}
  
// DisplayStack is full when top is equal to the last index 
bool stack_is_full(DisplayStack * stack) 
{ 
    return stack->top == stack->capacity - 1; 
}

// DisplayStack is empty when top is equal to -1 
bool stack_is_empty(DisplayStack * stack) 
{ 
    return stack->top == -1; 
} 
  
void stack_push(DisplayStack * stack, OptionFunction * item) 
{ 
    if (stack_is_full(stack)) 
        return; 

    stack->functions[++stack->top] = *item; 
} 
  
OptionFunction * stack_pop(DisplayStack* stack) 
{ 
    if (stack_is_empty(stack)) 
        return NULL;

    return &stack->functions[stack->top--]; 
} 
  
OptionFunction * stack_peek(DisplayStack* stack) 
{ 
    if (stack_is_empty(stack)) 
        return NULL; 

    return &stack->functions[stack->top]; 
} 
