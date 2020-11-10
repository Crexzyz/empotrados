#ifndef OPTIONS_CHOOSER_H
#define OPTIONS_CHOOSER_H

#include <tonc.h>

#define OPTS_MAX_SIZE 3

/**
 * Text for an option with its size 
 */
typedef struct _OptionText {
    char * text;
    u8 size; 
} OptionText;

/**
 * Initializes an Option text
 * @param ot The uninitialized struct
 * @param text A string that contains the text of the option
 * @param size The size of the text (commonly strlen(text))
 */
void OptionText_init(OptionText * ot, char * text, u8 size);

/**
 * Function that looks like function(void * data) 
 */
typedef struct _OptionFunction {
    void (*func)(void * data);
    void * data;
} OptionFunction;

/**
 * Initializes an option function
 * @param of The uninitialized struct
 * @param func A pointer to the function to be called
 * @param data Data that the function needs (NULL if not necessary) 
 */
void OptFunc_init(OptionFunction * of, void (*func), void * data);

/**
 * Data and control variables for the options chooser 
 */
typedef struct _OptsChser {
    u8 options_size;
    OptionText * options;
    u8 current_option;
    OptionFunction * functions;
} OptsChser;

/**
 * Initializes the data of the options chooser 
 * @param oc The uninitialized struct
 * @param options The text and size of the (3 maximum) options
 * @param size The amount of options
 * @param functions The function to call when option[i] is clicked with A
 */
void OptsChser_init(OptsChser * oc, OptionText * options, u8 size, OptionFunction * functions);

/**
 * Listens for key strokes and shows the content in screen 
 */
void OptsChser_show(OptsChser * oc);

/**
 * Listens for keystrokes to move the current option up or down
 */
void OptsChser_keypoll(OptsChser * oc);


#endif