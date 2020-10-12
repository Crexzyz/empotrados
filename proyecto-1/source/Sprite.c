#include <string.h>

#include "Sprite.h"
#include "charsprites.h"
#include "music.h"

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
    sprite->jumps = 0;
}

void sprite_paint(Sprite * sprite)
{
    // Set the sprite the 8 bits per pixel and square sprite bits
	sprite->sprite_attrs->attr0 = ATTR0_8BPP | ATTR0_SQUARE ;
    // Set the sprite as a 16x16 pixel sprite
	sprite->sprite_attrs->attr1 = ATTR1_SIZE_16;
    // If the sprite is facing left, flip it
	sprite->sprite_attrs->attr1 |= (sprite->facing_right ? 0 : ATTR1_HFLIP);
    // Draw the sprite
	obj_set_pos(sprite->sprite_attrs, sprite->pos_x, sprite->pos_y);
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
    if (sprite->speed_y != 0 || sprite->frames_in_air != 0)
        return JUMPING;
    else if(sprite->speed_x != 0)
        return MOVING;
    else 
        return STILL;
}

void sprite_update_pos_collision(Sprite * sprite, Rect ** rects, size_t rects_amount)
{
    // Check the buttons and update the speed values
    sprite_update_x_pos_speed(sprite);
    sprite_update_y_pos_speed(sprite);
    // Check for collisions
    sprite_update_xy_collision(sprite, rects, rects_amount);
    // Paint the sprite on screen
    sprite_paint(sprite);
}

void sprite_update_x_pos_speed(Sprite * sprite)
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
}

void sprite_update_y_pos_speed(Sprite * sprite)
{
    if (key_hit(KEY_A) && sprite_get_state(sprite) != JUMPING)
    {
        sprite->speed_y = VELOCITY;
        music_note_play(NOTE_B, 0);
        sprite->frames_in_air = 0;

        // Update the count pixels jumped
        sprite->jumps++;
    }
    
    if (sprite_get_state(sprite) == JUMPING)
    {
        // Move the sprite up decreasingly
        sprite->speed_y = VELOCITY + (sprite->frames_in_air * ACCELERATION);
        // Move the sprite down 
        sprite->speed_y = MIN(FALL_MAX_SPEED, sprite->speed_y);
        sprite->frames_in_air++;
    }
}

void sprite_update_xy_collision(Sprite * sprite, Rect ** rects, size_t rects_amount)
{
    // Create a square based on the sprite's position
    Rect sprite_rect;
    rect_init(&sprite_rect);
    rect_set_coords16(&sprite_rect, sprite->pos_x, sprite->pos_y);

    int intersect_exists = 0;
    size_t intersect_rect = 0;

    // Look for the first rect that collides with the sprite and save the index
    for(size_t rect = 0; rect < rects_amount; ++rect)
    {
        if(rect_intersects(&sprite_rect, &(*rects)[rect], sprite->speed_x, sprite->speed_y))
        {
            intersect_exists = 1;        
            intersect_rect = rect;
            break;
        }
    }

    if(intersect_exists)
    {
        // Stop the movement on the X coordinate if there is going to be a collision
        if(rect_intersects(&sprite_rect, &(*rects)[intersect_rect], sprite->speed_x, 0))
        {
            if(sprite->speed_x < 0) // Moving left, place the sprite on the rect's right
                sprite->pos_x = (*rects)[intersect_rect].x2+1;
            else if (sprite->speed_x > 0) // Moving right, place the sprite on the rect's left
                sprite->pos_x = (*rects)[intersect_rect].x1-16;

            // Stop the sprite
            sprite->speed_x = 0;
        }

        // Stop the movement on the Y coordinate if there is going to be a collision
        if(rect_intersects(&sprite_rect, &(*rects)[intersect_rect], 0, sprite->speed_y))
        {
            if(sprite->speed_y < 0) // Jumping up, place the sprite on the rect's bottom
            {
                sprite->pos_y = (*rects)[intersect_rect].y2+1;
            }
            else if (sprite->speed_y > 0) // Jumping down, place the sprite on the rect's top
            {
                sprite->pos_y = (*rects)[intersect_rect].y1-16;

                // Stop the sprite
                sprite->speed_y = 0;
                sprite->frames_in_air = 0;
            } 
        }
    }

    if(!intersect_exists)
    {
        int above_block = 0;
        for(size_t rect = 0; rect < rects_amount; ++rect)
        {
            // If the sprite is above any block in the map
            if(rect_intersects(&sprite_rect, &(*rects)[rect], 0, 1))
            {
                above_block = 1;        
                break;
            }
        }
        
        // Check if the sprite is no more over a block to start making it fall down
        if(!above_block && sprite->speed_y == 0)
            sprite->frames_in_air = (1 - VELOCITY) / ACCELERATION; // 9
    }
    

    // Teleport player to the other side if out of bounds
    if(sprite->pos_x + sprite->speed_x > 240)
        sprite->pos_x = 0;
    else if (sprite->pos_x + 15 + sprite->speed_x < 0)
        sprite->pos_x = 240;
    else
        sprite->pos_x += sprite->speed_x;
        
    // Move the sprite up or down
    sprite->pos_y += sprite->speed_y;
}

void sprite_place_on_rect(Sprite * sprite, Rect * rect)
{
    sprite->pos_x = rect->x1;
    sprite->pos_y = rect->y1-16;
}