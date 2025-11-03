
#ifndef MICROB_BUILDER_H
#define MICROB_BUILDER_H

#include "config.h"
#include "utils.h"
#include "files.h"

struct Builder
{
    struct Files* files;
    struct Config* config;
};


enum BuilderType
{
    BT_MAKEFILE,
    BT_CMAKE,
    BT_CMDLINE,
};


struct Builder* mb_builder_init(struct Config**);

int mb_builder_run(struct Builder**, enum BuilderType);

int mb_builder_exec(const char*);

int mb_builder_cmdline(struct Builder**);

int mb_builder_free(struct Builder**);


#endif // MICROB_BUILDER_H
