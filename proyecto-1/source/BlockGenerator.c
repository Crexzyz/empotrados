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
    blockgen->frame_interval = 10;
    blockgen->frame_counter = 0;
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

Rect * blockgen_get_block(BlockGenerator * blockgen, u32 block)
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

Rect * blockgen_get_bottom_block(BlockGenerator * blockgen)
{
    return NULL;
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

void blockgen_reposition(BlockGenerator * blockgen, Rect * block )
{
    u8 new_pos_x = qran_range(20, 220);
    rect_set_coords16(block, new_pos_x, 0);
}

void blockgen_move_blocks_with_map(BlockGenerator * blockgen)
{
    Map * map = blockgen->map;
    if(map)
    {
        if(key_is_down(KEY_RIGHT))
        {
            map_set_scroll(map, map->scroll_x + 2, map->scroll_y);
            for(size_t block = 0; block < MAX_BLOCKS; ++block)
            {
                Rect * rect = blockgen_get_block(blockgen, block);
                rect_set_coords16(rect, rect->x1 + 2, rect->y1);
            }
        }
        if(key_is_down(KEY_LEFT))
        {
            map_set_scroll(map, map->scroll_x - 2, map->scroll_y);
            for(size_t block = 0; block < MAX_BLOCKS; ++block)
            {
                Rect * rect = blockgen_get_block(blockgen, block);
                rect_set_coords16(rect, rect->x1 - 2, rect->y1);
            }
        }
        if(key_is_down(KEY_DOWN))
        {
            map_set_scroll(map, map->scroll_x, map->scroll_y + 2);
            for(size_t block = 0; block < MAX_BLOCKS; ++block)
            {
                Rect * rect = blockgen_get_block(blockgen, block);
                rect_set_coords16(rect, rect->x1, rect->y1 + 2);
            }
        }
        if(key_is_down(KEY_UP))
        {
            map_set_scroll(map, map->scroll_x, map->scroll_y - 2);
            for(size_t block = 0; block < MAX_BLOCKS; ++block)
            {
                Rect * rect = blockgen_get_block(blockgen, block);
                rect_set_coords16(rect, rect->x1, rect->y1 - 2);
            }
        }
    }
}