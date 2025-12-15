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

#define MB_LOG_TEMPLATE_GENERIC(__data) "[" __data MB_COLOR_RESET "]\t"
#define MB_LOG_TEMPLATE_INFO     MB_LOG_TEMPLATE_GENERIC(MB_COLOR_GREEN "INFO")
#define MB_LOG_TEMPLATE_ERROR    MB_LOG_TEMPLATE_GENERIC(MB_COLOR_RED "ERROR")
#define MB_LOG_TEMPLATE_WARN     MB_LOG_TEMPLATE_GENERIC(MB_COLOR_YELLOW "WARN")

#define MB_LOG_TEMPLAET_MODULE_BEGIN "[" MB_UNDERLINE
#define MB_LOG_TEMPLAET_MODULE_END MB_COLOR_RESET "] "

void
mb_log(int log_level, const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    switch(log_level)
    {
        case MB_LOG_LEVEL_INFO:
        {
            printf(MB_LOG_TEMPLATE_INFO);
            vprintf(fmt, args);
            printf("\n");
            break;
        }

        case MB_LOG_LEVEL_ERROR:
        {
            printf(MB_LOG_TEMPLATE_ERROR);
            vprintf(fmt, args);
            printf("\n");
            break;
        }
        
        case MB_LOG_LEVEL_WARN:
        {
            printf(MB_LOG_TEMPLATE_WARN);
            vprintf(fmt, args);
            printf("\n");
            break;
        }
        default: {}
    }
    va_end(args);
}