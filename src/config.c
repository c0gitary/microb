#include "config.h"

#define CONFIG_ATTR_FREE(__x) if(__x) free(__x)

#define CONFIG_PUSH_VALUE(__config, __attr, __value) \
    case __attr: \
    {\
        __config = __value; \
        if(__value) mb_log(MB_LOG_LEVEL_INFO, "Config", "Set '%s'", __config); \
        else mb_log(MB_LOG_LEVEL_WARN, "Config", "Value not found [[ %s ]]", config_template_keys[__attr]);\
        break;\
    }

static const char* const __config_file = "microb.ini";

#define size_keys 12
static const char* const 
config_template_keys[size_keys] = {
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
    "source_dir",
    "baud"
};


static inline
int
config_find_cfg_file(const char* path)
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


struct config*
mb_config_init(const char* path)
{
    struct config* cfg = (struct config*)malloc(sizeof(struct config));
    cfg->path = path;

    if(config_find_cfg_file(path)) return cfg;

    FILE* cfg_file = fopen(__config_file, "w");

    for(int i = 0; i < size_keys; i++)
    {
        fputs(config_template_keys[i], cfg_file);
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

        while(fgets(line, 256, config))
        {
            // if(strcmp(line, "\n") == 0) continue;

            char* key = NULL;
            char* value = NULL;
            char* tempValue = NULL;

            const char* eqPos = strchr(line, '=');
            if(!eqPos) continue;

            const size_t keyLen = eqPos - line;
            key = (char*)malloc(keyLen + 1); 
            memcpy(key, line, keyLen);
            key[keyLen] = '\0';
            utils_remove_space(key);
            
            const size_t tempValueLen = strlen(line) - keyLen;
            tempValue = (char*)malloc(tempValueLen + 1);
            memcpy(tempValue, eqPos + 1, tempValueLen - 2);
            tempValue[tempValueLen] = '\0';   

            for(size_t i = 0; i < tempValueLen; i++)
            {
                if(tempValue[i] == '\"')
                {
                    i++;
                    
                    char* qPos = strchr(tempValue + i, '\"');
                    const size_t valueLen = strlen(tempValue + i) - strlen(qPos); 
                    
                    value = (char*)malloc(valueLen);
                    memmove(value, tempValue + i, valueLen);
                    value[valueLen] = '\0';
                    
                    free(tempValue);
                    break;
                }

                if(utils_is_number(tempValue[i]))
                {   
                    size_t valueLen = 0;
                    size_t j = i;
                    while(utils_is_number(tempValue[j]))
                    {
                        j++;
                        valueLen++;
                    }
                    value = (char*)malloc(valueLen + 1);
                    memmove(value, tempValue + i, valueLen);
                    value[valueLen] = '\0';
                    
                    free(tempValue);
                    break;
                }
            }


            for(size_t i = 0; i < size_keys; i++)
            {
                if(strcmp(key, config_template_keys[i]) == 0)
                {
                    switch(i)
                    {
                        CONFIG_PUSH_VALUE((*cfg)->project_name, MB_CONFIG_ATTR_PROJECT_NAME,value);
                        CONFIG_PUSH_VALUE((*cfg)->mcu_name,     MB_CONFIG_ATTR_MCU_NAME,    value);
                        CONFIG_PUSH_VALUE((*cfg)->programmer,   MB_CONFIG_ATTR_PROGRAMMER,  value);
                        CONFIG_PUSH_VALUE((*cfg)->compiler,     MB_CONFIG_ATTR_COMPILER,    value);
                        CONFIG_PUSH_VALUE((*cfg)->linker,       MB_CONFIG_ATTR_LINKER,      value);
                        CONFIG_PUSH_VALUE((*cfg)->uploader,     MB_CONFIG_ATTR_UPLOADER,    value);
                        CONFIG_PUSH_VALUE((*cfg)->build_dir,    MB_CONFIG_ATTR_BUILD_DIR,   value);
                        CONFIG_PUSH_VALUE((*cfg)->bin_dir,      MB_CONFIG_ATTR_BIN_DIR,     value);
                        CONFIG_PUSH_VALUE((*cfg)->src_dir,      MB_CONFIG_ATTR_SRC_DIR,     value);
                        CONFIG_PUSH_VALUE((*cfg)->mcu_freq,     MB_CONFIG_ATTR_MCU_FREQ,    value);
                        CONFIG_PUSH_VALUE((*cfg)->baud,         MB_CONFIG_ATTR_BAUD,        value);
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

// void
// mb_config_print(struct config** cfg)
// {
//     if(cfg && *cfg)
//     {
//         mb_log(MB_LOG_LEVEL_INFO, "Project '%s'", (*cfg)->project_name);
//         mb_log(MB_LOG_LEVEL_INFO, "Programmer '%s'", (*cfg)->programmer);
//         mb_log(MB_LOG_LEVEL_INFO, "MCU '%s'", (*cfg)->mcu_name);
//         mb_log(MB_LOG_LEVEL_INFO, "MCU Freq '%s'", (*cfg)->mcu_freq);
//         mb_log(MB_LOG_LEVEL_INFO, "Baud '%s'", (*cfg)->baud);
//         mb_log(MB_LOG_LEVEL_INFO, "Build dir '%s'", (*cfg)->build_dir);
//         mb_log(MB_LOG_LEVEL_INFO, "Binary dir '%s'", (*cfg)->bin_dir);
//         mb_log(MB_LOG_LEVEL_INFO, "Include dir '%s'", (*cfg)->include_dir);
//         mb_log(MB_LOG_LEVEL_INFO, "Compiler '%s'", (*cfg)->compiler);
//         mb_log(MB_LOG_LEVEL_INFO, "Linker '%s'", (*cfg)->linker);
//         mb_log(MB_LOG_LEVEL_INFO, "Uploader '%s'", (*cfg)->uploader);
//     }
// }

void
mb_config_free(struct config** cfg)
{
    if(cfg && *cfg)
    {   
        CONFIG_ATTR_FREE((*cfg)->project_name); 
        CONFIG_ATTR_FREE((*cfg)->mcu_name); 
        CONFIG_ATTR_FREE((*cfg)->programmer); 
        CONFIG_ATTR_FREE((*cfg)->compiler); 
        CONFIG_ATTR_FREE((*cfg)->linker); 
        CONFIG_ATTR_FREE((*cfg)->uploader); 
        CONFIG_ATTR_FREE((*cfg)->build_dir);
        CONFIG_ATTR_FREE((*cfg)->bin_dir); 
        CONFIG_ATTR_FREE((*cfg)->src_dir); 
        CONFIG_ATTR_FREE((*cfg)->baud); 
        free(*cfg);
        *cfg = NULL;
    }
}
