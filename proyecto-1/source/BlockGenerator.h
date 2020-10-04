#ifndef BLOCK_GENERATOR_H
#define BLOCK_GENERATOR_H

#include "Rect.h"
#include "Map.h"

#define MAX_BLOCKS 4

typedef struct _BlockGenerator
{
    Rect * r1;
    Rect * r2;
    Rect * r3;
    Rect * r4;
    Map * map;
    // Pixels that blocks will move on each interval
    u8 autoscrolling_speed;
    // Frames that have to pass between each scroll
    u8 frame_interval;
    // Frame counter for the interval
    u8 frame_counter;

} BlockGenerator;

void blockgen_init(BlockGenerator * blockgen);
void blockgen_set_map(BlockGenerator * blockgen, Map * map);
void blockgen_set_blocks(BlockGenerator * blockgen, Rect ** blocks);
Rect * blockgen_get_block(BlockGenerator * blockgen, u32 block);
Rect * blockgen_get_bottom_block(BlockGenerator * blockgen);
void blockgen_move_blocks_with_map(BlockGenerator * blockgen);
// Returns 1 if a scroll was performed, 0 otherwise
int blockgen_autoscroll(BlockGenerator * blockgen);
// Repositions a block that reached the end of the screen
void blockgen_reposition(BlockGenerator * blockgen, Rect * block );

#endif