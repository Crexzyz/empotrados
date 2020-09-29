#include <tonc.h>

#include "Sprite.h"
#include "Map.h"

// 128-sprite buffer
OBJ_ATTR obj_buffer[128];

int main()
{
	sprite_load_to_mem();
	map_load_to_mem();
	
	REG_BG1CNT = BG_CBB(0) | BG_SBB(30) | BG_8BPP | BG_REG_32x32;
    REG_DISPCNT = DCNT_OBJ | DCNT_OBJ_1D | DCNT_MODE0 | DCNT_BG0 | DCNT_BG1;
	tte_init_se_default(0, BG_CBB(2) | BG_SBB(40) | BG_8BPP | BG_REG_32x32);

    irq_init(NULL);
    irq_add(II_VBLANK, NULL);
	oam_init(obj_buffer, 128);

	Map map;
	map_init(&map);
	// map_set_collision_map(&map, );
	Sprite sprite;
	sprite_init(&sprite, &obj_buffer[0]);
	sprite.pos_x = 50;
	int currentChar = 0;
	char test[100]; 

	while(1)
	{
        //hackily slowing down the frame rate
        //to see anims better
        for (int i = 0; i < 2; ++i)
            VBlankIntrWait();

		// Clean
		tte_write("#{P:0, 20}               ");
		snprintf(test, 100, "#{P:0, 20} x:%d y:%d", sprite.pos_x, sprite.pos_y);
		tte_write(test);

        key_poll();

		if(key_hit(KEY_B))
			currentChar = (currentChar + 1) % 2;

		if(currentChar == 1)
			map_key_move(&map);
		else
		{
			// sprite_map_update_position(&map, &sprite);
			sprite_update_position(&sprite);
			sprite_change_animation(&sprite);
		}

		// Move the sprites to VRAM
		oam_copy(oam_mem, obj_buffer, 1);

	}

	return 0;
}
