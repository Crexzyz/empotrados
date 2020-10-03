#include "Sprite.h"
#include "Map.h"
#include "Rect.h"
#include "music.h"

// 128-sprite buffer
OBJ_ATTR obj_buffer[128];
// sound
u8 txt_scrolly= 8;



int main()
{
	sound_setting();
//	effects();
	sprite_load_to_mem();
	map_load_to_mem();
	
	REG_BG1CNT = BG_CBB(0) | BG_SBB(30) | BG_8BPP | BG_REG_32x32;
    REG_DISPCNT = DCNT_OBJ | DCNT_OBJ_1D | DCNT_MODE0 | DCNT_BG0 | DCNT_BG1;
	tte_init_se_default(0, BG_CBB(2) | BG_SBB(40) | BG_8BPP | BG_REG_32x32);

    // irq_init(NULL);
    // irq_add(II_VBLANK, NULL);
	// oam_init(obj_buffer, 128);

	Sprite sprite;
	sprite_init(&sprite, &obj_buffer[0]);
	sprite.pos_x = 50;
	sprite.pos_y = 125;

	Map map;
	map_init(&map);

	Rect rect;
	rect_init(&rect);
	rect_set_sprite(&rect, &obj_buffer[1]);
	rect_set_coords(&rect, sprite.pos_x, sprite.pos_y, sprite.pos_x+16, sprite.pos_y+16);
	
	Rect rect2;
	rect_init(&rect2);
	rect_set_sprite(&rect2, &obj_buffer[2]);
	rect_set_coords(&rect2, 50, 140, 66, 156);

	Rect rect3;
	rect_init(&rect3);
	rect_set_sprite(&rect3, &obj_buffer[3]);
	rect_set_coords(&rect3,  100, 100, 116, 116);

	Rect rect4;
	rect_init(&rect4);
	rect_set_sprite(&rect4, &obj_buffer[4]);
	rect_set_coords(&rect4, 150, 80, 166, 96);

	int currentChar = 0;
	// char test[100]; 

	// User sprite does not go here
	Rect * rects[3];
	rects[0] = &rect2;
	rects[1] = &rect3;
	rects[2] = &rect4;


	//initial_sound();


	int keys_pressed, keys_released;
	
	do
	{
		VBlankIntrWait();
		mmFrame();

		// Clean
		// tte_write("#{P:0, 20}                       ");
		// snprintf(test, 100, "#{P:0, 20} 1 x:%d,%d y:%d,%d", rect.x1,rect.x2, rect.y1, rect.y2);
		// tte_write(test);

        key_poll();

		keys_pressed = keysDown();
		keys_released = keysUp();

		// Play looping ambulance sound effect out of left speaker if A button is pressed
		if ( keys_pressed & KEY_A ) {
			note_play(NOTE_B, 0);
		}

		for(size_t rect = 0; rect < 3; ++rect)
			rect_paint(rects[rect]);

		if(key_hit(KEY_B))
			currentChar = (currentChar + 1) % 2;

		if(currentChar == 1)
			map_key_move(&map);
		else
		{
			sprite_update_pos_collision(&sprite, (Rect**)(&rects), 3);
			sprite_change_animation(&sprite);

		}
		rect_set_coords(&rect, sprite.pos_x, sprite.pos_y, sprite.pos_x+16, sprite.pos_y+16);

		// Move the sprites to VRAM
		oam_copy(oam_mem, obj_buffer, 5);

	}while( 1 );

	return 0;
}
