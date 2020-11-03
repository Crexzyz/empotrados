#include <tonc.h>
#include "GameController.h"

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

// 128-sprite buffer
OBJ_ATTR obj_buffer[128];
// sound
u8 txt_scrolly= 8;

static Sprite sprite;
static Coin coin;
static BlockGenerator bgen;
static PATTERN pats[HWLEN];

void gamectrl_init_regs()
{
    REG_BG1CNT = BG_CBB(0) | BG_SBB(30) | BG_8BPP | BG_REG_32x32;
    REG_DISPCNT = DCNT_OBJ | DCNT_OBJ_1D | DCNT_MODE0 | DCNT_BG0 | DCNT_BG1;
	tte_init_se_default(0, BG_CBB(1) | BG_SBB(31));
}

void gamectrl_init_interrupts()
{
    irq_init(NULL);
    irq_add(II_VBLANK, NULL);
}

void gamectrl_init()
{
    gamectrl_init_regs();
    gamectrl_init_interrupts();
	sprite_load_to_mem();

    // Init sprite buffers
	oam_init(obj_buffer, 128);

    //init sprite tittle text
    txt_init_std();

	// Load first background
	dma3_cpy(pal_bg_mem, initPal, initPalLen);
    dma3_cpy(tile_mem[0], initTiles, initTilesLen);
    dma3_cpy(se_mem[30], initMap, initMapLen);
    
    //12 px between letters
    gptxt->dx = 12;

    //init sprite letter
	//set tittle properties
    title_init(pats, (OBJ_ATTR *)oam_mem);

    
}

int gamectrl_run()
{
	gamectrl_init();

    sprite_init(&sprite, &obj_buffer[0]);
	sprite_coin_init(&coin, &obj_buffer[1]);
	blockgen_init(&bgen, obj_buffer);
	blockgen_init_blocks(&bgen);
	sprite_place_on_rect(&sprite, blockgen_get_topmost_block8(&bgen, 0));

	initial_sound();
    sound_setting();

    gamectrl_start();

    return 0;
}

void gamectrl_start()
{
	// char to put in screen
	char totalScore[100]; 

	int start = 0;
	int ii= 0;
	int hScroll = 0;
	int h2Scroll = 0;
	int win=0;
    OBJ_ATTR * oe = (OBJ_ATTR *)oam_mem;
	
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
}