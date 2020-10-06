#ifndef BLOCK_GENERATOR_H
#define BLOCK_GENERATOR_H

#include "Rect.h"

#define OBJ_BUFFER_BASE_INDEX 2
#define BLOCKS_AMOUNT 8

#define BLOCKGEN_GET_BLOCK(block) (&blockgen->blocks[block])

typedef struct _BlockGenerator
{
    // Sprite buffer used tp set the sprites
    OBJ_ATTR * obj_buffer;
    // Array of BLOCKS_AMOUNT blocks that gets displayed on screen
    Rect * blocks;
    // Pixels that blocks will move on each interval
    u8 autoscrolling_speed;
    // Frames that have to pass between each scroll
    u8 frame_interval;
    // Frame counter for the interval
    u8 frame_counter;
    // To prevent blocks getting placed on one side of the map always
    u8 previous_direction_counter;
    // Last placed block's direction
    u8 previous_direction;

} BlockGenerator;

// Initializes pointers and values with a zero
void blockgen_init(BlockGenerator * blockgen, OBJ_ATTR * obj_buffer);
// Initializes the array of blocks with zeroes
void blockgen_init_blocks(BlockGenerator * blockgen);
// Returns 1 if a scroll was performed, 0 otherwise
int blockgen_autoscroll(BlockGenerator * blockgen);
// Repositions blocks that reached the end of the screen (4 blocks on screen)
void blockgen_reposition4(BlockGenerator * blockgen, Rect * block );
// Repositions blocks that reached the end of the screen (8 blocks on screen)
void blockgen_reposition8(BlockGenerator * blockgen, Rect * target, size_t block );
// Gets the rect at the top when there are 8 blocks on screen (4 on each half of the screen)
Rect * blockgen_get_topmost_block8(BlockGenerator * blockgen, u8 block);
// Gets the rect at the top when there are 4 blocks on screen (placed randomly)
Rect * blockgen_get_topmost_block4(BlockGenerator * blockgen);

#endif