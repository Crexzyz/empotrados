#ifndef ALARM_H
#define ALARM_H

#include "DisplayController.h"
#include "stdbool.h"

extern bool alarm_raised;

/**
 * Sets an alarm for some time in seconds
 * @param time The time the alarm will wait in seconds
 */
void alarm_set(u16 time);

/**
 * Handles the alarm interrupt when it raises
 */
void alarm_handle();

void alarm_set_irq();


#endif