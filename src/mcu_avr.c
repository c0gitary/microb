#include "mcu.h"
#include "logger.h"

static const char* MCU_8_AVR[][2] = {
    {"attiny13", "t13"},
    {"attiny13a", "t13a"},
    {"attiny25", "t25"},
    {"attiny45", "t45"},
    {"attiny85", "t85"},
    {"attiny2313", "t2313"},
    {"attiny4313", "t4313"},
    {"atmega8", "m8"},
    {"atmega16", "m16"},
    {"atmega32", "m32"},
    {"atmega48", "m48"},
    {"atmega88", "m88"},
    {"atmega168", "m168"},
    {"atmega328", "m328"},
    {"atmega328p", "m328p"},
    {"atmega644", "m644"},
    {"atmega1284", "m1284"},
    {"atmega2560", "m2560"},
    {"atmega128", "m128"},
    {"atmega1280", "m1280"},
    {"atmega324", "m324"},
    {"atmega164", "m164"},
    {"atmega324p", "m324p"},
    {"atmega644p", "m644p"},
    {"atmega1284p", "m1284p"},
    {"atmega16u2", "m16u2"},
    {"atmega32u4", "m32u4"},
    {"attiny84", "t84"},
    {"attiny44", "t44"},
    {"attiny24", "t24"},
    {"atmega8515", "m8515"},
    {"atmega8535", "m8535"},
    {"atmega162", "m162"},
    {"atmega169", "m169"},
    {"atmega64", "m64"},
    {"atmega32a", "m32a"},
    {"attiny167", "t167"},
    {"attiny87", "t87"},
    {"atmega88p", "m88p"},
    {"atmega168p", "m168p"},
    {"atmega328pb", "m328pb"},
    {"atmega164pa", "m164pa"},
    {"atmega324pa", "m324pa"},
    {"atmega644pa", "m644pa"},
    {"atmega1284pa", "m1284pa"}
};

static const unsigned char mcu_8_avr_sz = sizeof(MCU_8_AVR) / sizeof(MCU_8_AVR[0]);


struct mcu_data 
get_mcu(const char* name)
{
    if(name)
    {
        for(unsigned char i = 0; i < mcu_8_avr_sz; i++)
        {
            if(strcmp(MCU_8_AVR[i][0], name) == 0)
            {
                mb_log(MB_LOG_LEVEL_INFO, "MCU", "Find MCU name '%s', code '%s'", MCU_8_AVR[i][0], MCU_8_AVR[i][1]);
                return (struct mcu_data){.name = MCU_8_AVR[i][0], .code = MCU_8_AVR[i][1]};
            }
        }
    }
    else {
        mb_log(MB_LOG_LEVEL_ERROR, "MCU", "MCU name not found");
    }

    return (struct mcu_data){0, 0};
}