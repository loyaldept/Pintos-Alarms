#include "debug.h"
#include <stdio.h>
#include <stdarg.h>
static bool debug_enabled_flag = false;

void put_debug(const char *format, ...)
{
    if (debug_enabled_flag) {
        va_list args;
        va_start(args, format);
        vprintf(format, args);
        va_end(args);
    }
}

void debug_init(void)
{
    debug_enabled_flag = true;
}

bool debug_enabled(void)
{
    return debug_enabled_flag;
}

void debug_set_enabled(bool enabled)
{
    debug_enabled_flag = enabled;
}