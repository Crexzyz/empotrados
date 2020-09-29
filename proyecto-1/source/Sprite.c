#include <string.h>

#include "Sprite.h"
#include "charsprites.h"

void sprite_load_to_mem()
{
    // Move sprite image to memory
	memcpy(&tile_mem[4][0], charspritesTiles, charspritesTilesLen);
	// Add sprite palette
	memcpy(pal_obj_mem, charspritesPal, charspritesPalLen);

}

void sprite_init(Sprite * sprite, OBJ_ATTR * attribs)
{
    sprite->sprite_attrs = attribs;
    sprite->facing_right = 1;
    sprite->first_anim_frame = STILL1;
    sprite->anim_frame = 0;
    sprite->pos_x = 0;
    sprite->pos_y = FLOOR_Y;
    sprite->speed_x = 0;
    sprite->speed_y = 0;
    sprite->frames_in_air = 0;
}

void sprite_update_position(Sprite * sprite)
{
    sprite_update_x_pos(sprite);
    sprite_update_y_pos(sprite);

    // Set the sprite the 8 bits per pixel and square sprite bits
	sprite->sprite_attrs->attr0 = ATTR0_8BPP | ATTR0_SQUARE ;
    // Set the sprite as a 16x16 pixel sprite
	sprite->sprite_attrs->attr1 = ATTR1_SIZE_16;
    // If the sprite is facing left, flip it
	sprite->sprite_attrs->attr1 |= (sprite->facing_right ? 0 : ATTR1_HFLIP);
    // Draw the sprite
	obj_set_pos(sprite->sprite_attrs, sprite->pos_x, sprite->pos_y);
}

void sprite_update_x_pos(Sprite * sprite)
{
    if (key_is_down(KEY_LEFT))
    {
        sprite->facing_right = 0;
        sprite->speed_x = -WALK_SPEED;
    }
    else if (key_is_down(KEY_RIGHT))
    {
        sprite->facing_right = 1;
        sprite->speed_x = WALK_SPEED;
    }
    else 
        sprite->speed_x = 0;

    sprite->pos_x += sprite->speed_x;

    // Right horizontal limit
    sprite->pos_x = MIN(240-16, sprite->pos_x);
    // Left horizontal limit
    sprite->pos_x = MAX(0, sprite->pos_x);
}

void sprite_update_y_pos(Sprite * sprite)
{
    if (key_hit(KEY_A) && sprite_get_state(sprite) != JUMPING)
    {
        sprite->speed_y = JUMP_THRESHOLD;
        sprite->frames_in_air = 0;
    }

    if (sprite_get_state(sprite) == JUMPING)
    {
        // Move the sprite up decreasingly
        sprite->speed_y = JUMP_THRESHOLD + (GRAVITY_FORCE * sprite->frames_in_air);
        // Move the sprite down 
        sprite->speed_y = MIN(FALL_MAX_SPEED, sprite->speed_y);
        sprite->frames_in_air++;
    }

    // Move the sprite up or down
    sprite->pos_y += sprite->speed_y;

    // Give preference to the mid-air position
    sprite->pos_y = MIN(sprite->pos_y, FLOOR_Y);
}

void sprite_change_animation(Sprite * sprite)
{
    if (sprite_get_state(sprite) == JUMPING)
    {
        // Set the looking-up frame as base
        sprite->first_anim_frame = SPRITE_8PIXEL_OFFSET(JUMP_UP);
        // If going down, set the looking down frame, otherwise the looking up frame
        sprite->anim_frame = sprite->speed_y > 0 ? 1 : 0;
    }
    else
    {
        // Advance a frame
		sprite->anim_frame++;
        // Set the base frame as the first moving frame or move to the first still frame
        sprite->first_anim_frame = sprite_get_state(sprite) == MOVING ? SPRITE_8PIXEL_OFFSET(MOVE_H1) : SPRITE_8PIXEL_OFFSET(STILL1);
        // Cycle through the 3 moving frames or the 4 still frames
        sprite->anim_frame = sprite_get_state(sprite) == MOVING ? (sprite->anim_frame) % 3 : (sprite->anim_frame) % 4;
    }

    // Set the current sprite frame as base pixel + current frame pixel offset
	sprite->sprite_attrs->attr2= ATTR2_BUILD(sprite->first_anim_frame + SPRITE_8PIXEL_OFFSET(sprite->anim_frame), 0, 0); 
}

int sprite_get_state(Sprite * sprite)
{
    // Give priority to the jumping state
    if (sprite->pos_y != FLOOR_Y)
        return JUMPING;
    else if(sprite->speed_x != 0)
        return MOVING;
    else 
        return STILL;
}

void sprite_map_update_position(Map * map, Sprite * sprite)
{
    sprite_map_update_x_pos(map, sprite);


    // Set the sprite the 8 bits per pixel and square sprite bits
	sprite->sprite_attrs->attr0 = ATTR0_8BPP | ATTR0_SQUARE ;
    // Set the sprite as a 16x16 pixel sprite
	sprite->sprite_attrs->attr1 = ATTR1_SIZE_16;
    // If the sprite is facing left, flip it
	sprite->sprite_attrs->attr1 |= (sprite->facing_right ? 0 : ATTR1_HFLIP);
    // Draw the sprite
	obj_set_pos(sprite->sprite_attrs, sprite->pos_x, sprite->pos_y);
}

void sprite_map_update_x_pos(Map * map, Sprite * sprite)
{
    if (key_is_down(KEY_LEFT))
    {
        sprite->facing_right = 0;
        sprite->speed_x = -WALK_SPEED;
    }
    else if (key_is_down(KEY_RIGHT))
    {
        sprite->facing_right = 1;
        sprite->speed_x = WALK_SPEED;
    }
    else 
        sprite->speed_x = 0;

    if(map_get_tile_type(map, sprite->pos_x + sprite->speed_x, sprite->pos_y) == AIR)
        sprite->pos_x += sprite->speed_x;
}