#ifndef TRAP_H
#define TRAP_H

#include <tonc.h>
#include "charsprites.h"
#include "Sprite.h"
#include "Rect.h"
#include <stdlib.h> // For random numbers
#include "CoinTile.h"

typedef struct Trap
{
	OBJ_ATTR * sprite_attrs;
	int first_anim_frame; // Start animation frame
	int anim_frame; // Actual animation frame
	int pos_x; // Bigger value: More to the right
	int pos_y; // Bigger value: Lower position
    int speed_y; // Speed of movement
    int framesCurrent; // Frames needed to change animation
	int currentScore; // Count of traps collected
	int hidden; // 1 if the trap is hidden, 0 if not
} Trap;

void sprite_trap_load_to_mem();
void sprite_trap_init(Trap* sprite_trap, OBJ_ATTR * attribs);
void sprite_trap_init_with_colis(Trap* sprite_trap, OBJ_ATTR * attribs, Sprite* sprite);
void sprite_trap_paint(Trap* sprite_trap);
void sprite_trap_update_pos(Trap* sprite_trap);
void sprite_trap_update_y_pos(Trap* sprite_trap);
void sprite_trap_change_animation(Trap* sprite_trap);
int do_sprites_collision(Trap* sprite_trap, Sprite * sprite, Coin* coin);
int sprite_trap_unhide(Trap* jumps, Sprite * sprite);
void sprite_trap_check_pos(Trap* sprite_trap, Sprite* sprite);
void trap_hide(Trap* sprite_trap);

#endif