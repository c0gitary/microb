#include "utils.h"

int
utils_is_ext(struct dirent** f, const char* ext)
{
    return !strcmp(&(*f)->d_name[strlen((*f)->d_name) - 2], ext);
}

int
utils_is_back(struct dirent** f)
{
    return !strcmp((*f)->d_name, ".");
}

int
utils_is_src_file(struct dirent** f)
{
    return (
        utils_is_ext(f, ".c") ||
        utils_is_ext(f, ".S") ||
        utils_is_ext(f, ".s") ||
        utils_is_ext(f, ".asm")
    );
}


char*
utils_change_ext(const char* str, const char* ext)
{
    char* newstr = (char*)malloc(strlen(str) + strlen(ext) + 1);
    strcpy(newstr, str);
    strcpy(&newstr[strlen(str)], ext);
    return newstr;
}
