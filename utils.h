#ifndef MICROB_UTILS_H
#define MICROB_UTILS_H

#include <dirent.h>
#include <string.h>
#include <stdlib.h>

int utils_is_ext(struct dirent** f, const char* ext);

int utils_is_src_file(struct dirent** f);

int utils_is_back(struct dirent** f);

char* utils_change_ext(const char* str, const char* ext);


#endif //
