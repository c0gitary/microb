#ifndef __MICROB_LOGGER_H__
#define __MICROB_LOGGER_H__

#include <stdio.h>
#include <stdarg.h>

enum 
{
    MB_LOG_LEVEL_ERROR,
    MB_LOG_LEVEL_WARN,
    MB_LOG_LEVEL_INFO
};


void mb_log(int log_level, const char* fmt, ...);


#endif // __MICROB_LOGGER_H__