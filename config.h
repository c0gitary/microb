
#ifndef MICROB_CONFIG_H
#define MICROB_CONFIG_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <ctype.h>

enum ConfigType
{
    MB_CFG_PROJECT,
    MB_CFG_MCU,
    MB_CFG_PROGRAMMER,
    MB_CFG_BAUD,
    MB_CFG_FREQ,
    MB_CFG_CC,
    MB_CFG_LD,
    MB_CFG_FL,
};

struct Config
{
    const char* path;

    char* mcu;
    char* project_name;
    char* programmer;
    char* baud;
    char* freq;
    char* cc;
    char* ld;
    char* fl;
};

struct Config* mb_config_init(const char*);

int mb_config_parse(struct Config**);

void mb_config_print(struct Config**);

void mb_config_free(struct Config**);

#endif // MICROB_CONFIG_H
