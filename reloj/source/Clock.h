#ifndef CLOCK_H
#define CLOCK_H

#include <tonc.h>
#include "NumberPrinter.h"
#define TIME_OVERFLOW -1
#define MAX_LAPS 12 // Laps that can be shown in screen

typedef struct _Clock { // Normal clock
    u32 seconds;
    u32 minutes;
    u32 hours;
} Clock;

typedef struct _Laps { // Laps for chronometer
    u32 seconds;
    u32 minutes;
    u32 hours;
    int lap;
} Laps;

typedef struct _StopWatch { // chronometer
    u32 newSeconds;
    u32 newMinutes;
    u32 newHours;
    Laps laps[MAX_LAPS];
    int lastLap;
    int maxLap;
    int minLap;
    bool start;
} StopWatch;

/**
 * Init the registers for start value, frequency and secnds counter
 * Also init the values for the struct
 * @param clock struct that contains the values for clock
 */
void init_clock(Clock* clock);

/**
 * Update the seconds, minutes and hours of the clock
 * @param clock struct to be updated
 */
void update_clock(Clock* clock);

/**
 * Restart the clock timers
 */
void restart_clock();

void init_stop_watch(StopWatch* stopWatch);

void start_stop_watch(StopWatch* stopWatch);

void lap_stop_watch(StopWatch* stopWatch);

void update_stop_watch(StopWatch* stopWatch);

void show_laps(StopWatch* stopWatch);

#endif