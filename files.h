
#ifndef MICROB_FILES_H
#define MICROB_FILES_H

#include "config.h"
#include "utils.h"

#include <stdlib.h>
#include <string.h>

struct Files
{
    char** src;
    char** obj;
    int count_src_files;
    int count_obj_files;
};

struct Files* mb_files_init(struct Config**);

int mb_files_append(struct Files**, const char*);

void mb_files_free(struct Files**);

#endif // MICROB_FILES_H
