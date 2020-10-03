#include "CoinTile.h"


void sprite_coin_init(Coin* sprite_coin, OBJ_ATTR * attribs){
    sprite_coin->sprite_attrs = attribs;
    sprite_coin->first_anim_frame = 10 * 8; 
    sprite_coin->anim_frame = 10 * 8;
    sprite_coin->pos_x = 66; 
    sprite_coin->pos_y = 80; 
    sprite_coin->speed_y = 5;
    sprite_coin->framesCurrent = 0;
    sprite_coin->currentScore = 0;
    sprite_coin->hidden = 0;
}


void sprite_coin_paint(Coin* sprite_coin)
{
    // Set the sprite the 8 bits per pixel and square sprite bits
	sprite_coin->sprite_attrs->attr0 = ATTR0_8BPP | ATTR0_SQUARE ;
    // Set the sprite as a 16x16 pixel sprite
	sprite_coin->sprite_attrs->attr1 = ATTR1_SIZE_16;
    // Draw the sprite
	obj_set_pos(sprite_coin->sprite_attrs, sprite_coin->pos_x, sprite_coin->pos_y);
}

void sprite_coin_update_pos(Coin* sprite_coin)
{
    sprite_coin_update_y_pos(sprite_coin);

    if(!sprite_coin->hidden) // If the coin is not hidden
        sprite_coin_paint(sprite_coin); // Paint the coin in screen
}

void sprite_coin_update_y_pos(Coin* sprite_coin)
{
    //sprite_update_x_pos_speed(sprite);

    // Move the sprite up or down
    //sprite_coin->pos_y += sprite_coin->speed_y;

    // Give preference to the mid-air position
    sprite_coin->pos_y = MIN(sprite_coin->pos_y, FLOOR_Y);
}

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