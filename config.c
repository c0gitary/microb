#include "config.h"

// static const char* const __config_data = "project_name = 1\nmcu = 2\nprogrammer = as\nbaud = 4\n";

static const char* const __config_file = "microb.ini";

#define size_cgk 8
static const char* const __config_general_keys[size_cgk] = {
    "project",
    "mcu",
    "programmer",
    "baud",
    "freq",
    "compiler",
    "linked",
    "flasher"
};


// static
// void
// __config_parse_line(char* line, char** key, char** val)
// {
//     while((*line++))
//     {

//     }

//     char* equal_sign = strchr(line, '=');

//     if(!equal_sign) return;

//     size_t key_len = equal_sign - line;

//     *key = (char*)malloc(key_len + 1);
//     strncpy(*key, line, key_len);
//     (*key)[key_len] = '\0';

//     char* val_start = equal_sign + 1;
//     size_t val_len = strlen(val_start);
//     *val = (char*)malloc(val_len + 1);
//     strcpy(*val, val_start);
//     (*val)[val_len] = '\0';
// }
//

static inline
int
__config_find_cfg_file(const char* path)
{
    if(!path) return 0;
    DIR* dir = opendir(path);
    struct dirent* ent;
    while((ent = readdir(dir)))
    {
        if(strcmp(ent->d_name, __config_file) == 0)
            return 1;
    }
    return 0;
}


static
void
__config_remove_spaces(char* s)
{
    char* t = s;
    do
    {
        while(isspace(*t))
            t++;
    } while((*s++ = *t++));
}


struct Config*
mb_config_init(const char* path)
{
    struct Config* cfg = (struct Config*)malloc(sizeof(struct Config));
    cfg->path = path;

    if(__config_find_cfg_file(path)) return cfg;

    FILE* cfg_file = fopen(__config_file, "w");

    for(int i = 0; i < size_cgk; i++)
    {
        fputs(__config_general_keys[i], cfg_file);
        fputs(" = \n", cfg_file);
    }

    fclose(cfg_file);

    return cfg;
}


int
mb_config_parse(struct Config** cfg)
{
    if(cfg && *cfg && (*cfg)->path)
    {

        FILE* config = NULL;
        DIR* dir = opendir((*cfg)->path);
        struct dirent* entry;

        while((entry = readdir(dir)))
        {
            if(strcmp(entry->d_name, __config_file) == 0)
            {
                config = fopen(entry->d_name, "r");
                break;
            }
        }

        closedir(dir);

        if(config == NULL)
            return 0;

        char* line = (char*)malloc(256);
        size_t len = 0;
        ssize_t read;

        while((read = getline(&line, &len, config)) != (-1))
        {
            __config_remove_spaces(line);
            char* eqsign = strchr(line, '=');
            if(!eqsign) return 1;

            size_t keylen = eqsign - line;

            char* key = (char*)malloc(keylen + 1);
            memcpy(key, line, keylen);
            key[keylen] = '\0';

            for(int i = 0; i < size_cgk; i++)
            {
                if(!strcmp(key, __config_general_keys[i]))
                {
                    char* valbegin = eqsign + 1;
                    size_t vallen = strlen(valbegin);
                    char* val = (char*)malloc(vallen + 1);
                    memcpy(val, valbegin, vallen);
                    val[vallen] = '\0';

                    // printf("--- %s\n", val);

                    switch(i)
                    {
                    case MB_CFG_PROJECT:
                    {
                        (*cfg)->project_name = val;
                        break;
                    }

                    case MB_CFG_MCU:
                    {
                        (*cfg)->mcu = val;
                        break;
                    }

                    case MB_CFG_PROGRAMMER:
                    {
                        (*cfg)->programmer = val;
                        break;
                    }

                    case MB_CFG_BAUD:
                    {
                        (*cfg)->baud = val;
                        break;
                    }

                    case MB_CFG_FREQ:
                    {
                        (*cfg)->freq = val;
                        break;
                    }

                    case MB_CFG_CC:
                    {
                        (*cfg)->cc = val;
                        break;
                    }

                    case MB_CFG_LD:
                    {
                        (*cfg)->ld = val;
                        break;
                    }

                    case MB_CFG_FL:
                    {
                        (*cfg)->fl = val;
                        break;
                    }

                    default: {}
                    }

                    // free(val);

                }
            }

            free(key);
        }

        fclose(config);
        if(line)
            free(line);

        return 1;
    }

    return 0;
}

void
mb_config_print(struct Config** cfg)
{
    if(cfg && *cfg)
    {
        printf("project -> %s\n", (*cfg)->project_name);
        printf("mcu -> %s\n", (*cfg)->mcu);
        printf("programmer -> %s\n", (*cfg)->programmer);
        printf("freq -> %s\n", (*cfg)->freq);
        printf("baud -> %s\n", (*cfg)->baud);
    }
}


void
mb_config_free(struct Config** cfg)
{
    if(cfg && *cfg)
    {
        if((*cfg)->programmer) free((*cfg)->programmer);
        if((*cfg)->mcu) free((*cfg)->mcu);
        if((*cfg)->project_name) free((*cfg)->project_name);

        free(*cfg);
        *cfg = NULL;
    }
}
