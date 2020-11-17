#ifndef HEART_H
#define HEART_H

#include <tonc.h>
#include "charsprites.h"
#include "Sprite.h"
#include "Rect.h"
#include <stdlib.h> // For random numbers
#include "CoinTile.h"

typedef struct Heart
{
	OBJ_ATTR * sprite_attrs;
	int first_anim_frame; // Start animation frame
	int anim_frame; // Actual animation frame
	int pos_x; // Bigger value: More to the right
	int pos_y; // Bigger value: Lower position
    int speed_y; // Speed of movement
    int framesCurrent; // Frames needed to change animation
	int currentScore; // Count of hearts collected
	int hidden; // 1 if the heart is hidden, 0 if not
} Heart;

void sprite_heart_load_to_mem();
void sprite_heart_init(Heart* sprite_heart, OBJ_ATTR * attribs);
void sprite_heart_init_with_colis(Heart* sprite_heart, OBJ_ATTR * attribs, Sprite* sprite);
void sprite_heart_paint(Heart* sprite_heart);
void sprite_heart_update_pos(Heart* sprite_heart);
void sprite_heart_update_y_pos(Heart* sprite_heart);
void sprite_heart_change_pos(Heart* sprite_heart);
void sprite_heart_change_animation(Heart* sprite_heart);
int do_heart_collision(Heart* sprite_heart, Sprite * sprite, Coin* coin);
int sprite_heart_unhide(Heart* jumps, Sprite * sprite);
void sprite_heart_check_pos(Heart* sprite_heart, Sprite* sprite);
void heart_hide(Heart* sprite_heart);

#endif