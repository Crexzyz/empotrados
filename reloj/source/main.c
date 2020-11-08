#include <tonc.h>
#include <stdbool.h>
#include <string.h>

#include "Nums.h"
#include "NumberPrinter.h"

#define SPRITE_BUFFER_SIZE 9
OBJ_ATTR obj_buffer[128];


void test()
{
	NumberPrinter np;
	np_init(&np, &obj_buffer[0], SPRITE_BUFFER_SIZE);
	char buf[SPRITE_BUFFER_SIZE + 1]; buf[SPRITE_BUFFER_SIZE] = 0; 
	char tbuf[30];

	u8 color = WHITE;

	while(true)
	{
		
		VBlankIntrWait();
		key_poll();

		if(key_hit(KEY_B))
			color = (color + 1) % (YELLOW + 1);

		if(key_hit(KEY_A))
		{
			u16 num = qran_range(0, 61);
			snprintf(buf, SPRITE_BUFFER_SIZE + 1, "%02d:%02d:%02d", num, num, num);
			
			snprintf(tbuf, 30, "#{P:24,10} %s", buf);
			tte_write(tbuf);
		}

		np_print(&np, 30, 30, color, buf, strnlen(buf, SPRITE_BUFFER_SIZE + 1));
		oam_copy(oam_mem, obj_buffer, SPRITE_BUFFER_SIZE);
	}
}

int main()
{
	irq_init(NULL);
    irq_add(II_VBLANK, NULL);
	
    REG_DISPCNT = DCNT_OBJ | DCNT_OBJ_1D | DCNT_MODE0 | DCNT_BG0;
	oam_init(obj_buffer, 128);

	tte_init_se_default(0, BG_CBB(0) | BG_SBB(31));

	test();

	return 0;
}