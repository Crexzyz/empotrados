#include "DisplayController.h"

#include <tonc.h>
#include <string.h>

void DispCtrl_init(DispCtrl * dc, char * title, u8 t_len, OptionFunction * func_buffer)
{
    dc->title = title;
    dc->title_len = t_len;
    dc->content_change = false;
    stack_init(&dc->stack, func_buffer);
}

void DispCtrl_show(DispCtrl * dc)
{
    tte_write("#{es}");
    size_t buf_size = strlen(TONC_TITLE_POS) + dc->title_len + 5;
    char buf[buf_size];
    snprintf(buf, buf_size, "%s%s (%d)", TONC_TITLE_POS, dc->title, dc->stack.top);
    tte_write(buf);
    
    OptionFunction * function = stack_peek(&dc->stack);

    if(function)
    {
        (function->func)(function->data);
    }
}

void DispCtrl_back(DispCtrl * dc)
{
    dc->content_change = true;
    stack_pop(&dc->stack);
}