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
    REG_TM3CNT = 0;
    REG_TM3CNT = TM_ENABLE | TM_CASCADE;
    REG_TM3D = 0;
}

void init_stop_watch(StopWatch* stopWatch){
    stopWatch->newSeconds = 0;
    stopWatch->newMinutes = 0;
    stopWatch->newHours = 0;
    stopWatch->lastLap = 0;
    stopWatch->maxLap = 1;
    stopWatch->start = false;
}

void start_stop_watch(StopWatch* stopWatch){
    stopWatch->start = true;
}

void lap_stop_watch(StopWatch* stopWatch){
    Laps lap;
    lap.seconds = stopWatch->newSeconds;
    lap.minutes = stopWatch->newMinutes;
    lap.hours = stopWatch->newHours;
    lap.lap = stopWatch->maxLap;
    stopWatch->laps[stopWatch->lastLap] = lap;
    
    stopWatch->lastLap= (stopWatch->lastLap+1)%MAX_LAPS;
    ++stopWatch->maxLap;

    stopWatch->newMinutes = 0;
    stopWatch->newHours = 0;
    stopWatch->newSeconds = 0;

    show_laps(stopWatch);
    restart_clock();
}

void update_stop_watch(StopWatch* stopWatch){
    if(REG_TM3D != stopWatch->newSeconds){ // Time increment
        if(stopWatch->start){
            ++stopWatch->newSeconds; // Set seconds the data of the register
            stopWatch->newMinutes = (stopWatch->newSeconds%3600)/60;
            stopWatch->newHours = stopWatch->newSeconds/3600;
            stopWatch->newSeconds = stopWatch->newSeconds%60;
        }

        char buf[50];
        snprintf(buf, 50, "#{P:10,40} %02d:%02d:%02d",
				stopWatch->newHours, stopWatch->newMinutes, stopWatch->newSeconds);
        tte_write(buf);
    }
}

void show_laps(StopWatch* stopWatch){
    int pos_y = 30;
    int pos_x = 85;
    int limit = stopWatch->lastLap;
    if(stopWatch->lastLap == 0)
        limit = 12;

    char buf[40];

    for(int index = 0; index < limit; ++index){
        snprintf(buf, 40, "#{P:%d,%d} Lap %d: %02d:%02d:%02d",
				pos_x, pos_y, stopWatch->laps[index].lap, stopWatch->laps[index].hours, 
                stopWatch->laps[index].minutes, stopWatch->laps[index].seconds);
        tte_write(buf);
        pos_y += 10;
    }
}