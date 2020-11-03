#ifndef GAME_CONTROLLER_H
#define GAME_CONTROLLER_H

#define REG_BG1_SCROLL_H       *((volatile u16*)(0x04000014))
#define REG_BG1_SCROLL_V       *((volatile u16*)(0x04000016))

// Player + coin + blocks
#define SPRITES_AMOUNT (1 + 1 + BLOCKS_AMOUNT)

void gamectrl_init_regs();
void gamectrl_init_interrupts();
void gamectrl_init();
void gamectrl_start();
int gamectrl_run();

#endif