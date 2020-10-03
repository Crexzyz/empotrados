#ifndef BLOCK_GENERATOR_H
#define BLOCK_GENERATOR_H

#include "Rect.h"
#include "Map.h"

#define MAX_BLOCKS 3

typedef struct _BlockGenerator
{
    Rect * r1;
    Rect * r2;
    Rect * r3;
    Map * map;
    u8 autoscrolling_speed;

} BlockGenerator;

void blockgen_init(BlockGenerator * blockgen);
void blockgen_set_map(BlockGenerator * blockgen, Map * map);
void blockgen_set_blocks(BlockGenerator * blockgen, Rect ** blocks);
Rect * blockgen_get_block(BlockGenerator * blockgen, u32 block);
Rect * blockgen_get_bottom_block(BlockGenerator * blockgen);
void blockgen_move_blocks_with_map(BlockGenerator * blockgen);
void blockgen_autoscroll(BlockGenerator * blockgen);

#endif