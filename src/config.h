#ifndef CONF_H
#define CONF_H

#include <dirent.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

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
    char* src_dir;
    char* mcu_freq;
    char* baud;
};


enum config_attr
{
    MB_CONFIG_ATTR_PROJECT_NAME,
    MB_CONFIG_ATTR_MCU_NAME,
    MB_CONFIG_ATTR_MCU_FREQ,
    MB_CONFIG_ATTR_COMPILER,
    MB_CONFIG_ATTR_LINKER,
    MB_CONFIG_ATTR_UPLOADER,
    MB_CONFIG_ATTR_PROGRAMMER,
    MB_CONFIG_ATTR_BUILD_DIR,
    MB_CONFIG_ATTR_BIN_DIR,
    MB_CONFIG_ATTR_INCLUDE_DIR,
    MB_CONFIG_ATTR_SRC_DIR,
    MB_CONFIG_ATTR_BAUD
};

struct config* mb_config_init(const char* path);

int mb_config_parse(struct config** cfg);

// void mb_config_print(struct config** cfg);

void mb_config_free(struct config** cfg);

#endif // CONF_H
