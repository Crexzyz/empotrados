#include "NumberPrinter.h"
#include "Nums.h"

#include <string.h>

void np_load()
{
    memcpy(&tile_mem[4][0], NumsTiles, NumsTilesLen);
	memcpy(pal_obj_mem, NumsPal, NumsPalLen);
}

void np_init(NumberPrinter * np, OBJ_ATTR * sprites, u32 amount)
{
    np_load();

    np->sprites = sprites;
    np->sprite_amount = amount;

    for(size_t sprite = 0; sprite < amount; ++sprite)
    {
        obj_set_attr(&np->sprites[sprite], ATTR0_SQUARE | ATTR0_8BPP, ATTR1_SIZE_16, ATTR2_PALBANK(0));
    }
}

void np_print(NumberPrinter * np, u32 x, u32 y, u8 color, char * numbers, u32 len)
{
    u8 num = 0;
    for(u32 sprite = 0; sprite < np->sprite_amount; ++sprite)
    {
        if(num < len)
        {
            obj_unhide((&np->sprites[sprite]), 0);

            u8 real_number = numbers[num] - '0';
            if(real_number > 9)
                real_number = 0;

            (&np->sprites[sprite])->attr2 = ATTR2_BUILD(NP_BG_BUILD(color, real_number), 0, 0);
            obj_set_pos(&np->sprites[sprite], x, y);

            x += 16;
            ++num;
        }
        else
        {
            obj_hide((&np->sprites[sprite]));
        }
        
    }
}