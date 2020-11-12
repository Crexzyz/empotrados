#ifndef TIME_CHOOSER_H
#define TIME_CHOOSER_H

#include <tonc.h>
#include "NumberPrinter.h"

#define TIME_CHOOSER_BUF_SIZE 9 // 00:00:00\0

#define TIME_CHOOSER_FIRST_COLON 2
#define TIME_CHOOSER_MINUTE_INDEX 3
#define TIME_CHOOSER_SECOND_COLON 5
#define TIME_CHOOSER_SECOND_INDEX 6

#define IS_MINUTE_LIMIT(index) (index == TIME_CHOOSER_MINUTE_INDEX || index == TIME_CHOOSER_SECOND_INDEX)

typedef struct _TimeChooser {
    u8 current_number;
    char buffer[TIME_CHOOSER_BUF_SIZE];
    u8 start_x;
    u8 start_y;
    NumberPrinter * np;
    bool editing;
} TimeChooser;

void TimeChooser_init(TimeChooser * tc, NumberPrinter * np, u8 x, u8 y);
void TimeChooser_show(TimeChooser * tc);
void TimeChooser_edit(TimeChooser * tc);
void TimeChooser_toggle_edit(TimeChooser * tc);
void TimeChooser_add_alarm();

INLINE char * TimeChooser_get_time(TimeChooser * tc) { return tc->buffer; }

#endif