#include "Clock.h"

void initClock(Clock* clock){
    irq_init(NULL);
    REG_TM2D = -0x4000; // Init in value: 0xFFFFC000 - Have 3FFF space to clock until overflow
    REG_TM2CNT = TM_FREQ_1024 | TM_ENABLE; // frequency 1024 cycles of CPU makes overflow
    REG_TM3CNT = TM_ENABLE | TM_CASCADE; // Have the seconds

    clock->seconds = TIME_OVERFLOW;
    clock->minutes = 0;
    clock->hours = 0;
}

void updateClock(Clock* clock){

    if(REG_TM3D != clock->seconds){ // Time increment
        clock->seconds = REG_TM3D; // Set seconds the data of the register
        clock->minutes = (clock->seconds%3600)/60;
        clock->hours = clock->seconds/3600;
        clock->seconds = clock->seconds%60;
    }
}
