#ifndef GAME_CONTROLLER_H
#define GAME_CONTROLLER_H

#include <tonc.h>

#define REG_BG1_SCROLL_H       *((volatile u16*)(0x04000014))
#define REG_BG1_SCROLL_V       *((volatile u16*)(0x04000016))

// Player + coin + blocks
#define SPRITES_AMOUNT (1 + 1 + BLOCKS_AMOUNT + 1)

int gamectrl_run();
void gamectrl_init();
void gamectrl_init_regs();
void gamectrl_init_interrupts();
void gamectrl_start();

bool gamectrl_show_main_menu();
void gamectrl_show_first_lvl(char * totalScore, u32 * frame_counter, int * h2Scroll);
void gamectrl_show_second_lvl(char * totalScore, u32 * frame_counter, int * h2Scroll);
#endif