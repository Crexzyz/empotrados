#ifndef MENU_H
#define MENU_H

#include <tonc.h>

#define POS0 (80<<8)
#define GRAV 0x40
#define DAMP 0xD0
#define HWLEN 12
#define tittle "Tiquicia Jump"
#define instr "Presione A para comenzar"



typedef struct
{
    u32 state;
    int tt;
    FIXED fy;
    FIXED fvy;
} PATTERN;

void pat_bounce(PATTERN *pat);
void title_init(PATTERN* pats, OBJ_ATTR *oe);
void print_instructions();


#endif