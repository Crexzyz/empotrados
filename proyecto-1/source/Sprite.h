#ifndef SPRITE_H
#define SPRITE_H

#include <tonc.h>

#include "Map.h"
#include "Rect.h"

#define SPRITE_8PIXEL_OFFSET(state) (8 * state)

#define FLOOR_Y 140
#define ACCELERATION 1
#define WALK_SPEED 5
#define VELOCITY -9
#define FALL_MAX_SPEED 5

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
void sprite_update_pos_collision(Sprite * sprite, Rect ** rects, size_t rects_amount);
void sprite_update_x_pos_speed(Sprite * sprite);
void sprite_update_y_pos_speed(Sprite * sprite);
void sprite_update_xy_collision(Sprite * sprite, Rect ** rects, size_t rects_amount);
void sprite_update_position(Sprite * sprite);
void note_play(int note, int octave);
void sprite_update_x_pos(Sprite * sprite);
void sprite_update_y_pos(Sprite * sprite);
void sprite_change_animation(Sprite * sprite);
int sprite_get_state(Sprite * sprite);

#endif