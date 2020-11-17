#include "Enemy.h"

/** 
* Method for the normal init
*/
void sprite_enemy_init(Enemy* sprite_enemy, OBJ_ATTR * attribs, int index){
    sprite_enemy->sprite_attrs = attribs;
    sprite_enemy->first_anim_frame = index * 8;
    sprite_enemy->anim_frame = index * 8;
    sprite_enemy->pos_x = (rand() % (200 - 17)) + 17;
    sprite_enemy->pos_y = (rand() % (124 - 17)) + 17;
    sprite_enemy->speed_y = 5;
    sprite_enemy->framesCurrent = 0;
    sprite_enemy->currentScore = 0;
    sprite_enemy->hidden = 0;
}

/** 
* Method that check if exists collision between the enemy and the sprite
*/
void sprite_enemy_init_with_colis(Enemy* sprite_enemy, OBJ_ATTR * attribs, Sprite* sprite, int index){
    sprite_enemy->sprite_attrs = attribs;
    sprite_enemy->first_anim_frame = index * 8;
    sprite_enemy->anim_frame = index * 8;
    sprite_enemy->pos_x = (rand() % (200 - 17)) + 17;
    sprite_enemy->pos_y = (rand() % (124 - 17)) + 17;
    sprite_enemy_check_pos(sprite_enemy, sprite);
    sprite_enemy->speed_y = 5;
    sprite_enemy->framesCurrent = 0;
    sprite_enemy->currentScore = 0;
    sprite_enemy->hidden = 0;
}

/**
* Paint the enemy in screen.
*/
void sprite_enemy_paint(Enemy* sprite_enemy)
{
    // Set the sprite the 8 bits per pixel and square sprite bits
	sprite_enemy->sprite_attrs->attr0 = ATTR0_8BPP | ATTR0_SQUARE ;
    // Set the sprite as a 16x16 pixel sprite
	sprite_enemy->sprite_attrs->attr1 = ATTR1_SIZE_16;
    // Draw the sprite
	obj_set_pos(sprite_enemy->sprite_attrs, sprite_enemy->pos_x, sprite_enemy->pos_y);
}

/**
* Update the position of the enemy.
* If the enemy is hidden, nothing is done.
* Else paint the enemy in screen.
*/
void sprite_enemy_update_pos(Enemy* sprite_enemy)
{
    sprite_enemy_update_y_pos(sprite_enemy);
    if(!sprite_enemy->hidden) // If the enemy is not hidden
        sprite_enemy_paint(sprite_enemy); // Paint the enemy in screen
}

void sprite_enemy_change_pos(Enemy* sprite_enemy)
{
    sprite_enemy->pos_x = (rand() % (200 - 17)) + 17;
    sprite_enemy->pos_y = (rand() % (124 - 17)) + 17;
}

/**
* Update the "y" position of the enemy.
*/
void sprite_enemy_update_y_pos(Enemy* sprite_enemy)
{
    // Move the sprite up or down
    //sprite_enemy->pos_y += sprite_enemy->speed_y;
    // Give preference to the mid-air position
    sprite_enemy->pos_y = MIN(sprite_enemy->pos_y, FLOOR_Y);
}

/**
* Change the animation frame of the enemy.
* It change every 5 frames.
*/
void sprite_enemy_change_animation(Enemy* sprite_enemy)
{
    OBJ_ATTR* sprite_attrs = sprite_enemy->sprite_attrs;
    // Advance a frame
    sprite_enemy->framesCurrent++;
    if(sprite_enemy->framesCurrent%5 == 0){
        sprite_enemy->anim_frame++;
        sprite_enemy->framesCurrent = 0;
    }
    // Cycle through the 6 moving frames
    sprite_enemy->anim_frame = sprite_enemy->anim_frame % 6;
    // Set the current sprite frame as base pixel + current frame pixel offset
	sprite_attrs->attr2= ATTR2_BUILD(sprite_enemy->first_anim_frame, 0, 0); 
}

/**
* Check if exists a collision between sprites.
* Return true or false if there is a collision
*/
int do_enemy_collision(Enemy* sprite_enemy, Sprite * sprite, Coin* coin){    
    Rect enemy;
    Rect character;
    rect_init(&enemy);
    rect_init(&character);
    rect_set_coords16(&enemy, sprite_enemy->pos_x, sprite_enemy->pos_y);
    rect_set_coords16(&character, sprite->pos_x, sprite->pos_y);
    if(rect_intersects(&enemy, &character, sprite->speed_x, sprite->speed_y)){
        // There is a overlap
        if(!sprite_enemy->hidden){ // If the enemy is not hidden
            sprite->lives--;
            obj_hide(sprite_enemy->sprite_attrs);
            sprite_enemy->hidden = 1;
        }
    }
    return 1;
}

/**
* Method to unhide the enemy, check the total jumps that the sprite has made.
* Verify if the count of jumps is the needed to put another enemy.
*/
int sprite_enemy_unhide(Enemy* sprite_enemy, Sprite * sprite){
    if(sprite_enemy->hidden){ // If the enemy is hidden
        if(sprite->jumpsEnemy >= 4){ // Condition to put another enemy
            sprite_enemy->hidden = 0;
            sprite_enemy->pos_x = (rand() % (200 - 17)) + 17;
            sprite_enemy->pos_y = (rand() % (124 - 17)) + 17;
            sprite_enemy_check_pos(sprite_enemy, sprite);
            sprite_enemy_update_pos(sprite_enemy);
            obj_unhide(sprite_enemy->sprite_attrs, 0 );
            // Reset the count
            sprite->jumpsEnemy = 0;
            // Count reached
            return 1;
        }
    }else if(sprite->jumpsEnemy >= 4){
        // Reset the count
        sprite->jumpsEnemy = 0;
    }
    // Count not reached
    return 0;
}

/**
* This method check if where the enemy is going to be placed exists the sprite.
*/
void sprite_enemy_check_pos(Enemy* sprite_enemy, Sprite* sprite){
    Rect enemy;
    Rect character;
    rect_init(&enemy);
    rect_init(&character);
    rect_set_coords(&enemy, sprite_enemy->pos_x-20, sprite_enemy->pos_y-20, sprite_enemy->pos_x+20, sprite_enemy->pos_y+20);
    rect_set_coords(&character, sprite->pos_x-20, sprite->pos_y-20, sprite->pos_x+20, sprite->pos_y+20);
    if(rect_intersects(&enemy, &character, sprite->speed_x, sprite->speed_y)){
        if(sprite_enemy->pos_x < 120){ // Left side of screen
            sprite_enemy->pos_x = sprite_enemy->pos_x + 60;
        }else{ // Right side of screen
            sprite_enemy->pos_x = sprite_enemy->pos_x - 60;
        }
    }
}

void enemy_hide(Enemy* sprite_enemy){
    obj_hide(sprite_enemy->sprite_attrs);
    sprite_enemy->hidden = 1;
}