#include "BlockGenerator.h"
#include <tonc.h>

// Restrict access to the BlockGenerator file
static Rect blocks[BLOCKS_AMOUNT];

void blockgen_init(BlockGenerator * blockgen, OBJ_ATTR * obj_buffer)
{
    blockgen->obj_buffer = obj_buffer;
    blockgen->blocks = (Rect *)&blocks;
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

    // Y where blocks start to appear
    u8 base_y = 140;
    // Y size between blocks
    u8 increment_y = 40;

    for(size_t block = 0; block < BLOCKS_AMOUNT; ++block)
    {
        if(block % 2 == 0) // Odd blocks are placed on the left
        {
            rect_set_coords16(&blockgen->blocks[block], qran_range(0, 100), base_y);
        }
        else // Move a row up each 2 blocks
        {
            rect_set_coords16(&blockgen->blocks[block], qran_range(120, 220), base_y);
            base_y -= increment_y;
        }
    }
}

int blockgen_autoscroll(BlockGenerator * blockgen)
{
    // This function is called each VBlank/frame
    blockgen->frame_counter = (blockgen->frame_counter + 1) % 60;

    // When the frames needed to move a block down reach the limit
    if(blockgen->frame_counter % blockgen->frame_interval == 0)
    {
        for(size_t block = 0; block < BLOCKS_AMOUNT; ++block)
        {
            Rect * rect = BLOCKGEN_GET_BLOCK(block);

            // If the block reached the end of the screen, reposition it on the top
            if(rect->y1 > 160) 
                blockgen_reposition8(blockgen, rect, block);
            else // Move the block (speed) pixels down
                rect_set_coords16(rect, rect->x1, rect->y1 + blockgen->autoscrolling_speed);
        }

        return 1;
    }

    return 0;
}

Rect * blockgen_get_topmost_block(BlockGenerator * blockgen, u8 start_index, u8 increment)
{
    // Lowest Y is 160
    u8 highest_y = 160;
    // Stores the highest block index to return it later
    u8 highest_block_index = 0;
    // Check only the blocks that are on the same side of the screen
    for(size_t block = start_index; block < BLOCKS_AMOUNT; block += increment)
    {
        if(BLOCKGEN_GET_BLOCK(block)->y1 < highest_y)
        {
            highest_block_index = block;
            highest_y = BLOCKGEN_GET_BLOCK(block)->y1;
        }
    }

    return BLOCKGEN_GET_BLOCK(highest_block_index);
}

Rect * blockgen_get_topmost_block8(BlockGenerator * blockgen, u8 current_block)
{
    return blockgen_get_topmost_block(blockgen, current_block % 2, 2);
}

Rect * blockgen_get_topmost_block4(BlockGenerator * blockgen)
{
    return blockgen_get_topmost_block(blockgen, 0, 1);
}

void blockgen_reposition4(BlockGenerator * blockgen, Rect * target )
{
    u8 highest_x = blockgen_get_topmost_block4(blockgen)->x1;

    // Randomly choose if next block comes at the left or the right
    // of the highest one
    u8 new_pos_at_left = 1;

    u8 new_pos_x = 0;

    // If the random variable chose the left, or the highest x is too close to the right
    if(new_pos_at_left || highest_x >= 220) 
    {   
        // If the block is too close to the left, move the block to the right
        if(highest_x <= 80) 
            new_pos_x = qran_range(highest_x + 40, 130);
        else // Move the block to the left some pixels
            new_pos_x = qran_range(MAX(20, highest_x - 60), highest_x - 40);
    }
    else // Move the block to the right some pixels
    {
        new_pos_x = qran_range(highest_x + 40, MIN(highest_x + 80, 220));
    }

    rect_set_coords16(target, new_pos_x, 0);
}

void blockgen_reposition8(BlockGenerator * blockgen, Rect * target, size_t block )
{
    u8 new_pos_x = 0;
    // Get X coordinate of the topmost block in this frame
    u8 highest_x = blockgen_get_topmost_block8(blockgen, (u8)block)->x1;

    // Get a random x between 0 and 100 (left side) or
    // Get a random x between 120 and 220 (right side)
    new_pos_x += block % 2 == 0 ? qran_range(0, 100) : qran_range(120, 220);

    // Avoid placing the block directly on top of another
    if( highest_x - 8 < new_pos_x && new_pos_x < highest_x + 15)
        new_pos_x += block % 2 == 0 ? 16 : -16;

    rect_set_coords16(target, new_pos_x, 0);
}