#include "BlockGenerator.h"
#include <tonc.h>

void blockgen_init(BlockGenerator * blockgen, OBJ_ATTR * obj_buffer)
{
    blockgen->obj_buffer = obj_buffer;
    blockgen->blocks = &blocks;
    blockgen->autoscrolling_speed = 1;
    blockgen->frame_interval = 2;
    blockgen->frame_counter = 0;
    blockgen->previous_direction = 0;
    blockgen->previous_direction_counter = 0;
}

void blockgen_init_blocks(BlockGenerator * blockgen)
{
    for(size_t block = 0; block < BLOCKS_AMOUNT; ++block)
    {
        rect_init(&blockgen->blocks[block]);
        rect_set_sprite(&blockgen->blocks[block], &blockgen->obj_buffer[OBJ_BUFFER_BASE_INDEX + block]);
    }

    u8 base_y = 140;
    u8 increment_y = 40;

    for(size_t block = 0; block < BLOCKS_AMOUNT; ++block)
    {
        if(block % 2 == 0)
        {
            rect_set_coords16(&blockgen->blocks[block], qran_range(0, 100), base_y);
        }
        else
        {
            rect_set_coords16(&blockgen->blocks[block], qran_range(120, 220), base_y);
            base_y -= increment_y;
        }
    }
}

int blockgen_autoscroll(BlockGenerator * blockgen)
{
    // This function is called each VBlank/frame
    blockgen->frame_counter = blockgen->frame_counter + 1 % 60;

    if(blockgen->frame_counter % blockgen->frame_interval == 0)
    {
        for(size_t block = 0; block < BLOCKS_AMOUNT; ++block)
        {
            Rect * rect = BLOCKGEN_GET_BLOCK(block);

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
    for(size_t block = 0; block < BLOCKS_AMOUNT; ++block)
    {
        if(BLOCKGEN_GET_BLOCK(block)->y1 < highest_y)
        {
            highest_y = BLOCKGEN_GET_BLOCK(block)->y1;
            highest_x = BLOCKGEN_GET_BLOCK(block)->x1;
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
