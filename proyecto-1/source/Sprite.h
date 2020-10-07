#ifndef SPRITE_H
#define SPRITE_H

#include <tonc.h>

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
	// Boolean
	int facing_right;
	// First frame of the animation, depending on the state
	int first_anim_frame;
	// Current frame of the animation
	int anim_frame;
	// X Coordinate
	int pos_x;
	// Y Coordinate
	int pos_y;
	// Speed at what the player moves horizontally in one frame
	int speed_x;
	// Speed at what the player moves vertically in one frame
	int speed_y;
	// Counter
	int frames_in_air;
	// Pixels needed to put another coin
	int jumps; 
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
void sprite_update_x_pos(Sprite * sprite);
void sprite_update_y_pos(Sprite * sprite);
void sprite_change_animation(Sprite * sprite);
int sprite_get_state(Sprite * sprite);
// Places a sprite on top of a rect
void sprite_place_on_rect(Sprite * sprite, Rect * rect);

#endif