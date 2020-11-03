#ifndef GAME_CONTROLLER_H
#define GAME_CONTROLLER_H

#define REG_BG1_SCROLL_H       *((volatile u16*)(0x04000014))
#define REG_BG1_SCROLL_V       *((volatile u16*)(0x04000016))

void gamectrl_init_regs();
int gamectrl_run();

#endif