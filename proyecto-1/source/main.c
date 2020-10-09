#include <tonc.h>
#include <stdio.h>

#include "Sprite.h"
#include "Rect.h"
#include "Menu.h"
#include "BlockGenerator.h"
#include "CoinTile.h"
#include "music.h"
#include "twoCloud.h"
#include "win.h"
#include "init.h"
#include "soundbank.h"
#include "soundbank_bin.h"

typedef unsigned short     uint16;
#define REG_BG1_SCROLL_H       *((volatile uint16*)(0x04000014))
#define REG_BG1_SCROLL_V       *((volatile uint16*)(0x04000016))

// 128-sprite buffer
OBJ_ATTR obj_buffer[128];
// sound
u8 txt_scrolly= 8;

#define twoCloudgrayPalLen 256
const unsigned char twoCloudgrayPal[256] __attribute__((aligned(4)))=
{0x10,0x42,0x73,0x4E,0xB5,0x56};

int main()
{
	// Player + coin + blocks
	const u32 SPRITES_AMOUNT = 1 + 1 + BLOCKS_AMOUNT; 
	// Load first background
	dma3_cpy(pal_bg_mem, initPal, initPalLen);
    dma3_cpy(tile_mem[0], initTiles, initTilesLen);
    dma3_cpy(se_mem[30], initMap, initMapLen);
    
	// Initialize maxmod with default settings
    // pass soundbank address, and allocate 8 channels.
    mmInitDefault( (mm_addr) soundbank_bin, 8 );
	sprite_load_to_mem();

	REG_BG1CNT = BG_CBB(0) | BG_SBB(30) | BG_8BPP | BG_REG_32x32;
    REG_DISPCNT = DCNT_OBJ | DCNT_OBJ_1D | DCNT_MODE0 | DCNT_BG0 | DCNT_BG1;// | DCNT_BG2;
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
	sprite_coin_init(&coin, &obj_buffer[1]);

	BlockGenerator bgen;
	blockgen_init(&bgen, obj_buffer);
	blockgen_init_blocks(&bgen);
	
	sprite_place_on_rect(&sprite, blockgen_get_topmost_block8(&bgen, 0));
	
	// char to put in screen
	char totalScore[100]; 

	initial_sound();

	int start = 0;
	int ii= 0;
	int hScroll = 0;
	int h2Scroll = 0;
	int win=0;
	sound_setting();

	while(1)
	{
		VBlankIntrWait();
		mmFrame();
		key_poll();

		if(!start)
		{
			if(win){
				REG_BG1_SCROLL_H = hScroll;
				hScroll +=1;
			}
			for(ii=0; ii<HWLEN; ii++){
				pat_bounce(&pats[ii]);
				oe[ii].attr0 &= ~ATTR0_Y_MASK;
				oe[ii].attr0 |= (pats[ii].fy>>8)& ATTR0_Y_MASK;
			}

			// Skip start screen and print instructions
			if(key_hit(KEY_START)){	
				print_instructions(oe);
			}
			// Start Game
			if(key_hit(KEY_A)){
				oam_copy(oe, 0, 12);
				sprite_coin_init(&coin, &obj_buffer[1]);
				start=1;
			}
		}

		if(start){
			tte_write("#{es}");

			for(int i = 0; i < BLOCKS_AMOUNT; ++i)
				rect_paint(&bgen.blocks[i]);

			// Load game background
			dma3_cpy(pal_bg_mem, twoCloudPal, twoCloudPalLen);
			dma3_cpy(tile_mem[0], twoCloudTiles, twoCloudTilesLen);
			dma3_cpy(se_mem[30], twoCloudMap, twoCloudMapLen);
	
			int do_scroll = blockgen_autoscroll(&bgen);
			if(do_scroll) sprite.pos_y += 1;

			Rect * rects = bgen.blocks;
			sprite_update_pos_collision(&sprite, &rects, BLOCKS_AMOUNT);
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

			// Move the sprites to VRAM. Player + coin + blocks
			oam_copy(oam_mem, obj_buffer, SPRITES_AMOUNT);
			
			// Move background vertical
		    REG_BG1_SCROLL_V = h2Scroll;
			hScroll -=1;
			h2Scroll = hScroll/5;

		}

		if(coin.currentScore==3)
		{
			// Sets the end game screen to be shown in this case game won
			final_screen(oam_mem, coin.currentScore, SPRITES_AMOUNT);
			sprite_place_on_rect(&sprite, blockgen_get_topmost_block8(&bgen, 0));
			sprite_coin_init_with_colis(&coin, &obj_buffer[1],&sprite);
			sprite.jumps = 0;
			start=0;

			REG_BG1_SCROLL_V = 0;
			REG_BG1_SCROLL_H = 0;
			hScroll = 0;
			h2Scroll = 0;
			win=1;
			dma3_cpy(pal_bg_mem, winPal, winPalLen);
			dma3_cpy(tile_mem[0], winTiles, winTilesLen);
			dma3_cpy(se_mem[30], winMap, winMapLen);

		}
		else if(sprite.pos_y > 160)
		{
			// Sets the end game screen to be shown in this case game lost
			final_screen(oam_mem, coin.currentScore, SPRITES_AMOUNT);
			sprite_place_on_rect(&sprite, blockgen_get_topmost_block8(&bgen, 0));
			sprite_coin_init_with_colis(&coin, &obj_buffer[1],&sprite);
			sprite.jumps = 0;
			start=0;
			win = 0;
			// Change palette
			dma3_cpy(pal_bg_mem, twoCloudgrayPal, twoCloudgrayPalLen);
		}
	}
	return 0;
}