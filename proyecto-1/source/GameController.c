#include <tonc.h>
#include "Enemy.h"
#include "GameController.h"
#include "Sprite.h"
#include "Rect.h"
#include "Menu.h"
#include "BlockGenerator.h"
#include "CoinTile.h"
#include "Trap.h"
#include "music.h"
#include "twoCloud.h"
#include "space.h"
#include "win.h"
#include "init.h"
#include "Heart.h"
#include "soundbank.h"
#include "soundbank_bin.h"

// 128-sprite buffer
OBJ_ATTR obj_buffer[128];
// sound
u8 txt_scrolly= 8;
int count = 0;
u32 sec = -1;
static Sprite sprite;
static Coin coin;
static Trap trap;
static Heart heart;
static BlockGenerator bgen;
static PATTERN pats[HWLEN];
static Enemy enemy1;
static Enemy enemy2;

bool win = false;


void dma_handler(){
	bool finish = false;
    char buf[50] = {};
	sec = REG_TM3D;

	while(false == finish){
		if(REG_TM3D != sec){
			sec = REG_TM3D;
			if(((sec%60) % 4) == 0){
				finish = true;
			}
		}
		snprintf(buf, 50, "#{P:24,60} Cargando \t\t%02d:%02d:%02d",
            sec/3600, (sec%3600)/60, sec%60);
        tte_write(buf);
	}

    // Clean text
	tte_write("#{es}");
	REG_IE |= IRQ_DMA3;
}


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
    irq_add(II_DMA3, dma_handler);
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

    REG_IE |= IRQ_DMA3;
	// Load first background
	dma3_cpy(pal_bg_mem, initPal, initPalLen);
    dma3_cpy(tile_mem[0], initTiles, initTilesLen);
	DMA_TRANSFER(se_mem[30], initMap, initMapLen/4, 3, DMA_CPY32 | DMA_IRQ | DMA_ENABLE);

    //12 px between letters
    gptxt->dx = 12;

    //init sprite letter
	//set tittle properties
    title_init(pats, (OBJ_ATTR *)oam_mem);    
}

int gamectrl_run()
{

    REG_TM2D = -0x4000; // 0xFFFFC000
    REG_TM2CNT = TM_FREQ_1024 | TM_ENABLE;
    REG_TM3CNT = TM_ENABLE | TM_CASCADE;

	gamectrl_init();

    sprite_init(&sprite, &obj_buffer[0]);
	sprite_coin_init(&coin, &obj_buffer[1]);
	sprite_trap_init(&trap, &obj_buffer[10]);
    sprite_enemy_init(&enemy1, &obj_buffer[11], 17);
    sprite_enemy_init(&enemy2, &obj_buffer[12], 18);
    sprite_heart_init(&heart, &obj_buffer[13]);
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
	// Text buffer
	char totalScore[100]; 

	bool start = false;
    bool second_level= false;

	int hScroll = 0;
	int h2Scroll = 0;
    u32 frame_counter = 0;

	while(1)
	{
		VBlankIntrWait();
		mmFrame();
		key_poll();

		if(!start)
		{
			if(win)
				REG_BG1_SCROLL_H = hScroll = hScroll + 1;

            if((start = gamectrl_show_main_menu()))
            {
                // Clean text
			    tte_write("#{es}");

                if(!second_level)
                    mmStart( MOD_FLATOUTLIES, MM_PLAY_LOOP );

            }
		}

		if(start)
        {
            if(!second_level){
                coin.currentScore = 0;
                gamectrl_show_first_lvl(totalScore, &frame_counter, &h2Scroll);

				// DMA-copy the background
				dma3_cpy(pal_bg_mem, twoCloudPal, twoCloudPalLen);
				dma3_cpy(tile_mem[0], twoCloudTiles, twoCloudTilesLen);
				dma3_cpy(se_mem[30], twoCloudMap, twoCloudMapLen);

			}else{
				// DMA-copy the background
				dma3_cpy(pal_bg_mem, spacePal, spacePalLen);
				dma3_cpy(tile_mem[0], spaceTiles, spaceTilesLen);
				dma3_cpy(se_mem[30], spaceMap, spaceMapLen);				

                gamectrl_show_second_lvl(totalScore, &frame_counter, &h2Scroll);
                VBlankIntrWait(); 
                mmFrame(); 
            }
			
            if(coin.currentScore == 3)
            {
                win = true;
                mmStop(); 
                VBlankIntrWait(); 

                REG_BG1_SCROLL_V = 0;
                REG_BG1_SCROLL_H = 0;
                hScroll = 0;
                h2Scroll = 0;

                // Sets the end game screen to be shown
                dma3_cpy(pal_bg_mem, winPal, winPalLen);
                dma3_cpy(tile_mem[0], winTiles, winTilesLen);
                dma3_cpy(se_mem[30], winMap, winMapLen);
            }
            else if(sprite.pos_y > 160)
            {
                win = false;
                mmStop();
                // Change palette
                dma3_cpy(pal_bg_mem, twoCloudgrayPal, twoCloudgrayPalLen);
                trap_hide(&trap);
                heart_hide(&heart);
                enemy_hide(&enemy1);
                enemy_hide(&enemy2);

            }

            if(coin.currentScore == 1 && !second_level)
            {
                change_music();
                second_level_transition(oam_mem, SPRITES_AMOUNT);
                dma3_cpy(pal_bg_mem, twoCloudPal, twoCloudPalLen);
                dma3_cpy(tile_mem[0], twoCloudTiles, twoCloudTilesLen);
                dma3_cpy(se_mem[30], twoCloudMap, twoCloudMapLen);
                sprite.jumps = 0;
                second_level= true;
                start = false;
            }

            if(sprite.pos_y > 160 || (coin.currentScore == 3 && second_level) || sprite.lives == 0)
            {
                mmStop();
                final_screen(oam_mem, coin.currentScore, SPRITES_AMOUNT);
                sprite_place_on_rect(&sprite, blockgen_get_topmost_block8(&bgen, 0));
                sprite_coin_init_with_colis(&coin, &obj_buffer[1],&sprite);

                sprite.jumps = 0;
                sprite.lives = 3;
                second_level = false;
                start = false;
                trap_hide(&trap);
                heart_hide(&heart);
                enemy_hide(&enemy1);
                enemy_hide(&enemy2);
            }
        }
	}
}

bool gamectrl_show_main_menu()
{
    OBJ_ATTR * oe = (OBJ_ATTR *)oam_mem;

    // Tiquicia Jump bounce text
    for(int ii = 0; ii < HWLEN; ii++)
    {
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
        sprite_trap_init(&trap, &obj_buffer[10]);
        sprite_enemy_init(&enemy1, &obj_buffer[11], 17);
        sprite_enemy_init(&enemy2, &obj_buffer[12], 18);
        sprite_heart_init(&heart, &obj_buffer[13]);

        return true;
    }

    return false;
}

void gamectrl_show_first_lvl(char * totalScore, u32 * frame_counter, int * h2Scroll)
{
    for(int i = 0; i < BLOCKS_AMOUNT; ++i)
        rect_paint(&bgen.blocks[i]);

    // If the blocks scrolled, scroll the player as well
    if(blockgen_autoscroll(&bgen)) sprite.pos_y += 1;

    Rect * rects = bgen.blocks;
    sprite_update_pos_collision(&sprite, &rects, BLOCKS_AMOUNT);
    sprite_change_animation(&sprite);

    // Change coin animation
    sprite_coin_update_pos(&coin);
    sprite_coin_change_animation(&coin);

    // Detect coin-sprite collision
    if(do_sprites_collisions(&coin,&sprite)){
        // Write in screen, position x = 0, y = 0
        snprintf(totalScore, 100, "#{P:0, 0}Coins:%02d", coin.currentScore);
        tte_write(totalScore);
    }

    sprite_coin_unhide(&coin, &sprite);

    // Move the sprites to VRAM. Player + coin + blocks
    oam_copy(oam_mem, obj_buffer, SPRITES_AMOUNT);

    *frame_counter = (*frame_counter + 1) % 5;

    // Move background vertical
    REG_BG1_SCROLL_V = *h2Scroll += *frame_counter == 0 ? 1 : 0;
}

void gamectrl_show_second_lvl(char * totalScore, u32 * frame_counter, int * h2Scroll)
{

    for(int i = 0; i < BLOCKS_AMOUNT; ++i)
        rect_paint(&bgen.blocks[i]);

   
    // If the blocks scrolled, scroll the player as well
    if(blockgen_autoscroll(&bgen)) sprite.pos_y += 1;

    Rect * rects = bgen.blocks;
    sprite_update_pos_collision(&sprite, &rects, BLOCKS_AMOUNT);
    sprite_change_animation(&sprite);

    // Change coin animation
    sprite_coin_update_pos(&coin);
    sprite_coin_change_animation(&coin);

    // 
    sprite_trap_update_pos(&trap);
    sprite_trap_change_animation(&trap);

    //Every 5 seconds enemys will change position
    if(REG_TM3D != sec){
        sec = REG_TM3D;

        if(((sec%60) % 5) == 0){
           
            sprite_enemy_change_pos(&enemy1);
            sprite_enemy_change_pos(&enemy2);
        }
        
    }

    sprite_enemy_update_pos(&enemy1);
    sprite_enemy_change_animation(&enemy1);

    //heart will show when lives are low
    if(sprite.lives == 1){
        sprite_heart_update_pos(&heart);
        sprite_heart_change_animation(&heart);
    }

    sprite_enemy_update_pos(&enemy2);
    sprite_enemy_change_animation(&enemy2);

    // Detect coin-sprite collision
    if(do_sprites_collisions(&coin,&sprite)){
        // Write in screen, position x = 0, y = 0
        snprintf(totalScore, 100, "#{P:0, 0}Coins:%02d", coin.currentScore);
        tte_write(totalScore);
    }

    // Detect trap-sprite collision
    if(do_sprites_collision(&trap, &sprite, &coin)){
        // Write in screen, position x = 0, y = 0
        snprintf(totalScore, 100, "#{P:0, 0}Coins:%02d", coin.currentScore);
        tte_write(totalScore);
    }

    // Detect enemy-sprite collision
    if(do_enemy_collision(&enemy1 ,&sprite, &coin)){
        // Write in screen, position x = 0, y = 0
        snprintf(totalScore, 100, "#{P:0, 150}Lives:%02d", sprite.lives);
        tte_write(totalScore);
    }

        // Detect enemy-sprite collision
    if(do_enemy_collision(&enemy2 ,&sprite, &coin)){
        // Write in screen, position x = 0, y = 0
        snprintf(totalScore, 100, "#{P:0, 200}Lives:%02d", sprite.lives);
        tte_write(totalScore);
    }

         // Detect heart-sprite collision
    if(do_heart_collision(&heart ,&sprite, &coin)){
        // Write in screen, position x = 0, y = 0
        snprintf(totalScore, 100, "#{P:0, 200}Lives:%02d", sprite.lives);
        tte_write(totalScore);
    }

    sprite_coin_unhide(&coin, &sprite);
    sprite_trap_unhide(&trap, &sprite);
    sprite_enemy_unhide(&enemy1, &sprite);
    sprite_enemy_unhide(&enemy2, &sprite);

    sprite_heart_unhide(&heart, &sprite);

    // Move the sprites to VRAM. Player + coin + blocks
    oam_copy(oam_mem, obj_buffer, SPRITES_AMOUNT);

    *frame_counter = (*frame_counter + 1) % 5;

    // Move background vertical
    REG_BG1_SCROLL_V = *h2Scroll += *frame_counter == 0 ? 1 : 0;
}