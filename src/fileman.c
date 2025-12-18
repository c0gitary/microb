#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>

#include "fileman.h"
#include "utils.h"
#include "logger.h"


struct fileman*
mb_fm_init(const char* path)
{
	if(!path) return NULL;

	mb_log(MB_LOG_LEVEL_INFO, "Fileman", "%s", path);

	struct fileman* fm = (struct fileman*)malloc(sizeof(struct fileman));

	fm->path = path;
	fm->count_files = fm->count_src_files = 0;
	fm->src_files = NULL;

	DIR* dir = opendir(fm->path);

	struct dirent* entry;

	while((entry = readdir(dir)))
	{
		char* ext = strchr(entry->d_name, '.');
		ext++;

		if(!strcmp(ext, ".") || !strcmp(ext, "")) continue;

		if(strcmp(ext, "c") == 0)	
		{
			fm->count_src_files++;
			fm->src_files = (char*)realloc(fm->src_files, strlen(entry->d_name) + 1);
			
			sprintf(fm->src_files, "%s ", entry->d_name);
			mb_log(MB_LOG_LEVEL_INFO, "Fileman", "Found source files '%s'", entry->d_name);
		}

		fm->count_files++;
	}

	closedir(dir);

	return fm;
};

void
mb_fm_free(struct fileman** fm)
{
	if(fm && *fm)
	{
		if((*fm)->count_files)
		{
			(*fm)->count_files = 0;
			free((*fm)->src_files);
			(*fm)->src_files = NULL;
		}
		free(*fm);
		*fm = NULL;
	}
}
