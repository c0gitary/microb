#ifndef __MICROB_MCU_H__
#define __MICROB_MCU_H__

#include <string.h>

struct mcu_data
{
    const char* name;
    const char* code;
};

struct mcu_data get_mcu(const char* name);


#endif // __MICROB_MCU_H__