#ifndef SPRITE_H
#define SPRITE_H

#include <tonc.h>
#include "Map.h"

#define SPRITE_8PIXEL_OFFSET(state) (8 * state)

#define FLOOR_Y 140
#define GRAVITY_FORCE 3
#define WALK_SPEED 10
#define JUMP_THRESHOLD -15
#define FALL_MAX_SPEED 15

typedef struct Sprite
{
	OBJ_ATTR * sprite_attrs;
	int facing_right;
	int first_anim_frame;
	int anim_frame;
	int pos_x;
	int pos_y;
	int speed_x;
	int speed_y;
	int frames_in_air;
} Sprite;

enum SpriteFrames 
{
    STILL1 = 0, STILL2, STILL3, STILL4,
    MOVE_H1, MOVE_H2, MOVE_H3, JUMP_UP,
    JUMP_DOWN
};

enum SpriteStates
{
    STILL, MOVING, JUMPING
};

void sprite_load_to_mem();
void sprite_init(Sprite* sprite, OBJ_ATTR * attribs);
void sprite_update_position(Sprite * sprite);
void sprite_map_update_position(Map * map, Sprite * sprite);
void sprite_map_update_x_pos(Map * map, Sprite * sprite);
void sprite_update_x_pos(Sprite * sprite);
void sprite_update_y_pos(Sprite * sprite);
void sprite_change_animation(Sprite * sprite);
int sprite_get_state(Sprite * sprite);

#endif