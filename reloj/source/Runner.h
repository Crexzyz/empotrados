#ifndef RUNNER_H
#define RUNNER_H

#include "DisplayController.h"


#define SPRITE_BUFFER_SIZE 8 // "00:00:00"
#define OPTIONS_STOPWATCH 4

void run_init();

void run_clock(DispCtrl * dc);
void run_stopwatch(DispCtrl * dc);
void run_alarm(DispCtrl * dc);
void run_nprinter_test(DispCtrl * dc);

#endif