#ifndef RECT_H
#define RECT_H

#include <tonc.h>

typedef struct _Rect {
    u32 x1;
    u32 x2;
    u32 y1;
    u32 y2;
    OBJ_ATTR * sprite_attrs;
} Rect;

void rect_init(Rect * rect);
void rect_set_coords(Rect * rect, u32 x1, u32 y1, u32 x2, u32 y2);
void rect_set_coords16(Rect * rect, u32 x1, u32 y1);
void rect_set_sprite(Rect * rect, OBJ_ATTR * attrs);
int rect_intersects(Rect * rect1, Rect * rect2, s32 speed_x, s32 speed_y);
int rect_intersects_x(Rect * rect1, Rect * rect2);
void rect_paint_offset(Rect * rect, u32 x, u32 y);
void rect_paint(Rect * rect);

#endif