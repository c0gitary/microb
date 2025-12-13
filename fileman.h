#ifndef __MICROB_FILEMAN_H__
#define __MICROB_FILEMAN_H__

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>

#include "config.h"
#include "utils.h"

struct fileman
{
	const char* path;
	int count_src_files;
	int count_files;
};


struct fileman* mb_fm_init(const char* path);

int mb_fm_create_makefile(struct fileman** fm, struct config** cfg);

void mb_fm_print(struct fileman** fm);

void mb_fm_free(struct fileman** fm);


#endif // __MICROB_FILEMAN_H__
