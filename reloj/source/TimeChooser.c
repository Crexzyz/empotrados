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

bool TimeChooser_show(TimeChooser * tc)
{
    bool save_pressed = false;
    if(tc->editing)
    {
        save_pressed = TimeChooser_edit(tc);
        np_print_highlight(tc->np, tc->start_x, tc->start_y, WHITE, BLACK, tc->current_number, tc->buffer, TIME_CHOOSER_BUF_SIZE);
    }
    else
    {
        np_print(tc->np, tc->start_x, tc->start_y, WHITE, tc->buffer, TIME_CHOOSER_BUF_SIZE);
    }

    return save_pressed;
}

void TimeChooser_toggle_edit(TimeChooser * tc)
{
    tc->editing = !tc->editing;
}

bool TimeChooser_edit(TimeChooser * tc)
{
    bool save_pressed = false;
    if(key_hit(KEY_UP))
    {
        if(IS_MINUTE_LIMIT(tc->current_number))
        {
            if(tc->buffer[tc->current_number] == '5')
                tc->buffer[tc->current_number] = '0';
            else
                ++tc->buffer[tc->current_number];
        }
        else
        {
            if(tc->buffer[tc->current_number] == '9')
                tc->buffer[tc->current_number] = '0';
            else
                ++tc->buffer[tc->current_number];
        }
    }
    else if (key_hit(KEY_DOWN))
    {
        if(IS_MINUTE_LIMIT(tc->current_number))
        {
            if(tc->buffer[tc->current_number] == '0')
                tc->buffer[tc->current_number] = '5';
            else
                --tc->buffer[tc->current_number];
        }
        else
        {
            if(tc->buffer[tc->current_number] == '0')
                tc->buffer[tc->current_number] = '9';
            else
                --tc->buffer[tc->current_number];
        }
        
    }
    else if(key_hit(KEY_LEFT))
    {
        if(tc->current_number - 1 == TIME_CHOOSER_FIRST_COLON || tc->current_number - 1 == TIME_CHOOSER_SECOND_COLON)
            --tc->current_number;
        tc->current_number = tc->current_number == 0 ? TIME_CHOOSER_BUF_SIZE - 2 : tc->current_number - 1;
    }
    else if(key_hit(KEY_RIGHT))
    {
        if(tc->current_number + 1 == TIME_CHOOSER_FIRST_COLON || tc->current_number + 1 == TIME_CHOOSER_SECOND_COLON)
            ++tc->current_number;

        tc->current_number = (tc->current_number + 1) % (TIME_CHOOSER_BUF_SIZE - 1);
    }
    else if(key_hit(KEY_A))
    {
        TimeChooser_toggle_edit(tc);

        // Wait till key released
        while(!key_released(KEY_A))
            key_poll();

        save_pressed = true;
    }

    return save_pressed;
}

u16 TimeChooser_buffer2secs(TimeChooser * tc)
{
    u16 seconds = TIMECHOOSER_CTOI(tc->buffer[TIME_CHOOSER_SECOND_INDEX + 1]);
    seconds += TIMECHOOSER_CTOI(tc->buffer[TIME_CHOOSER_SECOND_INDEX]) * 10;

    u16 minutes = TIMECHOOSER_CTOI(tc->buffer[TIME_CHOOSER_MINUTE_INDEX + 1]);
    minutes += TIMECHOOSER_CTOI(tc->buffer[TIME_CHOOSER_MINUTE_INDEX]) * 10;

    u16 hours = TIMECHOOSER_CTOI(tc->buffer[0 + 1]);
    hours += TIMECHOOSER_CTOI(tc->buffer[0]) * 10;

    seconds += minutes * 60;
    seconds += hours * 60 * 60;

    return seconds;
}
