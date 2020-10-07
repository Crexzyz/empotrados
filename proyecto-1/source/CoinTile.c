#include "CoinTile.h"

/** 
* Method for the normal init
*/
void sprite_coin_init(Coin* sprite_coin, OBJ_ATTR * attribs){
    sprite_coin->sprite_attrs = attribs;
    sprite_coin->first_anim_frame = 10 * 8;
    sprite_coin->anim_frame = 10 * 8;
    sprite_coin->pos_x = (rand() % (200 - 17)) + 17;
    sprite_coin->pos_y = (rand() % (124 - 17)) + 17;
    sprite_coin->speed_y = 5;
    sprite_coin->framesCurrent = 0;
    sprite_coin->currentScore = 0;
    sprite_coin->hidden = 0;
}

/** 
* Method that check if exists collision between the coin and the sprite
*/
void sprite_coin_init_with_colis(Coin* sprite_coin, OBJ_ATTR * attribs, Sprite* sprite){
    sprite_coin->sprite_attrs = attribs;
    sprite_coin->first_anim_frame = 10 * 8;
    sprite_coin->anim_frame = 10 * 8;
    sprite_coin->pos_x = (rand() % (200 - 17)) + 17;
    sprite_coin->pos_y = (rand() % (124 - 17)) + 17;
    sprite_coin_check_pos(sprite_coin, sprite);
    sprite_coin->speed_y = 5;
    sprite_coin->framesCurrent = 0;
    sprite_coin->currentScore = 0;
    sprite_coin->hidden = 0;
}

/**
* Paint the coin in screen.
*/
void sprite_coin_paint(Coin* sprite_coin)
{
    // Set the sprite the 8 bits per pixel and square sprite bits
	sprite_coin->sprite_attrs->attr0 = ATTR0_8BPP | ATTR0_SQUARE ;
    // Set the sprite as a 16x16 pixel sprite
	sprite_coin->sprite_attrs->attr1 = ATTR1_SIZE_16;
    // Draw the sprite
	obj_set_pos(sprite_coin->sprite_attrs, sprite_coin->pos_x, sprite_coin->pos_y);
}

/**
* Update the position of the coin.
* If the coin is hidden, nothing is done.
* Else paint the coin in screen.
*/
void sprite_coin_update_pos(Coin* sprite_coin)
{
    sprite_coin_update_y_pos(sprite_coin);
    if(!sprite_coin->hidden) // If the coin is not hidden
        sprite_coin_paint(sprite_coin); // Paint the coin in screen
}

/**
* Update the "y" position of the coin.
*/
void sprite_coin_update_y_pos(Coin* sprite_coin)
{
    // Move the sprite up or down
    //sprite_coin->pos_y += sprite_coin->speed_y;
    // Give preference to the mid-air position
    sprite_coin->pos_y = MIN(sprite_coin->pos_y, FLOOR_Y);
}

/**
* Change the animation frame of the coin.
* It change every 5 frames.
*/
void sprite_coin_change_animation(Coin* sprite_coin)
{
    OBJ_ATTR* sprite_attrs = sprite_coin->sprite_attrs;
    // Advance a frame
    sprite_coin->framesCurrent++;
    if(sprite_coin->framesCurrent%5 == 0){
        sprite_coin->anim_frame++;
        sprite_coin->framesCurrent = 0;
    }
    // Cycle through the 6 moving frames
    sprite_coin->anim_frame = sprite_coin->anim_frame % 6;
    // Set the current sprite frame as base pixel + current frame pixel offset
	sprite_attrs->attr2= ATTR2_BUILD(sprite_coin->first_anim_frame + (sprite_coin->anim_frame*8), 0, 0); 
}

/**
* Check if exists a collision between sprites.
* Return true or false if there is a collision
*/
int do_sprites_collisions(Coin* sprite_coin, Sprite * sprite){    
    Rect coin;
    Rect character;
    rect_init(&coin);
    rect_init(&character);
    rect_set_coords(&coin, sprite_coin->pos_x, sprite_coin->pos_y, sprite_coin->pos_x+15, sprite_coin->pos_y+15);
    rect_set_coords(&character, sprite->pos_x, sprite->pos_y, sprite->pos_x+15, sprite->pos_y+15);
    if(rect_intersects(&coin, &character, sprite->speed_x, sprite->speed_y)){
        // There is a overlap
        if(!sprite_coin->hidden){ // If the coin is not hidden
            sprite_coin->currentScore++;
            obj_hide(sprite_coin->sprite_attrs);
            sprite_coin->hidden = 1;
        }
    }
    return 1;
}

/**
* Method to unhide the coin, check the total jumps that the sprite has made.
* Verify if the count of jumps is the needed to put another coin.
*/
int sprite_coin_unhide(Coin* sprite_coin, Sprite * sprite){
    if(sprite_coin->hidden){ // If the coin is hidden
        if(sprite->jumps >= 4){ // Condition to put another coin
            sprite_coin->hidden = 0;
            sprite_coin->pos_x = (rand() % (200 - 17)) + 17;
            sprite_coin->pos_y = (rand() % (124 - 17)) + 17;
            sprite_coin_check_pos(sprite_coin, sprite);
            sprite_coin_update_pos(sprite_coin);
            obj_unhide(sprite_coin->sprite_attrs, 0 );
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
* This method check if where the coin is going to be placed exists the sprite.
*/
void sprite_coin_check_pos(Coin* sprite_coin, Sprite* sprite){
    Rect coin;
    Rect character;
    rect_init(&coin);
    rect_init(&character);
    rect_set_coords(&coin, sprite_coin->pos_x-20, sprite_coin->pos_y-20, sprite_coin->pos_x+20, sprite_coin->pos_y+20);
    rect_set_coords(&character, sprite->pos_x-20, sprite->pos_y-20, sprite->pos_x+20, sprite->pos_y+20);
    if(rect_intersects(&coin, &character, sprite->speed_x, sprite->speed_y)){
        if(sprite_coin->pos_x < 120){ // Left side of screen
            sprite_coin->pos_x = sprite_coin->pos_x + 60;
        }else{ // Right side of screen
            sprite_coin->pos_x = sprite_coin->pos_x - 60;
        }
    }
}