#include "DisplayController.h"
#include "Alarm.h"
#include "Runner.h"

#include <tonc.h>
#include <string.h>

void DispCtrl_init(DispCtrl * dc, char * title, u8 t_len, OptionFunction * func_buffer)
{
    dc->title = title;
    dc->title_len = t_len;
    dc->content_change = false;
    stack_init(&dc->stack, func_buffer);
}

void open_alarm_alert(DispCtrl * dc)
{
	DispCtrl_push(dc, run_alarm_alert);
}

void DispCtrl_show(DispCtrl * dc)
{
    tte_write("#{es}");
    size_t buf_size = strlen(TONC_TITLE_POS) + dc->title_len + 5;
    char buf[buf_size];
    snprintf(buf, buf_size, "%s%s (%d)", TONC_TITLE_POS, dc->title, dc->stack.top);
    tte_write(buf);
    
    if(alarm_raised)
        open_alarm_alert(dc);

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

void DispCtrl_push(DispCtrl * dc, void (*func))
{
    OptionFunction function;
	OptFunc_init(&function, func, dc);
	stack_push(&dc->stack, &function);
}