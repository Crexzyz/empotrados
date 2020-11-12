#include "TimeChooser.h"

void TimeChooser_init(TimeChooser * tc, NumberPrinter * np, u8 x, u8 y)
{
    tc->np = np; // Must be previously initialized
    tc->start_x = x;
    tc->start_y = y;
    tc->current_number = TIME_CHOOSER_BUF_SIZE - 2;
    tc->editing = false;

    tc->buffer[TIME_CHOOSER_BUF_SIZE - 1] = '\0';
    for(size_t i = 0; i < TIME_CHOOSER_BUF_SIZE - 1; ++i) 
        tc->buffer[i] = '0';
    tc->buffer[TIME_CHOOSER_FIRST_COLON] = ':';
    tc->buffer[TIME_CHOOSER_SECOND_COLON] = ':';
}

void TimeChooser_show(TimeChooser * tc)
{
    if(tc->editing)
    {
        if(key_hit(KEY_A))
        {
            TimeChooser_toggle_edit(tc);

            // Wait till key released
            while(!key_released(KEY_A))
                key_poll();
        }
        else if(key_hit(KEY_B))
        {
            TimeChooser_toggle_edit(tc);
        }
    }
    else
    {
        np_print(tc->np, tc->start_x, tc->start_y, WHITE, tc->buffer, TIME_CHOOSER_BUF_SIZE);
    }
}

void TimeChooser_toggle_edit(TimeChooser * tc)
{
    tc->editing = !tc->editing;
}

int test = 0;

void TimeChooser_handle_alarm()
{
    char buf[30];
    snprintf(buf, 30, "#{P:50,50} ALARM %d", test++);
    tte_write(buf);

    irq_delete(II_TIMER1);
}

void TimeChooser_add_alarm()
{
    REG_TM0D = -0x4000;
    REG_TM0CNT = TM_ENABLE | TM_FREQ_1024;

    REG_TM1D = -1;
    REG_TM1CNT = TM_ENABLE | TM_CASCADE;

    irq_add(II_TIMER1, TimeChooser_handle_alarm);
}

