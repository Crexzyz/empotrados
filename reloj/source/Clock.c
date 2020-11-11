#include "Clock.h"

void init_clock(Clock* clock){
    irq_init(NULL);
    REG_TM2D = -0x4000; // Init in value: 0xFFFFC000 - Have 3FFF space to clock until overflow
    REG_TM2CNT = TM_FREQ_1024 | TM_ENABLE; // frequency 1024 cycles of CPU makes overflow
    REG_TM3CNT = TM_ENABLE | TM_CASCADE; // Have the seconds

    clock->seconds = TIME_OVERFLOW;
    clock->minutes = 0;
    clock->hours = 0;
}

void update_clock(Clock* clock){
    if(REG_TM3D != clock->seconds){ // Time increment
        clock->seconds = REG_TM3D; // Set seconds the data of the register
        clock->minutes = (clock->seconds%3600)/60;
        clock->hours = clock->seconds/3600;
        clock->seconds = clock->seconds%60;
    }
}

void restart_clock(){
    REG_TM2CNT = 0;
    REG_TM2CNT = TM_FREQ_1024 | TM_ENABLE;
    REG_TM2D = -0x4000;

    REG_TM3CNT = 0;
    REG_TM3CNT = TM_ENABLE | TM_CASCADE;
    REG_TM3D = 0;
}