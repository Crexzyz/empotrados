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

// Function that makes the bounce of the letters in the tittle screen
void pat_bounce(PATTERN *pat);
// Function that sets the tittle screen of the game
void title_init(PATTERN* pats, OBJ_ATTR *oe);
// Function that sets the instructions screen
void print_instructions(OBJ_ATTR *oe);
// Function that sets the end game screen
void final_screen(OBJ_ATTR *oam, int currentScore, u32 sprites_amount);
// Function that sets the second level screen
void second_level_transition(OBJ_ATTR *oam, u32 sprites_amount);

#endif