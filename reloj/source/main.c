#include <tonc.h>
#include <stdbool.h>
#include <string.h>

#include "Nums.h"
#include "NumberPrinter.h"

#define SPRITE_BUFFER_SIZE 4
OBJ_ATTR obj_buffer[128];


void test()
{
	NumberPrinter np;
	np_init(&np, &obj_buffer[0], SPRITE_BUFFER_SIZE);
	char buf[SPRITE_BUFFER_SIZE + 1];
	buf[SPRITE_BUFFER_SIZE] = 0; 

	while(true)
	{
		for(size_t i = 0; i < 60; ++i)
			VBlankIntrWait();
		
		u16 num = qran_range(0, 9999);
		u8 color = qran_range(WHITE, YELLOW+1);

		snprintf(buf, SPRITE_BUFFER_SIZE + 1, "%d", num);
		np_print(&np, 30, 30, color, buf, strnlen(buf, SPRITE_BUFFER_SIZE + 1));

		oam_copy(oam_mem, obj_buffer, SPRITE_BUFFER_SIZE);
	}
}

int main()
{
	irq_init(NULL);
    irq_add(II_VBLANK, NULL);
	
    REG_DISPCNT = DCNT_OBJ | DCNT_OBJ_1D;
	oam_init(obj_buffer, 128);

	test();

	return 0;
}