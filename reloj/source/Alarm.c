#include "Alarm.h"

bool alarm_raised = false;
// bool alarm_irq_set = false;

void alarm_set_irq()
{
    irq_add(II_TIMER1, alarm_handle);
}

void alarm_set(u16 time)
{
    // Create a 1Hz/1s timer that cascades onto timer 1
    REG_TM0D = -0x4000;
    REG_TM0CNT = TM_ENABLE | TM_FREQ_1024;

    // Set the time that it will wait before raising an interrupt
    REG_TM1CNT ^= TM_ENABLE;
    REG_TM1D = (u16)(0 - time);
    REG_TM1CNT = TM_ENABLE | TM_CASCADE;
    irq_add(II_TIMER1, alarm_handle);
}

void alarm_handle()
{
    // Raise the flag
    alarm_raised = true;

    // Disable the alarm
    REG_TM0CNT ^= TM_ENABLE;
    irq_delete(II_TIMER1);
}