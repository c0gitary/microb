#ifndef __MICROB_LOGGER_H__
#define __MICROB_LOGGER_H__

#include <stdio.h>
#include <stdarg.h>

enum log_level
{
    MB_LOG_LEVEL_ERROR,
    MB_LOG_LEVEL_WARN,
    MB_LOG_LEVEL_INFO
};


void mb_log(const enum log_level log_level, const char* module, const char* fmt, ...);


#endif // __MICROB_LOGGER_H__