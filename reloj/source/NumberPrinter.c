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

    np->rainbow_color_index = WHITE;
    np->sprites = sprites;
    np->sprite_amount = amount;

    for(size_t sprite = 0; sprite < amount; ++sprite)
    {
        obj_set_attr(&np->sprites[sprite], ATTR0_SQUARE | ATTR0_8BPP, ATTR1_SIZE_16, ATTR2_PALBANK(0));
        obj_hide((&np->sprites[sprite]));
    }
}

u8 char_to_sprite(char character)
{
    if(character == ':')
        return COLON;
    else if('0' <= character && character <= '9')
        return character - '0';
    else
        return QUESTION_MARK;
}

void np_print(NumberPrinter * np, u32 x, u32 y, u8 color, char * numbers, u32 len)
{
    u8 num = 0;
    for(u32 sprite = 0; sprite < np->sprite_amount; ++sprite)
    {
        if(num < len)
        {
            obj_unhide((&np->sprites[sprite]), 0);

            u8 real_number = char_to_sprite(numbers[num]);

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

void np_print_highlight(NumberPrinter * np, u32 x, u32 y, u8 color, u8 highlight_color, u8 highlight_index, char * numbers, u32 len)
{
    u8 num = 0;
    for(u32 sprite = 0; sprite < np->sprite_amount; ++sprite)
    {
        if(num < len)
        {
            obj_unhide((&np->sprites[sprite]), 0);

            u8 real_number = char_to_sprite(numbers[num]);

            if(sprite == highlight_index)
                (&np->sprites[sprite])->attr2 = ATTR2_BUILD(NP_BG_BUILD(highlight_color, real_number), 0, 0);
            else
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

void np_rainbow_print(NumberPrinter * np, u32 x, u32 y, char * numbers, u32 len)
{
    u8 color = np->rainbow_color_index;

    for(size_t sprite = 0; sprite < np->sprite_amount; ++sprite)
    {
        obj_unhide(&np->sprites[sprite], 0);
        u8 real_number = sprite < len ? char_to_sprite(numbers[sprite]) : QUESTION_MARK;

        (&np->sprites[sprite])->attr2 = ATTR2_BUILD(NP_BG_BUILD(color, real_number), 0, 0);
        obj_set_pos(&np->sprites[sprite], x, y);

        x += 16;
        color = (color + 1) % (YELLOW + 1);
    }
    np->rainbow_color_index = (np->rainbow_color_index + 1) % (YELLOW + 1);
}