#ifndef MAP_H
#define MAP_H

typedef struct _Map
{
    u32 scroll_x;
    u32 scroll_y;
    u32 max_x;
    u32 max_y;
    const u8 * collision_map;
} Map;

enum TileTypes
{
    AIR = 0, SOLID
};

void map_load_to_mem();
void map_init(Map * map);
void map_set_scroll(Map * map, u32 x, u32 y);
void map_key_move(Map * map);
void map_set_collision_map(Map * map, const u8 * ptr);
int map_get_tile_type(Map * map, u32 x, u32 y);

#endif