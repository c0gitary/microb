#ifndef __MICROB_UTILS_H__
#define __MICROB_UTILS_H__

#include <dirent.h>
#include <string.h>
#include <ctype.h>

int utils_is_ext(struct dirent** f, const char* ext);

int utils_is_src_file(struct dirent** f);

int utils_is_back(struct dirent** f);

void utils_remove_space(char* s);

int utils_is_number(char c);

#endif // __MICROB_UTILS_H__
