#ifndef DISPLAY_CONTROLLER_H
#define DISPLAY_CONTROLLER_H

#include <tonc.h>

/**________________________
 * |_________title________|
 * |                      |
 * |        content       |
 * |                      |
 * |______________________|
 */

#define TONC_TITLE_POS "#{P:100,10}"

/**
 * Contains basic data for the display controller: title, length, and 
 * a function to call to show the contents of the display
 */
typedef struct DisplayController
{
    char * title;
    u8 title_len;
    void (*content_function)(void * data);
    void * function_data;
} DispCtrl;

/**
 * Initializes the data for the Display Controller
 * @param dc The uninitialized controller
 * @param title The display title
 * @param len The display title's length
 * @param content_function The function to call to show some content on screen
 * @param data Optional data that the content function need
 */
void DispCtrl_init(DispCtrl * dc, char * title, u8 t_len, void (*content_function), void * data);

/**
 * Shows the title and content of the current screen
 * If there is no content function attached, no content is shown
 * @param dc The initialized display controller 
 */
void DispCtrl_show(DispCtrl * dc);

#endif