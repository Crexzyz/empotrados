#include <tonc.h>
#include <string.h>

#include "bombmap.h"
#include "Map.h"

void map_init(Map * map)
{
    map->scroll_x = 0;
    map->scroll_y = 0;
    map->max_x = 0;
    map->max_y = 0;
    map->collision_map = NULL;
}

void map_set_collision_map(Map * map, const u8 * ptr)
{
    map->collision_map = ptr;
}

void map_load_to_mem()
{
    // Palette
    memcpy(pal_bg_mem, bombmapPal, bombmapPalLen);
    // Tiles in CBB 0
    memcpy(&tile8_mem[0][0], bombmapTiles, bombmapTilesLen);
    // Map in SBB 30
    memcpy(&se_mem[30][0], bombmapMap, bombmapMapLen);
}

void map_set_scroll(Map * map, u32 x, u32 y)
{
    REG_BG1HOFS = map->scroll_x = x;
    REG_BG1VOFS = map->scroll_y = y;
}

void map_key_move(Map * map)
{
    if(key_is_down(KEY_RIGHT))
        map_set_scroll(map, map->scroll_x + 2, map->scroll_y);
    if(key_is_down(KEY_LEFT))
        map_set_scroll(map, map->scroll_x - 2, map->scroll_y);
    if(key_is_down(KEY_DOWN))
        map_set_scroll(map, map->scroll_x, map->scroll_y + 2);
    if(key_is_down(KEY_UP))
        map_set_scroll(map, map->scroll_x, map->scroll_y - 2);
}

int map_get_tile_type(Map * map, u32 x, u32 y)
{
    if(map->collision_map == NULL)
        return 1;
    else
    {
        if(x > map->max_x - 1 || y > map->max_y - 1)
            return 1;
        else
            return map->collision_map[x * map->max_x + y];
    }
}