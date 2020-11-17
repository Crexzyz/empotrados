#include "Heart.h"

/** 
* Method for the normal init
*/
void sprite_heart_init(Heart* sprite_heart, OBJ_ATTR * attribs){
    sprite_heart->sprite_attrs = attribs;
    sprite_heart->first_anim_frame = 19 * 8;
    sprite_heart->anim_frame = 19 * 8;
    sprite_heart->pos_x = (rand() % (200 - 17)) + 17;
    sprite_heart->pos_y = (rand() % (124 - 17)) + 17;
    sprite_heart->speed_y = 5;
    sprite_heart->framesCurrent = 0;
    sprite_heart->currentScore = 0;
    sprite_heart->hidden = 0;
}

/** 
* Method that check if exists collision between the heart and the sprite
*/
void sprite_heart_init_with_colis(Heart* sprite_heart, OBJ_ATTR * attribs, Sprite* sprite){
    sprite_heart->sprite_attrs = attribs;
    sprite_heart->first_anim_frame = 19 * 8;
    sprite_heart->anim_frame = 19 * 8;
    sprite_heart->pos_x = (rand() % (200 - 17)) + 17;
    sprite_heart->pos_y = (rand() % (124 - 17)) + 17;
    sprite_heart_check_pos(sprite_heart, sprite);
    sprite_heart->speed_y = 5;
    sprite_heart->framesCurrent = 0;
    sprite_heart->currentScore = 0;
    sprite_heart->hidden = 0;
}

/**
* Paint the heart in screen.
*/
void sprite_heart_paint(Heart* sprite_heart)
{
    // Set the sprite the 8 bits per pixel and square sprite bits
	sprite_heart->sprite_attrs->attr0 = ATTR0_8BPP | ATTR0_SQUARE ;
    // Set the sprite as a 16x16 pixel sprite
	sprite_heart->sprite_attrs->attr1 = ATTR1_SIZE_16;
    // Draw the sprite
	obj_set_pos(sprite_heart->sprite_attrs, sprite_heart->pos_x, sprite_heart->pos_y);
}

/**
* Update the position of the heart.
* If the heart is hidden, nothing is done.
* Else paint the heart in screen.
*/
void sprite_heart_update_pos(Heart* sprite_heart)
{
    sprite_heart_update_y_pos(sprite_heart);
    if(!sprite_heart->hidden) // If the heart is not hidden
        sprite_heart_paint(sprite_heart); // Paint the heart in screen
}

void sprite_heart_change_pos(Heart* sprite_heart)
{
    sprite_heart->pos_x = (rand() % (200 - 17)) + 17;
    sprite_heart->pos_y = (rand() % (124 - 17)) + 17;
}

/**
* Update the "y" position of the heart.
*/
void sprite_heart_update_y_pos(Heart* sprite_heart)
{
    // Move the sprite up or down
    //sprite_heart->pos_y += sprite_heart->speed_y;
    // Give preference to the mid-air position
    sprite_heart->pos_y = MIN(sprite_heart->pos_y, FLOOR_Y);
}

/**
* Change the animation frame of the heart.
* It change every 5 frames.
*/
void sprite_heart_change_animation(Heart* sprite_heart)
{
    OBJ_ATTR* sprite_attrs = sprite_heart->sprite_attrs;
    // Advance a frame
    sprite_heart->framesCurrent++;
    if(sprite_heart->framesCurrent%5 == 0){
        sprite_heart->anim_frame++;
        sprite_heart->framesCurrent = 0;
    }
    // Cycle through the 6 moving frames
    sprite_heart->anim_frame = sprite_heart->anim_frame % 6;
    // Set the current sprite frame as base pixel + current frame pixel offset
	sprite_attrs->attr2= ATTR2_BUILD(sprite_heart->first_anim_frame, 0, 0); 
}

/**
* Check if exists a collision between sprites.
* Return true or false if there is a collision
*/
int do_heart_collision(Heart* sprite_heart, Sprite * sprite, Coin* coin){    
    Rect heart;
    Rect character;
    rect_init(&heart);
    rect_init(&character);
    rect_set_coords16(&heart, sprite_heart->pos_x, sprite_heart->pos_y);
    rect_set_coords16(&character, sprite->pos_x, sprite->pos_y);
    if(rect_intersects(&heart, &character, sprite->speed_x, sprite->speed_y)){
        // There is a overlap
        if(!sprite_heart->hidden){ // If the heart is not hidden
            sprite->lives++;
            obj_hide(sprite_heart->sprite_attrs);
            sprite_heart->hidden = 1;
        }
    }
    return 1;
}

/**
* Method to unhide the heart, check the total jumps that the sprite has made.
* Verify if the count of jumps is the needed to put another heart.
*/
int sprite_heart_unhide(Heart* sprite_heart, Sprite * sprite){
    if(sprite_heart->hidden){ // If the heart is hidden
        if(sprite->jumpsHeart >= 4){ // Condition to put another heart
            sprite_heart->hidden = 0;
            sprite_heart->pos_x = (rand() % (200 - 17)) + 17;
            sprite_heart->pos_y = (rand() % (124 - 17)) + 17;
            sprite_heart_check_pos(sprite_heart, sprite);
            sprite_heart_update_pos(sprite_heart);
            obj_unhide(sprite_heart->sprite_attrs, 0 );
            // Reset the count
            sprite->jumpsHeart = 0;
            // Count reached
            return 1;
        }
    }else if(sprite->jumpsHeart >= 4){
        // Reset the count
        sprite->jumpsHeart = 0;
    }
    // Count not reached
    return 0;
}

/**
* This method check if where the heart is going to be placed exists the sprite.
*/
void sprite_heart_check_pos(Heart* sprite_heart, Sprite* sprite){
    Rect heart;
    Rect character;
    rect_init(&heart);
    rect_init(&character);
    rect_set_coords(&heart, sprite_heart->pos_x-20, sprite_heart->pos_y-20, sprite_heart->pos_x+20, sprite_heart->pos_y+20);
    rect_set_coords(&character, sprite->pos_x-20, sprite->pos_y-20, sprite->pos_x+20, sprite->pos_y+20);
    if(rect_intersects(&heart, &character, sprite->speed_x, sprite->speed_y)){
        if(sprite_heart->pos_x < 120){ // Left side of screen
            sprite_heart->pos_x = sprite_heart->pos_x + 60;
        }else{ // Right side of screen
            sprite_heart->pos_x = sprite_heart->pos_x - 60;
        }
    }
}

void heart_hide(Heart* sprite_heart){
    obj_hide(sprite_heart->sprite_attrs);
    sprite_heart->hidden = 1;
}