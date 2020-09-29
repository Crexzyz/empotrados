#include "Rect.h"

void rect_init(Rect * rect)
{
    rect->x1 = 0;
    rect->x2 = 0;
    rect->y1 = 0;
    rect->y2 = 0;
}

void rect_init_data(Rect * rect, u32 x1, u32 y1, u32 x2, u32 y2)
{
    rect->x1 = x1;
    rect->x2 = x2;
    rect->y1 = y1;
    rect->y2 = y2;
}

int rect_intersects_x(Rect * rect1, Rect * rect2, s32 speed)
{
    return (rect1->x2 + speed >= rect2->x1 && rect1->x2 + speed <= rect2->x2) ||
            (rect1->x1 + speed >= rect2->x1 && rect1->x1 + speed <= rect2->x2);
}