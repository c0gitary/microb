#include "logger.h"

#define MB_COLOR_RED     "\x1B[31m"
#define MB_COLOR_GREEN   "\x1B[32m"
#define MB_COLOR_YELLOW  "\x1B[33m"
#define MB_COLOR_BLUE    "\x1B[34m"
#define MB_COLOR_MAGENTA "\x1B[35m"
#define MB_COLOR_CYAN    "\x1B[36m"
#define MB_COLOR_WHITE   "\x1B[37m"
#define MB_COLOR_RESET   "\x1B[0m" 
#define MB_UNDERLINE     "\x1B[4m"
#define MB_BOLD     "\x1B[1m"

#define MB_LOG_TEMPLATE_GENERIC(__data) "[" __data MB_COLOR_RESET "] "
#define MB_LOG_TEMPLATE_INFO     MB_LOG_TEMPLATE_GENERIC(MB_COLOR_GREEN MB_BOLD "I")
#define MB_LOG_TEMPLATE_ERROR    MB_LOG_TEMPLATE_GENERIC(MB_COLOR_RED MB_BOLD "E")
#define MB_LOG_TEMPLATE_WARN     MB_LOG_TEMPLATE_GENERIC(MB_COLOR_YELLOW MB_BOLD "W")
#define MB_LOG_TEMPLATE_MODULE   "[%12s] "

#define MB_LOG_INFO_PREFIX      MB_LOG_TEMPLATE_INFO    MB_LOG_TEMPLATE_MODULE
#define MB_LOG_WARN_PREFIX      MB_LOG_TEMPLATE_WARN    MB_LOG_TEMPLATE_MODULE
#define MB_LOG_ERROR_PREFIX     MB_LOG_TEMPLATE_ERROR   MB_LOG_TEMPLATE_MODULE

void
mb_log(const enum log_level log_level, const char* module, const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    switch(log_level)
    {
        case MB_LOG_LEVEL_INFO:
        {
            printf(MB_LOG_INFO_PREFIX, module);
            break;
        }

        case MB_LOG_LEVEL_WARN:
        {
            printf(MB_LOG_WARN_PREFIX, module);
            break;
        }
        
        case MB_LOG_LEVEL_ERROR:
        {
            printf(MB_LOG_ERROR_PREFIX, module);
            break;
        }
        default: 
        {
            va_end(args);
            return;
        }
    }
    vprintf(fmt, args);
    printf("\n");
    va_end(args);
}