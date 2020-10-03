#ifndef COIN_TILE_H
#define COIN_TILE_H

#include <tonc.h>
#include "charsprites.h"
#include "Sprite.h"

#define FLOOR_Y 140
#define WALK_SPEED 5
#define FALL_MAX_SPEED 5

typedef struct Coin
{
	OBJ_ATTR * sprite_attrs;
	int first_anim_frame; // Start animation frame
	int anim_frame; // Actual animation frame
	int pos_x; // Bigger value: More to the right
	int pos_y; // Bigger value: Lower position
    int speed_y; // Speed of movement
    int framesCurrent; // Frames needed to change animation
	int currentScore; // Count of coins collected
	int hidden; // 1 if the coin is hidden, 0 if not
} Coin;

typedef struct {
	int x,y;
} Point;

void sprite_coin_load_to_mem();
void sprite_coin_init(Coin* sprite_coin, OBJ_ATTR * attribs);
void sprite_coin_paint(Coin* sprite_coin);
void sprite_coin_update_pos(Coin* sprite_coin);
void sprite_coin_update_y_pos(Coin* sprite_coin);
void sprite_coin_change_animation(Coin* sprite_coin);

// Return true or false if there is a collision
int do_sprites_collisions(Coin* sprite_coin, Sprite * sprite);

#endif