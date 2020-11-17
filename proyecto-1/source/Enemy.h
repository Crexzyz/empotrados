#ifndef ENEMY_H
#define ENEMY_H

#include <tonc.h>
#include "charsprites.h"
#include "Sprite.h"
#include "Rect.h"
#include <stdlib.h> // For random numbers
#include "CoinTile.h"

typedef struct Enemy
{
	OBJ_ATTR * sprite_attrs;
	int first_anim_frame; // Start animation frame
	int anim_frame; // Actual animation frame
	int pos_x; // Bigger value: More to the right
	int pos_y; // Bigger value: Lower position
    int speed_y; // Speed of movement
    int framesCurrent; // Frames needed to change animation
	int currentScore; // Count of enemys collected
	int hidden; // 1 if the enemy is hidden, 0 if not
} Enemy;

void sprite_enemy_load_to_mem();
void sprite_enemy_init(Enemy* sprite_enemy, OBJ_ATTR * attribs, int index);
void sprite_enemy_init_with_colis(Enemy* sprite_enemy, OBJ_ATTR * attribs, Sprite* sprite, int index);
void sprite_enemy_paint(Enemy* sprite_enemy);
void sprite_enemy_update_pos(Enemy* sprite_enemy);
void sprite_enemy_update_y_pos(Enemy* sprite_enemy);
void sprite_enemy_change_pos(Enemy* sprite_enemy);
void sprite_enemy_change_animation(Enemy* sprite_enemy);
int do_enemy_collision(Enemy* sprite_enemy, Sprite * sprite, Coin* coin);
int sprite_enemy_unhide(Enemy* jumps, Sprite * sprite);
void sprite_enemy_check_pos(Enemy* sprite_enemy, Sprite* sprite);
void enemy_hide(Enemy* sprite_enemy);

#endif