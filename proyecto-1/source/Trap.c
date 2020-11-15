#include "Trap.h"

/** 
* Method for the normal init
*/
void sprite_trap_init(Trap* sprite_trap, OBJ_ATTR * attribs){
    sprite_trap->sprite_attrs = attribs;
    sprite_trap->first_anim_frame = 10 * 8;
    sprite_trap->anim_frame = 10 * 8;
    sprite_trap->pos_x = (rand() % (200 - 17)) + 17;
    sprite_trap->pos_y = (rand() % (124 - 17)) + 17;
    sprite_trap->speed_y = 5;
    sprite_trap->framesCurrent = 0;
    sprite_trap->currentScore = 0;
    sprite_trap->hidden = 0;
}

/** 
* Method that check if exists collision between the trap and the sprite
*/
void sprite_trap_init_with_colis(Trap* sprite_trap, OBJ_ATTR * attribs, Sprite* sprite){
    sprite_trap->sprite_attrs = attribs;
    sprite_trap->first_anim_frame = 10 * 8;
    sprite_trap->anim_frame = 10 * 8;
    sprite_trap->pos_x = (rand() % (200 - 17)) + 17;
    sprite_trap->pos_y = (rand() % (124 - 17)) + 17;
    sprite_trap_check_pos(sprite_trap, sprite);
    sprite_trap->speed_y = 5;
    sprite_trap->framesCurrent = 0;
    sprite_trap->currentScore = 0;
    sprite_trap->hidden = 0;
}

/**
* Paint the trap in screen.
*/
void sprite_trap_paint(Trap* sprite_trap)
{
    // Set the sprite the 8 bits per pixel and square sprite bits
	sprite_trap->sprite_attrs->attr0 = ATTR0_8BPP | ATTR0_SQUARE ;
    // Set the sprite as a 16x16 pixel sprite
	sprite_trap->sprite_attrs->attr1 = ATTR1_SIZE_16;
    // Draw the sprite
	obj_set_pos(sprite_trap->sprite_attrs, sprite_trap->pos_x, sprite_trap->pos_y);
}

/**
* Update the position of the trap.
* If the trap is hidden, nothing is done.
* Else paint the trap in screen.
*/
void sprite_trap_update_pos(Trap* sprite_trap)
{
    sprite_trap_update_y_pos(sprite_trap);
    if(!sprite_trap->hidden) // If the trap is not hidden
        sprite_trap_paint(sprite_trap); // Paint the trap in screen
}

/**
* Update the "y" position of the trap.
*/
void sprite_trap_update_y_pos(Trap* sprite_trap)
{
    // Move the sprite up or down
    //sprite_trap->pos_y += sprite_trap->speed_y;
    // Give preference to the mid-air position
    sprite_trap->pos_y = MIN(sprite_trap->pos_y, FLOOR_Y);
}

/**
* Change the animation frame of the trap.
* It change every 5 frames.
*/
void sprite_trap_change_animation(Trap* sprite_trap)
{
    OBJ_ATTR* sprite_attrs = sprite_trap->sprite_attrs;
    // Advance a frame
    sprite_trap->framesCurrent++;
    if(sprite_trap->framesCurrent%5 == 0){
        sprite_trap->anim_frame++;
        sprite_trap->framesCurrent = 0;
    }
    // Cycle through the 6 moving frames
    sprite_trap->anim_frame = sprite_trap->anim_frame % 6;
    // Set the current sprite frame as base pixel + current frame pixel offset
	sprite_attrs->attr2= ATTR2_BUILD(16*8, 0, 0); 
}

/**
* Check if exists a collision between sprites.
* Return true or false if there is a collision
*/
int do_sprites_collision(Trap* sprite_trap, Sprite * sprite, Coin* coin){    
    Rect trap;
    Rect character;
    rect_init(&trap);
    rect_init(&character);
    rect_set_coords16(&trap, sprite_trap->pos_x, sprite_trap->pos_y);
    rect_set_coords16(&character, sprite->pos_x, sprite->pos_y);
    if(rect_intersects(&trap, &character, sprite->speed_x, sprite->speed_y)){
        // There is a overlap
        if(!sprite_trap->hidden){ // If the trap is not hidden
            coin->currentScore--;
            obj_hide(sprite_trap->sprite_attrs);
            sprite_trap->hidden = 1;
        }
    }
    return 1;
}

/**
* Method to unhide the trap, check the total jumps that the sprite has made.
* Verify if the count of jumps is the needed to put another trap.
*/
int sprite_trap_unhide(Trap* sprite_trap, Sprite * sprite){
    if(sprite_trap->hidden){ // If the trap is hidden
        if(sprite->jumps >= 4){ // Condition to put another trap
            sprite_trap->hidden = 0;
            sprite_trap->pos_x = (rand() % (200 - 17)) + 17;
            sprite_trap->pos_y = (rand() % (124 - 17)) + 17;
            sprite_trap_check_pos(sprite_trap, sprite);
            sprite_trap_update_pos(sprite_trap);
            obj_unhide(sprite_trap->sprite_attrs, 0 );
            // Reset the count
            sprite->jumps = 0;
            // Count reached
            return 1;
        }
    }else if(sprite->jumps >= 4){
        // Reset the count
        sprite->jumps = 0;
    }
    // Count not reached
    return 0;
}

/**
* This method check if where the trap is going to be placed exists the sprite.
*/
void sprite_trap_check_pos(Trap* sprite_trap, Sprite* sprite){
    Rect trap;
    Rect character;
    rect_init(&trap);
    rect_init(&character);
    rect_set_coords(&trap, sprite_trap->pos_x-20, sprite_trap->pos_y-20, sprite_trap->pos_x+20, sprite_trap->pos_y+20);
    rect_set_coords(&character, sprite->pos_x-20, sprite->pos_y-20, sprite->pos_x+20, sprite->pos_y+20);
    if(rect_intersects(&trap, &character, sprite->speed_x, sprite->speed_y)){
        if(sprite_trap->pos_x < 120){ // Left side of screen
            sprite_trap->pos_x = sprite_trap->pos_x + 60;
        }else{ // Right side of screen
            sprite_trap->pos_x = sprite_trap->pos_x - 60;
        }
    }
}