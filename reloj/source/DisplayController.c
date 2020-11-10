#include "DisplayController.h"

#include <tonc.h>
#include <string.h>

void DispCtrl_init(DispCtrl * dc, char * title, u8 t_len, void (*content_function), void * data)
{
    dc->title = title;
    dc->title_len = t_len;
    dc->function_data = data;
    dc->content_function = content_function;
}

void DispCtrl_show(DispCtrl * dc)
{
    size_t buf_size = strlen(TONC_TITLE_POS) + dc->title_len + 1;
    char buf[buf_size];
    snprintf(buf, buf_size, "%s%s", TONC_TITLE_POS, dc->title);
    tte_write(buf);

    if(dc->content_function)
        (dc->content_function)(dc->function_data);
}