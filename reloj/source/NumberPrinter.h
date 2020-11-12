#ifndef NUMBER_PRINTER_H
#define NUMBER_PRINTER_H

#include <tonc.h>

/**
 * Calculates the position of the sprite data based on the background color and the actual number
 * @param bg_type The background color
 * @param number The number to lookup
 */
#define NP_BG_BUILD(bg_type, number) (12 * 8 * bg_type + 8 * number)

#define COLON 10
#define QUESTION_MARK 11

typedef struct _NumberPrinter {
    OBJ_ATTR * sprites;
    u32 sprite_amount;
    u8 rainbow_color_index;
} NumberPrinter;

enum bg_colors {
    WHITE,
    BLACK,
    GREEN,
    RED,
    YELLOW
};

void np_load();

/**
 * Initializes the NumberPrinter struct and copies the sprites to VRAM
 * @param np the non-initialized struct
 * @param sprites pointer to the first sprite "container"
 * @param amount amount of sprites to use as text display (size of the buffer)
 */
void np_init(NumberPrinter * np, OBJ_ATTR * sprites, u32 amount);

/**
 * Prints numbers on-screen using special sprites
 * @param np The NumberPrinter struct, previously initialized
 * @param x the x coordinate to start printing
 * @param y the y coordinate to start printing
 * @param color the background color of the numbers 
 * @param numbers an array with the numbers in ascii format
 * @param len the length of the array
 * @see bg_colors for the available background colors
 */
void np_print(NumberPrinter * np, u32 x, u32 y, u8 color, char * numbers, u32 len);

void np_print_highlight(NumberPrinter * np, u32 x, u32 y, u8 color, u8 highlight_color, u8 highlight_index, char * numbers, u32 len);

void np_rainbow_print(NumberPrinter * np, u32 x, u32 y, char * numbers, u32 len);

#endif