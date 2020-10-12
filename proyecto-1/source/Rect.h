#ifndef RECT_H
#define RECT_H

#include <tonc.h>

typedef struct _Rect {
    // X coordinate on the top left corner
    u32 x1;
    // X coordinate on the bottom right corner
    u32 x2;
    // Y coordinate on the top left corner
    u32 y1;
    // Y coordinate on the bottom right corner
    u32 y2;
    // Optional sprite for the rect
    OBJ_ATTR * sprite_attrs;
} Rect;

// Sets a 0 to every value
void rect_init(Rect * rect);
// Sets the dimensions of the rect, custom size
void rect_set_coords(Rect * rect, u32 x1, u32 y1, u32 x2, u32 y2);
// Creates a 16x16 square on the x,y coordinate
void rect_set_coords16(Rect * rect, u32 x1, u32 y1);
// Sets the optional sprite to the rect
void rect_set_sprite(Rect * rect, OBJ_ATTR * attrs);
// Returns true if a rect intersects another one (collision)
int rect_intersects(Rect * rect1, Rect * rect2, s32 speed_x, s32 speed_y);
// Returns true if a rect intersects another one (collision) only in the X coordinate
int rect_intersects_x(Rect * rect1, Rect * rect2);
// Paints the rect's sprite on screen
void rect_paint(Rect * rect);

#endif