#include <tonc.h>
#include <stdbool.h>
#include <string.h>
#include <stdbool.h>

#include "Runner.h"
#include "Nums.h"
#include "NumberPrinter.h"
#include "DisplayController.h"
#include "OptionsChooser.h"
#include "Clock.h"
#include "StopWatch.h"
#include "TimeChooser.h"
#include "Alarm.h"

OBJ_ATTR obj_buffer[128];

void run_init()
{
    irq_init(NULL);
    irq_add(II_VBLANK, NULL);
	
    REG_DISPCNT = DCNT_OBJ | DCNT_OBJ_1D | DCNT_MODE0 | DCNT_BG0;
	oam_init(obj_buffer, 128);

	tte_init_se_default(0, BG_CBB(0) | BG_SBB(31));

	// Just init the registers
	init_clock(NULL);
}

void runner_rainbow_print(Clock * clock, char * buffer, NumberPrinter * np, u8 x, u8 y, size_t len)
{
	update_clock(clock);
	
	snprintf(buffer, len, "%02d:%02d:%02d",
		clock->hours, clock->minutes, clock->seconds);
	
	np_rainbow_print(np, x, y, buffer, strnlen(buffer, len));
}

void run_clock(DispCtrl * dc)
{
    char buf[SPRITE_BUFFER_SIZE + 1]; buf[SPRITE_BUFFER_SIZE] = 0;

	Clock clock;
    init_clock(&clock);

	OptionText options[1];
	OptionText_init(&options[0], "Back", strlen("Back"));

	OptionFunction functions[1];
	OptFunc_init(&functions[0], DispCtrl_back, dc);
	
	OptsChser oc;
	OptsChser_init(&oc, options, 1, functions);

	NumberPrinter np;
	np_init(&np, &obj_buffer[0], SPRITE_BUFFER_SIZE);

	size_t frame_counter = 0;
	
	while(true)
	{
		VBlankIntrWait();
		frame_counter = (frame_counter + 1) % 60;

		key_poll();
		OptsChser_show(&oc);
		
		if(frame_counter % 10 == 0)
		{
			runner_rainbow_print(&clock, buf, &np, 50, 30, SPRITE_BUFFER_SIZE + 1);
			oam_copy(oam_mem, obj_buffer, SPRITE_BUFFER_SIZE);
		}

		if(alarm_raised)
			DispCtrl_back(dc);

		if(dc->content_change == true)
			break;
	}

	oam_copy(oam_mem, 0, SPRITE_BUFFER_SIZE);
	dc->content_change = false;
}

void run_stopwatch(DispCtrl * dc)
{
    Clock clockWatch;
	StopWatch stopWatch;
    init_stop_watch(&stopWatch);

	Params param;
	param.clock = &clockWatch;
    param.stopWatch = &stopWatch;

	OptionText options[OPTIONS_STOPWATCH];
	OptionText_init(&options[0], "Start", strlen("Start"));
	OptionText_init(&options[1], "Lap", strlen("Lap"));
	OptionText_init(&options[2], "Reset", strlen("Reset"));
	OptionText_init(&options[3], "Back", strlen("Back"));

	OptionFunction functions[OPTIONS_STOPWATCH];
	OptFunc_init(&functions[0], start_stop_watch, &param);
	OptFunc_init(&functions[1], lap_stop_watch, &param);
	OptFunc_init(&functions[2], reset_stop_watch, &stopWatch);
	OptFunc_init(&functions[3], DispCtrl_back, dc);
	
	OptsChser oc;
	OptsChser_init(&oc, options, OPTIONS_STOPWATCH, functions);
	OptsChser_set_coords(&oc, 0 , OPTSCHSER_DEFAULT_Y);
	NumberPrinter np;
	np_init(&np, &obj_buffer[0], SPRITE_BUFFER_SIZE);

	size_t frame_counter = 0;

	char buf[50];
    snprintf(buf, 50, "#{P:10,30} Timer:");
    tte_write(buf);
    snprintf(buf, 50, "#{P:10,40} 00:00:00");
    tte_write(buf);
	
	while(true)
	{
		VBlankIntrWait();
		frame_counter = (frame_counter + 1) % 60;

		key_poll();
		OptsChser_show(&oc);
		
		if(frame_counter % 10 == 0)
		{
			update_stop_watch(&stopWatch, &clockWatch);
		}

		if(alarm_raised)
			DispCtrl_back(dc);

		if(dc->content_change == true)
			break;
	}

	oam_copy(oam_mem, 0, SPRITE_BUFFER_SIZE);
	dc->content_change = false;
}

void run_alarm(DispCtrl * dc)
{
	char buf[SPRITE_BUFFER_SIZE + 1]; buf[SPRITE_BUFFER_SIZE] = 0;
	const u8 options_count = 2;

	TimeChooser tc;
	NumberPrinter np;
	OptionText options[options_count];
	OptionFunction functions[options_count];
	OptsChser oc;
	NumberPrinter np_clock;
	Clock clock;
	OBJ_ATTR * sprite_buf = &obj_buffer[SPRITE_BUFFER_SIZE + 1];
	
    init_clock(&clock);

	OptionText_init(&options[0], "Back", strlen("Back"));
	OptionText_init(&options[1], "Set alarm", strlen("Set alarm"));

	OptFunc_init(&functions[0], DispCtrl_back, dc);
	OptFunc_init(&functions[1], TimeChooser_toggle_edit, &tc);

	OptsChser_init(&oc, options, options_count, functions);
	OptsChser_set_coords(&oc, 0, 110);

	np_init(&np_clock, &obj_buffer[0], SPRITE_BUFFER_SIZE);
	np_init(&np, sprite_buf, TIME_CHOOSER_BUF_SIZE - 1);

	TimeChooser_init(&tc, &np, 112, 70);

	tte_write("#{P:0,40}Clock");
	tte_write("#{P:0,78}Current\nalarm");
	size_t frame_counter = 0;

	while(true)
	{
		VBlankIntrWait();
		key_poll();
		frame_counter = (frame_counter + 1) % 60;

		// If save pressed
		if (TimeChooser_show(&tc) )
			if(clock.seconds < TimeChooser_buffer2secs(&tc))
				alarm_set(TimeChooser_buffer2secs(&tc) - REG_TM3D);

		if(frame_counter % 10 == 0)
			runner_rainbow_print(&clock, buf, &np_clock, 112, 40, SPRITE_BUFFER_SIZE + 1);

		if(!tc.editing)
		{
			// Prevents listening for keystrokes on the menu
			OptsChser_show(&oc);
			tte_write("#{P:104,126}          ");
		}
		else
		{
			tte_write("#{P:0,126} A: Save   D-Pad: Move");
		}

		oam_copy(oam_mem, obj_buffer, SPRITE_BUFFER_SIZE);
		oam_copy(&oam_mem[SPRITE_BUFFER_SIZE], sprite_buf, TIME_CHOOSER_BUF_SIZE - 1);

		if(alarm_raised)
			DispCtrl_back(dc);

		if(dc->content_change == true)
			break;
	}

	oam_copy(oam_mem, 0, SPRITE_BUFFER_SIZE);
	oam_copy(&oam_mem[SPRITE_BUFFER_SIZE], 0, TIME_CHOOSER_BUF_SIZE);

	dc->content_change = false;
}

void run_alarm_alert(DispCtrl * dc)
{
	OptionText options[1];
	OptionText_init(&options[0], "Back", strlen("Back"));

	OptionFunction functions[1];
	OptFunc_init(&functions[0], DispCtrl_back, dc);

	OptsChser oc;
	OptsChser_init(&oc, options, 1, functions);
	OptsChser_set_coords(&oc, 0, 140);

	u32 frame_counter = 0;

	while(true)
	{
		VBlankIntrWait();

		frame_counter = (frame_counter + 1) % 60;

		if(frame_counter > 55)
			tte_set_color(TTE_INK, CLR_RED);
		else if(frame_counter > 50)
			tte_set_color(TTE_INK, CLR_ORANGE);
		else if(frame_counter > 45)
			tte_set_color(TTE_INK, CLR_YELLOW);
		else if(frame_counter > 40)
			tte_set_color(TTE_INK, CLR_LIME);
		else if(frame_counter > 35)
			tte_set_color(TTE_INK, CLR_GREEN);
		else if(frame_counter > 30)
			tte_set_color(TTE_INK, CLR_CYAN);	
		else if(frame_counter > 25)
			tte_set_color(TTE_INK, CLR_BLUE);
		else if(frame_counter > 20)
			tte_set_color(TTE_INK, CLR_MAG);
		else if(frame_counter > 15)
			tte_set_color(TTE_INK, CLR_FUCHSIA);
		else if(frame_counter > 10)
			tte_set_color(TTE_INK, CLR_SKYBLUE);
		else
			tte_set_color(TTE_INK, CLR_WHITE);

		tte_write("#{P:26,54} A L A R M - A L A R M");
		tte_write("#{P:26,70} A L A R M - A L A R M");
		tte_write("#{P:26,80} A L A R M - A L A R M");
		tte_write("#{P:26,96} A L A R M - A L A R M");

		key_poll();
		OptsChser_show(&oc);

		if(dc->content_change == true)
			break;
	}

	alarm_raised = false;
	dc->content_change = false;
	tte_set_color(TTE_INK, CLR_YELLOW);
}