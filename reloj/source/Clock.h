#ifndef CLOCK_H
#define CLOCK_H

#include <tonc.h>
#include "NumberPrinter.h"
#define TIME_OVERFLOW -1

typedef struct Clock {
    u32 seconds;
    u32 minutes;
    u32 hours;
} Clock;

/**
 * Init the registers for start value, frequency and secnds counter
 * Also init the values for the struct
 * @param clock struct that contains the values for clock
 */
void initClock(Clock* clock);

/**
 * Update the seconds, minutes and hours of the clock
 * @param clock struct to be updated
 */
void updateClock(Clock* clock);


#endif