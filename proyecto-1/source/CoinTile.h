#ifndef COIN_TILE_H
#define COIN_TILE_H

#include <tonc.h>

#define SPRITE_8PIXEL_OFFSET(state) (8 * state)

#define FLOOR_Y 140
#define ROOF_Y 240 // ????
#define ACCELERATION 1
#define WALK_SPEED 5
#define VELOCITY -10
#define FALL_MAX_SPEED 5

typedef struct Coin
{
	OBJ_ATTR * sprite_attrs;
	int first_anim_frame; // Start animation frame
	int anim_frame;
	int pos_x; // Bigger value: More to the right
	int pos_y; // Bigger value: Lower position
    int speed_y;
	int frames_in_air;
    int framesCurrent;
} Coin;

void sprite_coin_load_to_mem();
void sprite_coin_init(Coin* sprite_coin, OBJ_ATTR * attribs);
void sprite_coin_paint(Coin* sprite_coin);
void sprite_coin_update_pos(Coin* sprite_coin);
void sprite_coin_update_y_pos(Coin* sprite_coin);
void sprite_coin_change_animation(Coin* sprite_coin);

// void sprite_update_pos_collision(Sprite * sprite, Rect ** rects, size_t rects_amount);
// void sprite_update_x_pos_speed(Sprite * sprite);
// void sprite_update_y_pos_speed(Sprite * sprite);
// void sprite_update_xy_collision(Sprite * sprite, Rect ** rects, size_t rects_amount);
// void sprite_update_position(Sprite * sprite);
// void sprite_update_x_pos(Sprite * sprite);
// void sprite_update_y_pos(Sprite * sprite);
// void sprite_change_animation(Sprite * sprite);
// int sprite_get_state(Sprite * sprite);

#endif