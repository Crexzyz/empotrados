#include "StopWatch.h"

void init_stop_watch(StopWatch* stopWatch){
    stopWatch->lastLap = 0;
    stopWatch->maxLap = 1;
    stopWatch->totalSeconds = 0;
    stopWatch->initTime = 0;
    stopWatch->start = false;
}

void start_stop_watch(Params* data){
    data->stopWatch->start = true;

    init_clock(data->clock);
    // update_clock(data->clock);
    
    data->stopWatch->totalSeconds = REG_TM3D;
    data->stopWatch->initTime = REG_TM3D;
}

void lap_stop_watch(Params* data){
    if(data->stopWatch->start){
        int newSeconds = REG_TM3D - data->stopWatch->totalSeconds;
        data->stopWatch->totalSeconds = REG_TM3D;
        Laps lap;
        lap.seconds = newSeconds%60;
        lap.minutes = (newSeconds%3600)/60;
        lap.hours = newSeconds/3600;
        lap.lap = data->stopWatch->maxLap;

        data->stopWatch->laps[data->stopWatch->lastLap] = lap;
        data->stopWatch->lastLap= (data->stopWatch->lastLap+1)%MAX_LAPS;
        ++data->stopWatch->maxLap;

        show_laps(data->stopWatch);
    }
}

void update_stop_watch(StopWatch* stopWatch, Clock* clock){
    int newSeconds = 0;
    char buf[50];
    if(REG_TM3D != clock->seconds && stopWatch->start){ // Time increment
        newSeconds = REG_TM3D - stopWatch->initTime;
        clock->seconds = newSeconds%60;
        clock->minutes = (newSeconds%3600)/60;
        clock->hours = newSeconds/3600;
    }
    if(stopWatch->start){
        
        snprintf(buf, 50, "#{P:10,30} Timer:");
        tte_write(buf);
        snprintf(buf, 50, "#{P:10,40} %02d:%02d:%02d",
            clock->hours, clock->minutes, clock->seconds);
        tte_write(buf);
    }else{
        snprintf(buf, 50, "#{P:10,30} Timer:");
        tte_write(buf);
        snprintf(buf, 50, "#{P:10,40} 00:00:00");
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

void reset_stop_watch(StopWatch* stopWatch){
    stopWatch->start = false;
    stopWatch->lastLap = 0;
    stopWatch->maxLap = 1;
    
    stopWatch->totalSeconds = REG_TM3D;
    stopWatch->initTime = REG_TM3D;

    int pos_y = 30;
    int pos_x = 85;
    int limit = 12;
    char buf[40];

    for(int index = 0; index < limit; ++index){
        
        snprintf(buf,40,"#{P:%d,%d}                      ", pos_x , pos_y);
        tte_write(buf);
        pos_y += 10;
    }
}