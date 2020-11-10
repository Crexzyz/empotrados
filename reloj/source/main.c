#include <tonc.h>
#include <stdbool.h>
#include <string.h>
#include <stdbool.h>

#include "Nums.h"
#include "NumberPrinter.h"
#include "DisplayController.h"
#include "OptionsChooser.h"
#include "Clock.h"

#define SPRITE_BUFFER_SIZE 8 // "00:00:00"
OBJ_ATTR obj_buffer[128];

void nprinter_test()
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

void test1_click(OptsChser * oc)
{
	tte_write("#{P:100,70} Click 1");
	if(key_hit(KEY_B))
	{
		tte_write("#{P:100,70}         ");
		if(oc)
			oc->option_locked = false;

	}
}

void test4_click(DispCtrl * dc)
{	
	char buf[SPRITE_BUFFER_SIZE + 1]; buf[SPRITE_BUFFER_SIZE] = 0;

	Clock clock;
    initClock(&clock);

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
			updateClock(&clock);
			
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

void test2_click(DispCtrl* dc )
{
	// tte_write("#{P:100,86} Click 2");

	OptionFunction function;
	OptFunc_init(&function, test4_click, dc);

	stack_push(&dc->stack, &function);

}

void test3_click()
{
	tte_write("#{P:100,100} Click 3");
}

#define OPTIONS_AMOUNT 3

void dispctrl_test()
{
	OptsChser oc;
	DispCtrl dc;
	OptionText options[OPTIONS_AMOUNT];
	OptionFunction functions[OPTIONS_AMOUNT];
	OptionFunction function_buffer[STACK_MAX_CAPACITY];

	OptionText_init(&options[0], "Test 1", strlen("Test 1"));
	OptionText_init(&options[1], "Test 2", strlen("Test 2"));
	OptionText_init(&options[2], "Test 3", strlen("Test 3"));

	OptFunc_init(&functions[0], test1_click, &oc);
	OptFunc_init(&functions[1], test2_click, &dc);
	OptFunc_init(&functions[2], test3_click, NULL);

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
	irq_init(NULL);
    irq_add(II_VBLANK, NULL);
	
    REG_DISPCNT = DCNT_OBJ | DCNT_OBJ_1D | DCNT_MODE0 | DCNT_BG0;
	oam_init(obj_buffer, 128);

	tte_init_se_default(0, BG_CBB(0) | BG_SBB(31));

	// nprinter_test();
	dispctrl_test();

	return 0;
}