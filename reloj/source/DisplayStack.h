#ifndef DISPLAY_STACK_H
#define DISPLAY_STACK_H

#include <tonc.h>

#include "OptionsChooser.h"
#define STACK_MAX_CAPACITY 5


/**
 * DisplayStack that a DisplayController can use to manage a hierarchy of contents
 */
typedef struct _DisplayStack {
    u8 capacity; /// Max capacity of the stack
    s8 top; /// Index
    OptionFunction * functions; /// Array with the function to call
} DisplayStack;

void stack_init(DisplayStack * stack, OptionFunction * function_buffer);

/**
 * Checks if a stack is full
 * @param stack The stack to check
 * @return true if the top equals the capacity
 */
bool stack_is_full(DisplayStack * stack);

/**
 * Checks if a stack is full
 * @param stack The stack to check
 * @return true if the top equals -1
 */
bool stack_is_empty(DisplayStack * stack); 

/**
 * Adds a function to the stack
 * @param stack The target stack
 * @param item The function that is going to be added
 */
void stack_push(DisplayStack * stack, OptionFunction * item);

/**
 * Removes the top item from the stack
 * @param stack The target stack
 * @return The removed function
 */
OptionFunction * stack_pop(DisplayStack* stack);

/**
 * Returns the top from stack without removing it 
 * @param stack The stack to peek
 * @return The function at the top of the stack
 */
OptionFunction * stack_peek(DisplayStack* stack);

#endif