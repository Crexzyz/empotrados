#include <tonc.h>
#include <stdbool.h>
#include <string.h>
#include <stdbool.h>

#include "Nums.h"
#include "NumberPrinter.h"
#include "DisplayController.h"
#include "OptionsChooser.h"
#include "Clock.h"
#include "StopWatch.h"
#include "Runner.h"

#define SPRITE_BUFFER_SIZE 8 // "00:00:00"
#define OPTIONS_AMOUNT 3

typedef struct _PushData {
	DispCtrl * dc;
	void (*func);
} PushData;

void test1_click()
{
	tte_write("#{P:100,70} Click 1");
}

void open_clock(DispCtrl * dc)
{
	DispCtrl_push(dc, run_clock);
}

void open_stopwatch(DispCtrl * dc)
{
	DispCtrl_push(dc, run_stopwatch);
}

void open_alarm(DispCtrl * dc)
{
	DispCtrl_push(dc, run_alarm);
}

void dispctrl_test()
{
	OptsChser oc;
	DispCtrl dc;
	OptionText options[OPTIONS_AMOUNT];
	OptionFunction functions[OPTIONS_AMOUNT];
	OptionFunction function_buffer[STACK_MAX_CAPACITY];

	OptionText_init(&options[0], "Alarm", strlen("Alarm"));
	OptionText_init(&options[1], "Clock", strlen("Clock"));
	OptionText_init(&options[2], "Stopwatch", strlen("Stopwatch"));

	OptFunc_init(&functions[0], open_alarm, &dc);
	OptFunc_init(&functions[1], open_clock, &dc);
	OptFunc_init(&functions[2], open_stopwatch, &dc);

	OptsChser_init(&oc, options, OPTIONS_AMOUNT, functions);

	OptionFunction main_function;
	OptFunc_init(&main_function, OptsChser_show, &oc);

	char * title = "Clock";
	DispCtrl_init(&dc, title, strlen(title), function_buffer);
	stack_push(&dc.stack, &main_function);

	while(true)
	{
		VBlankIntrWait();
		key_poll();

		DispCtrl_show(&dc);
	}
}

int main()
{
	run_init();

	// nprinter_test();
	dispctrl_test();

	return 0;
}