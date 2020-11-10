#include "OptionsChooser.h"

#include <string.h>

static char text_buffer[300];

void OptionText_init(OptionText * ot, char * text, u8 size)
{
    ot->text = text;
    ot->size = size;
}

void OptFunc_init(OptionFunction * of, void (*func), void * data)
{
    of->func = func;
    of->data = data;
}

void OptsChser_init(OptsChser * oc, OptionText * options, u8 size, OptionFunction * functions)
{
    memset(text_buffer, 0, 300);
    oc->options = options;
    oc->options_size = size <= OPTS_MAX_SIZE ? size : OPTS_MAX_SIZE;
    oc->current_option = 0;
    oc->functions = functions;
}

void OptsChser_show(OptsChser * oc)
{
    OptsChser_keypoll(oc);

    for(size_t i = 0; i < oc->options_size; ++i)
    {   
        u16 x = 20;
        u16 y = 70 + 16 * i;

        if(i == oc->current_option)
        {
            // tte_set_color(TTE_INK, CLR_SILVER);
            snprintf(text_buffer, 300, "#{P:%d,%d} -> %s", x, y, oc->options[i].text);
        }
        else
        {
            // tte_set_color(TTE_INK, CLR_YELLOW);
            snprintf(text_buffer, 300, "#{P:%d,%d}    %s", x, y, oc->options[i].text);
        }

        tte_write(text_buffer);
    }
}

void OptsChser_keypoll(OptsChser * oc)
{
    if(key_hit(KEY_DOWN))
    {
        oc->current_option = (oc->current_option + 1) % oc->options_size;
    }
    else if(key_hit(KEY_UP))
    {
        oc->current_option = oc->current_option > 0 ? oc->current_option - 1 : oc->options_size - 1;
    }
    else if(key_hit(KEY_A))
    {
        // Call function(data)
        (*oc->functions[oc->current_option].func)(oc->functions[oc->current_option].data);
    }
}