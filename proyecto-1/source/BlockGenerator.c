#include "BlockGenerator.h"
#include <tonc.h>

void blockgen_init(BlockGenerator * blockgen)
{
    blockgen->r1 = 0;
    blockgen->r2 = 0;
    blockgen->r3 = 0;
    blockgen->r4 = 0;
    blockgen->map = 0;
    blockgen->autoscrolling_speed = 1;
    blockgen->frame_interval = 2;
    blockgen->frame_counter = 0;
    blockgen->previous_direction = 0;
    blockgen->previous_direction_counter = 0;
}

void blockgen_set_map(BlockGenerator * blockgen, Map * map)
{
    blockgen->map = map;
}

void blockgen_set_blocks(BlockGenerator * blockgen, Rect ** blocks)
{
    blockgen->r1 = &(*blocks)[0];
    blockgen->r2 = &(*blocks)[1];
    blockgen->r3 = &(*blocks)[2];
    blockgen->r4 = &(*blocks)[3];
}

Rect * blockgen_get_block(BlockGenerator * blockgen, size_t block)
{
    switch (block)
    {
        case 0: return blockgen->r1;
        case 1: return blockgen->r2;
        case 2: return blockgen->r3;
        case 3: return blockgen->r4;
        default: return NULL;
    }
}

int blockgen_autoscroll(BlockGenerator * blockgen)
{
    // This function is called each VBlank/frame
    blockgen->frame_counter = blockgen->frame_counter + 1 % 60;

    if(blockgen->frame_counter % blockgen->frame_interval == 0)
    {
        for(size_t block = 0; block < MAX_BLOCKS; ++block)
        {
            Rect * rect = blockgen_get_block(blockgen, block);

            if(rect->y1 > 160)
                blockgen_reposition(blockgen, rect);
            else
                rect_set_coords16(rect, rect->x1, rect->y1 + blockgen->autoscrolling_speed);
        }

        return 1;
    }

    return 0;
}

void blockgen_reposition(BlockGenerator * blockgen, Rect * target )
{
    // Lowest Y is 160
    u8 highest_x = 0;
    u8 highest_y = 160;
    // Get X coordinate from the highest block
    for(size_t block = 0; block < MAX_BLOCKS; ++block)
    {
        if(blockgen_get_block(blockgen, block)->y1 < highest_y)
        {
            highest_y = blockgen_get_block(blockgen, block)->y1;
            highest_x = blockgen_get_block(blockgen, block)->x1;
        }
    }

    // Randomly choose if next block comes at the left or the right
    // of the highest one
    u8 new_pos_at_left = 1;

    if(new_pos_at_left == blockgen->previous_direction)
        blockgen->previous_direction_counter++;

    if(blockgen->previous_direction_counter > 2)
    {
        blockgen->previous_direction = new_pos_at_left = !new_pos_at_left;
        blockgen->previous_direction_counter = 0;
    }

    u8 new_pos_x = 0;

    if(new_pos_at_left || highest_x >= 220) // Random choosing or block too close to the right
    {   
        if(highest_x <= 80) // Too close to the left, force the block to the right
            new_pos_x = qran_range(highest_x + 40, 130);
        else
            new_pos_x = qran_range(MAX(20, highest_x - 60), highest_x - 40);
    }
    else
    {
        new_pos_x = qran_range(highest_x + 40, MIN(highest_x + 80, 220));
    }


    rect_set_coords16(target, new_pos_x, 0);
}
