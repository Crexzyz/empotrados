#ifndef STOPWATCH_H
#define STOPWATCH_H

#include <tonc.h>
#include "Clock.h"

#define MAX_LAPS 12 // Laps that can be shown in screen

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

typedef struct _Params { // chronometer
    Clock* clock;
    StopWatch* stopWatch;
} Params;

/**
 * Init the values for the struct
 * @param stopWatch struct that contains the values and record for clock and laps
 */
void init_stop_watch(StopWatch* stopWatch);

/**
 * Start the stop watch and the timer of the clock
 * @param data struct that contains the pointer to clock and stopWatch
 */
void start_stop_watch(Params* data);

/**
 * Register a lap
 * @param stopWatch struct that contains the record of the laps
 */
void lap_stop_watch(StopWatch* stopWatch);

/**
 * Update the hours, minutes and seconds
 * @param stopWatch struct that contains the record of the laps
 */
void update_stop_watch(StopWatch* stopWatch);

/**
 * Show the laps recorded
 * @param stopWatch struct that contains the record of the laps
 */
void show_laps(StopWatch* stopWatch);

#endif