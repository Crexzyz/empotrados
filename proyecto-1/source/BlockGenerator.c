#include "BlockGenerator.h"

void blockgen_init(BlockGenerator * blockgen)
{
    blockgen->r1 = 0;
    blockgen->r2 = 0;
    blockgen->r3 = 0;
    blockgen->map = 0;
    blockgen->autoscrolling_speed = 1; // No autoscroll
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
}

Rect * blockgen_get_block(BlockGenerator * blockgen, u32 block)
{
    switch (block)
    {
        case 0: return blockgen->r1;
        case 1: return blockgen->r2;
        case 2: return blockgen->r3;
        default: return NULL;
    }
}

Rect * blockgen_get_bottom_block(BlockGenerator * blockgen)
{
    return NULL;
}

void blockgen_autoscroll(BlockGenerator * blockgen)
{
    for(size_t block = 0; block < MAX_BLOCKS; ++block)
    {
        Rect * rect = blockgen_get_block(blockgen, block);

        if(rect->y1 > 160)
            rect_set_coords16(rect, rect->x1, 0);
        else
            rect_set_coords16(rect, rect->x1, rect->y1 + blockgen->autoscrolling_speed);
    }
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