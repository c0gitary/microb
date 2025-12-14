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
    return utils_is_ext(f, ".c");
}

void
utils_remove_space(char* s)
{
    char* t = s;
    do
    {
        if(isspace(*t)) 
        {
            t++;
        }
    }
    while((*s++ = *t++));
}

int
utils_is_number(char c)
{
    return c >= '0' && c <= '9';
}