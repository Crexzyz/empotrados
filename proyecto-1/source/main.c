#include <tonc.h>

#include "Sprite.h"
#include "Map.h"
#include "Rect.h"
#include "Menu.h"

// 128-sprite buffer
OBJ_ATTR obj_buffer[128];
// sound
u8 txt_scrolly= 8;

// Play a little ditty
void initial_song()
{
	const u8 lens[6]= { 1,1,4, 1,1,4 };
	const u8 notes[6]= { 0x02, 0x05, 0x12,  0x02, 0x05, 0x12 };
	int ii;
	for(ii=0; ii<6; ii++)
	{
		note_play(notes[ii]&15, notes[ii]>>4);
		VBlankIntrDelay(8*lens[ii]);
	}
}

void initial_sound()
{
	// turn sound on
	REG_SNDSTAT= SSTAT_ENABLE;
	// snd1 on left/right ; both full volume
	REG_SNDDMGCNT = SDMG_BUILD_LR(SDMG_SQR1, 7);
	// DMG ratio to 100%
	REG_SNDDSCNT= SDS_DMG100;

	// no sweep
	REG_SND1SWEEP= SSW_OFF;
	// envelope: vol=12, decay, max step time (7) ; 50% duty
	REG_SND1CNT= SSQR_ENV_BUILD(12, 0, 7) | SSQR_DUTY1_2;
	REG_SND1FREQ= 0;

	initial_song();
}

int main()
{
	sprite_load_to_mem();
	
	REG_BG1CNT = BG_CBB(0) | BG_SBB(30) | BG_8BPP | BG_REG_32x32;
    REG_DISPCNT =  DCNT_OBJ | DCNT_OBJ_1D | DCNT_MODE0 | DCNT_BG0 | DCNT_BG1;
	tte_init_se_default(0, BG_CBB(1) | BG_SBB(31));

    irq_init(NULL);
    irq_add(II_VBLANK, NULL);
	oam_init(obj_buffer, 128);

    //init sprite tittle text
    txt_init_std();
    txt_init_obj(&oam_mem[0], 0xF200, CLR_YELLOW, 0xEE);
    //12 px between letters
    gptxt->dx= 12;
	OBJ_ATTR *oe= oam_mem;
    //init sprite letter
    obj_puts2(120-12*HWLEN/2, 8, hwstr, 0xF200, oe);

	//set tittle propperties
	PATTERN pats[HWLEN];
    title_init(pats, oe);

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


	initial_sound();

	int start = 0;
	int ii= 0;
	tte_write("#{P:81, 110} Presione A");
	while(1)
	{
		VBlankIntrWait();

		// Clean
		// tte_write("#{P:0, 20}                       ");
		// snprintf(test, 100, "#{P:0, 20} 1 x:%d,%d y:%d,%d", rect.x1,rect.x2, rect.y1, rect.y2);
		// tte_write(test);	

		key_poll();
			
		for(ii=0; ii<HWLEN; ii++){
        	pat_bounce(&pats[ii]);
        	oe[ii].attr0 &= ~ATTR0_Y_MASK;
       		oe[ii].attr0 |= (pats[ii].fy>>8)& ATTR0_Y_MASK;
        }		

		if(key_hit(KEY_A))
			start=1;

		if(start){

			OAM_CLEAR();
			tte_write("#{es}");
			map_load_to_mem();
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

		}
		
	}
	return 0;
}
