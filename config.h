#ifndef CONF_H
#define CONF_H

#include <dirent.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "logger.h"

struct config
{
    const char* path;
    char* project_name;
    char* mcu_name;
    char* programmer;
    char* compiler;
    char* linker;
    char* uploader;
    char* build_dir;
    char* bin_dir;
    char* include_dir;

    uint32_t mcu_freq;
    uint32_t baud;
};


enum
{
    MB_CFG_PROJECT_NAME,
    MB_CFG_MCU_NAME,
    MB_CFG_MCU_FREQ,
    MB_CFG_COMPILER,
    MB_CFG_LINKER,
    MB_CFG_UPLOADER,
    MB_CFG_PROGRAMMER,
    MB_CFG_BUILD_DIR,
    MB_CFG_BIN_DIR,
    MB_CFG_INCLUDE_DIR,
    MB_CFG_BAUD
};

struct config* mb_config_init(const char* path);

int mb_config_parse(struct config** cfg);

void mb_config_print(struct config** cfg);

void mb_config_free(struct config** cfg);

#endif // CONF_H
