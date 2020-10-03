#include "Rect.h"
#include "Sprite.h"

void rect_init(Rect * rect)
{
    rect->x1 = 0;
    rect->x2 = 0;
    rect->y1 = 0;
    rect->y2 = 0;
    rect->sprite_attrs = 0;
}

void rect_set_sprite(Rect * rect, OBJ_ATTR * attrs)
{
    rect->sprite_attrs = attrs;
}

void rect_set_coords(Rect * rect, u32 x1, u32 y1, u32 x2, u32 y2)
{
    rect->x1 = x1;
    rect->x2 = x2;
    rect->y1 = y1;
    rect->y2 = y2;
}

void rect_set_coords16(Rect * rect, u32 x1, u32 y1)
{
    rect->x1 = x1;
    rect->x2 = x1 + 15;
    rect->y1 = y1;
    rect->y2 = y1 + 15;
}

int rect_intersects(Rect * rect1, Rect * rect2, s32 speed_x, s32 speed_y)
{
    return (rect1->x1 + speed_x < rect2->x2 && rect1->x2 + speed_x > rect2->x1 &&
            rect1->y1 + speed_y < rect2->y2 && rect1->y2 + speed_y > rect2->y1);
}

void rect_paint(Rect * rect)
{
    rect->sprite_attrs->attr2= ATTR2_BUILD(SPRITE_8PIXEL_OFFSET(9), 0, 0); 
    // Set the sprite the 8 bits per pixel and square sprite bits
	rect->sprite_attrs->attr0 = ATTR0_8BPP | ATTR0_SQUARE ;
    // Set the sprite as a 16x16 pixel sprite
	rect->sprite_attrs->attr1 = ATTR1_SIZE_16;
    // Draw the sprite
	obj_set_pos(rect->sprite_attrs, rect->x1, rect->y1);
}