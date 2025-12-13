#include "config.h"

#define MB_CONFIG_ATTR_FREE(__x) if(__x) free(__x)

static const char* const __config_file = "microb.ini";

#define size_cgk 11
static const char* const 
__config_general_keys[size_cgk] = {
    "project_name",
    "mcu_name",
    "mcu_freq",
    "compiler",
    "linker",
    "uploader",
    "programmer",
    "build_dir",
    "bin_dir",
    "include_dir",
    "baud"
};


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
        while(isspace(*t)) t++;
        if(*t == '=') break; 
    } 
    while((*s++ = *t++));
}


struct config*
mb_config_init(const char* path)
{
    struct config* cfg = (struct config*)malloc(sizeof(struct config));
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
mb_config_parse(struct config** cfg)
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

        while(fgets(line, 256, config))
        {
            char* eqsign = strchr(line, '=');
            if(!eqsign) continue;

            __config_remove_spaces(line);

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

                    switch(i)
                    {
                    case MB_CFG_PROJECT_NAME:
                    {
                        (*cfg)->project_name = val;
                        break;
                    }

                    case MB_CFG_MCU_NAME:
                    {
                        (*cfg)->mcu_name = val;
                        break;
                    }

                    case MB_CFG_PROGRAMMER:
                    {
                        (*cfg)->programmer = val;
                        break;
                    }

                    case MB_CFG_BUILD_DIR:
                    {
                        (*cfg)->build_dir = val;
                        break;
                    }

                    case MB_CFG_BIN_DIR:
                    {
                        (*cfg)->bin_dir = val;
                        break;
                    }

                    case MB_CFG_INCLUDE_DIR:
                    {
                        (*cfg)->include_dir = val;
                        break;
                    }

                    case MB_CFG_COMPILER:
                    {
                        (*cfg)->compiler = val;
                        break;
                    }

                    case MB_CFG_LINKER:
                    {
                        (*cfg)->linker = val;
                        break;
                    }

                    case MB_CFG_UPLOADER:
                    {
                        (*cfg)->uploader = val;
                        break;
                    }

                    case MB_CFG_BAUD:
                    {
                        (*cfg)->baud = (uint32_t)atoi(val);
                        break;
                    }

                    case MB_CFG_MCU_FREQ:
                    {
                        (*cfg)->mcu_freq= (uint32_t)atoi(val);
                        break;
                    }

                    default: {}
                    }

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
mb_config_print(struct config** cfg)
{
    if(cfg && *cfg)
    {
        mb_log(MB_LOG_LEVEL_INFO, "Project '%s'", (*cfg)->project_name);
        mb_log(MB_LOG_LEVEL_INFO, "Programmer '%s'", (*cfg)->programmer);
        mb_log(MB_LOG_LEVEL_INFO, "MCU '%s'", (*cfg)->mcu_name);
        mb_log(MB_LOG_LEVEL_INFO, "MCU Freq '%u'", (*cfg)->mcu_freq);
        mb_log(MB_LOG_LEVEL_INFO, "Baud '%u'", (*cfg)->baud);
        mb_log(MB_LOG_LEVEL_INFO, "Build dir '%s'", (*cfg)->build_dir);
        mb_log(MB_LOG_LEVEL_INFO, "Binary dir '%s'", (*cfg)->bin_dir);
        mb_log(MB_LOG_LEVEL_INFO, "Include dir '%s'", (*cfg)->include_dir);
        mb_log(MB_LOG_LEVEL_INFO, "Compiler '%s'", (*cfg)->compiler);
        mb_log(MB_LOG_LEVEL_INFO, "Linker '%s'", (*cfg)->linker);
        mb_log(MB_LOG_LEVEL_INFO, "Uploader '%s'", (*cfg)->uploader);
    }
}

void
mb_config_free(struct config** cfg)
{
    if(cfg && *cfg)
    {   
        MB_CONFIG_ATTR_FREE((*cfg)->project_name);
        MB_CONFIG_ATTR_FREE((*cfg)->mcu_name);
        MB_CONFIG_ATTR_FREE((*cfg)->programmer);
        MB_CONFIG_ATTR_FREE((*cfg)->compiler);
        MB_CONFIG_ATTR_FREE((*cfg)->linker);
        MB_CONFIG_ATTR_FREE((*cfg)->uploader);
        MB_CONFIG_ATTR_FREE((*cfg)->build_dir);
        MB_CONFIG_ATTR_FREE((*cfg)->bin_dir);
        MB_CONFIG_ATTR_FREE((*cfg)->include_dir);
        free(*cfg);
        *cfg = NULL;
    }
}
