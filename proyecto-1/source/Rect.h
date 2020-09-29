#ifndef RECT_H
#define RECT_H

#include <tonc.h>

typedef struct _Rect {
    u32 x1;
    u32 x2;
    u32 y1;
    u32 y2;
} Rect;

void rect_init(Rect * rect);
void rect_init_data(Rect * rect, u32 x1, u32 y1, u32 x2, u32 y2);
int rect_intersects_x(Rect * rect1, Rect * rect2, s32 speed);

#endif