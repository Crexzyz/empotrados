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

OBJ_ATTR obj_buffer[128];

void run_init()
{
    irq_init(NULL);
    irq_add(II_VBLANK, NULL);
	
    REG_DISPCNT = DCNT_OBJ | DCNT_OBJ_1D | DCNT_MODE0 | DCNT_BG0;
	oam_init(obj_buffer, 128);

	tte_init_se_default(0, BG_CBB(0) | BG_SBB(31));
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
			update_clock(&clock);
			
			snprintf(buf, SPRITE_BUFFER_SIZE + 1, "%02d:%02d:%02d",
				clock.hours, clock.minutes, clock.seconds);
			
			np_rainbow_print(&np, 50, 30, buf, strnlen(buf, SPRITE_BUFFER_SIZE + 1));

			oam_copy(oam_mem, obj_buffer, SPRITE_BUFFER_SIZE);
		}

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
	OptionText_init(&options[2], "Back", strlen("Back"));

	OptionFunction functions[OPTIONS_STOPWATCH];
	OptFunc_init(&functions[0], start_stop_watch, &param);
	OptFunc_init(&functions[1], lap_stop_watch, &param);
	OptFunc_init(&functions[2], DispCtrl_back, dc);
	
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

		if(dc->content_change == true)
			break;
	}

	oam_copy(oam_mem, 0, SPRITE_BUFFER_SIZE);
	dc->content_change = false;
}

void run_time_chooser(DispCtrl * dc)
{
}

void open_time_chooser(DispCtrl * dc)
{
	DispCtrl_push(dc, run_time_chooser);
}

void run_alarm(DispCtrl * dc)
{
	const u8 options_count = 2;

	TimeChooser tc;
	NumberPrinter np;
	OptionText options[options_count];
	OptionFunction functions[options_count];
	OptsChser oc;

	OBJ_ATTR * sprite_buf = &obj_buffer[SPRITE_BUFFER_SIZE + 1];

	OptionText_init(&options[0], "Back", strlen("Back"));
	OptionText_init(&options[1], "Set alarm", strlen("Set alarm"));

	OptFunc_init(&functions[0], DispCtrl_back, dc);
	OptFunc_init(&functions[1], TimeChooser_toggle_edit, &tc);

	OptsChser_init(&oc, options, options_count, functions);
	OptsChser_set_coords(&oc, 0, 100);

	np_init(&np, sprite_buf, TIME_CHOOSER_BUF_SIZE - 1);
	TimeChooser_init(&tc, &np, 30, 30);

	while(true)
	{
		VBlankIntrWait();

		key_poll();
		TimeChooser_show(&tc);

		if(!tc.editing)
		{
			// Prevents listening for keystrokes on the menu
			OptsChser_show(&oc);
			tte_write("#{P:0,150}                              ");
		}
		else
		{
			tte_write("#{P:0,150} A: Save           B: Cancel");
		}

		oam_copy(&oam_mem[SPRITE_BUFFER_SIZE], sprite_buf, TIME_CHOOSER_BUF_SIZE - 1);
		if(dc->content_change == true)
			break;
	}

	oam_copy(&oam_mem[SPRITE_BUFFER_SIZE], 0, TIME_CHOOSER_BUF_SIZE);
	dc->content_change = false;
}

void run_nprinter_test(DispCtrl * dc)
{
    NumberPrinter np;
	np_init(&np, &obj_buffer[0], SPRITE_BUFFER_SIZE);
	char buf[SPRITE_BUFFER_SIZE + 1]; buf[SPRITE_BUFFER_SIZE] = 0; 
	// char tbuf[30];

	u8 color = WHITE;

	while(true)
	{
		for(size_t i = 0; i < 10; ++i)
			VBlankIntrWait();

		key_poll();

		if(key_hit(KEY_B))
			color = (color + 1) % (YELLOW + 1);

		if(key_hit(KEY_A))
		{
			u16 num = qran_range(0, 61);
			snprintf(buf, SPRITE_BUFFER_SIZE + 1, "%02d:%02d:%02d", num, num, num);
			
			// snprintf(tbuf, 30, "#{P:24,10} %s %d", buf, SPRITE_BUFFER_SIZE);
			// tte_write(tbuf);
		}

		np_rainbow_print(&np, 30, 30, buf, strnlen(buf, SPRITE_BUFFER_SIZE + 1));
		oam_copy(oam_mem, obj_buffer, SPRITE_BUFFER_SIZE);
	}
}