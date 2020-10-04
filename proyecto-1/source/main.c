#include <tonc.h>

#include "Sprite.h"
#include "Map.h"
#include "Rect.h"
#include "Menu.h"
#include "BlockGenerator.h"
#include "CoinTile.h"
#include "music.h"

#include "soundbank.h"
#include "soundbank_bin.h"

// 128-sprite buffer
OBJ_ATTR obj_buffer[128];
// sound
u8 txt_scrolly= 8;

int main()
{
    // Initialize maxmod with default settings
    // pass soundbank address, and allocate 8 channels.
    mmInitDefault( (mm_addr) soundbank_bin, 8 );
	sprite_load_to_mem();
	
	REG_BG1CNT = BG_CBB(0) | BG_SBB(30) | BG_8BPP | BG_REG_32x32;
    REG_DISPCNT = DCNT_OBJ | DCNT_OBJ_1D | DCNT_MODE0 | DCNT_BG0 | DCNT_BG1;
	tte_init_se_default(0, BG_CBB(1) | BG_SBB(31));

    irq_init(NULL);
    irq_add(II_VBLANK, NULL);
	oam_init(obj_buffer, 128);

    //init sprite tittle text
    txt_init_std();
    //12 px between letters
    gptxt->dx= 12;
	OBJ_ATTR *oe= oam_mem;
    //init sprite letter
	//set tittle propperties
	PATTERN pats[HWLEN];
    title_init(pats, oe);

	Sprite sprite;
	sprite_init(&sprite, &obj_buffer[0]);

	Coin coin;
	sprite_coin_init(&coin, &obj_buffer[5]);

	sprite.pos_x = 30; sprite.pos_y = 120;

	Map map;
	map_init(&map);
	map_set_scroll(&map, 0, 0);
	
	Rect r1;
	rect_init(&r1);
	rect_set_sprite(&r1, &obj_buffer[1]);
	rect_set_coords16(&r1, 30, 150);

	Rect r2;
	rect_init(&r2);
	rect_set_sprite(&r2, &obj_buffer[2]);
	rect_set_coords16(&r2, 90, 110);

	Rect r3;
	rect_init(&r3);
	rect_set_sprite(&r3, &obj_buffer[3]);
	rect_set_coords16(&r3, 140, 70);

	Rect r4;
	rect_init(&r4);
	rect_set_sprite(&r4, &obj_buffer[4]);
	rect_set_coords16(&r4, 200, 30);

	// char to put in screen
	char totalScore[100]; 

	// User sprite does not go here
	Rect * rects[MAX_BLOCKS];
	rects[0] = &r1;
	rects[1] = &r2;
	rects[2] = &r3;
	rects[3] = &r4;

	BlockGenerator bgen;
	blockgen_init(&bgen);
	blockgen_set_blocks(&bgen, (Rect**)(&rects));
	blockgen_set_map(&bgen, &map);

	initial_sound();

	int start = 0;
	int ii= 0;
	sound_setting();
	map_load_to_mem();
	while(1)
	{
		VBlankIntrWait();
		mmFrame();

		key_poll();
		for(ii=0; ii<HWLEN; ii++){
        	pat_bounce(&pats[ii]);
        	oe[ii].attr0 &= ~ATTR0_Y_MASK;
       		oe[ii].attr0 |= (pats[ii].fy>>8)& ATTR0_Y_MASK;
        }		

		if(key_hit(KEY_START)){	
			print_instructions(oe);
		}

		if(key_hit(KEY_A)){
			oam_copy(oe, 0, 12);
			start=1;
		}

		if(key_hit(KEY_SELECT))
		{
			sprite.pos_x = 50;
			sprite.pos_y = 30;
		}

		if(start){
			tte_write("#{es}");
			map_load_to_mem();

			for(int i = 0; i < MAX_BLOCKS; ++i)
				rect_paint(rects[i]);

			int do_scroll = blockgen_autoscroll(&bgen);
			if(do_scroll) sprite.pos_y += 1;

			sprite_update_pos_collision(&sprite, (Rect**)(&rects), MAX_BLOCKS);
			sprite_change_animation(&sprite);

			// Change coin animation
			sprite_coin_update_pos(&coin);
			sprite_coin_change_animation(&coin);
			// Detect coin-sprite collision
			if(do_sprites_collisions(&coin,&sprite)){
				// Write in screen, position x = 0, y = 0
				snprintf(totalScore, 100, "#{P:0, 0}Coins:%d", coin.currentScore);
				tte_write(totalScore);
			}
			sprite_coin_unhide(&coin, &sprite);

			// Move the sprites to VRAM
			oam_copy(oam_mem, obj_buffer, 6);
		}

		if(coin.currentScore==3 || sprite.pos_y > 160){
				final_screen(oam_mem, coin.currentScore);
				sprite.pos_x = 50;
				sprite.pos_y = 30;
				coin.currentScore=0;
				start=0;
		}
	}
	return 0;
}