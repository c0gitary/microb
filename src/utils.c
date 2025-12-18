#include "utils.h"
#include "ctype.h"

static const char* const extSrcFile[] = {"c", "s"};

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
    for(int i = 0; i < 2; i++)
    {
        if(utils_is_ext(f, extSrcFile[i]))
        {
            return 1;
        }
    }
    return 0;
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