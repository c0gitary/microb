#ifndef __MICROB_FILEMAN_H__
#define __MICROB_FILEMAN_H__

#include "config.h"

struct fileman
{
	const char* path;
	char* src_files;
	int count_src_files;
	int count_files;
};


struct fileman* mb_fm_init(const char* path);

void mb_fm_free(struct fileman** fm);


#endif // __MICROB_FILEMAN_H__
