#include "generator.h"

#define EQCONCAT(__a, __b) __a " = " __b

#define GMK_DEFAULT_PROJECT_NAME "firmware"
#define GMK_DEFAULT_BINARY_DIR "bin"
#define GMK_DEFAULT_BUILD_DIR "build"
#define GMK_DEFAULT_INCLUDE_DIR "include"
#define GMK_DEFAULT_SOURCE_DIR "src"
#define GMK_DEFAULT_BAUD "9600"

#define GMK_DEFAULT_COMPILER__AVR "avr-gcc"
#define GMK_DEFAULT_LINKER__AVR "avr-objcopy"
#define GMK_DEFAULT_UPLOADER__AVR "avrdude"

#define GMK_PROJECT_NAME "PROJECT_NAME"
#define GMK_BINARY_DIR "BIN_DIR"
#define GMK_BUILD_DIR "BUILD_DIR"
#define GMK_SOURCE_DIR "SOURCE_DIR"
#define GMK_BAUD "BAUD"
#define GMK_MCU_NAME "MCU"
#define GMK_MCU_FREQ "MCU_FREQ"
#define GMK_COMPILER "CC"
#define GMK_LINKER "LD"
#define GMK_UPLOADER "UPLOADER"


enum gmk_id
{
    GMK_ID_NULL = -1,
    GMK_ID_PROJECT_NAME,
    GMK_ID_BINARY_DIR,
    GMK_ID_BUILD_DIR,
    GMK_ID_SOURCE_DIR,
    GMK_ID_BAUD,
    GMK_ID_MCU_NAME,
    GMK_ID_MCU_FREQ,
    GMK_ID_COMPILER,
    GMK_ID_LINKER,
    GMK_ID_UPLOADER
};


static const char* const gmk_data[][2] = {
    {GMK_PROJECT_NAME, GMK_DEFAULT_PROJECT_NAME},
    {GMK_BINARY_DIR, GMK_DEFAULT_BINARY_DIR},
    {GMK_BUILD_DIR, GMK_DEFAULT_BUILD_DIR},
    {GMK_SOURCE_DIR, GMK_DEFAULT_SOURCE_DIR},
    {GMK_BAUD, GMK_DEFAULT_BAUD},
    {GMK_MCU_NAME, NULL},
    {GMK_MCU_FREQ, NULL},
    {GMK_COMPILER, NULL},
    {GMK_LINKER, NULL},
    {GMK_UPLOADER, NULL}
};


static const char* const gmk_data__toolchain[] = {
    GMK_DEFAULT_COMPILER__AVR,
    GMK_DEFAULT_LINKER__AVR,
    GMK_DEFAULT_UPLOADER__AVR
};


static void 
GMK_SET(const enum gmk_type gtype, const enum gmk_id id, const char* attr, FILE* mk)
{
    if(!attr)
    {
        if(gmk_data[id][1] == NULL)
        {
            if(id >= GMK_ID_COMPILER)
            {
                mb_log(MB_LOG_LEVEL_WARN, "%s not found, set default '%s'", gmk_data[id][0], gmk_data__toolchain[id + gtype]);         
                fputs(gmk_data[id][0], mk);
                fputs(" = ", mk);
                fputs(gmk_data__toolchain[id + gtype], mk);
                fputs("\n\n", mk);
                return;        
            }
            else 
            {
                mb_log(MB_LOG_LEVEL_ERROR, "Value not found '%s'", gmk_data[id][0]);
                return;
            }
        }
        
        else
        {
            mb_log(MB_LOG_LEVEL_WARN, "%s not found, set default '%s'", gmk_data[id][0], gmk_data[id][1]);         
            fputs(gmk_data[id][0], mk);
            fputs(" = ", mk);
            fputs(gmk_data[id][1], mk);
            fputs("\n\n", mk);
            return;
        }
    }

    fputs(gmk_data[id][0], mk);
    fputs(" = ", mk);
    fputs(attr, mk);
    fputs("\n\n", mk);
}


int 
mb_generator__makefile(const struct config* config, const enum gmk_type gtype)
{
    if(config)
    {
        switch (gtype)
        {
            case GMK_TYPE__AVR:
            {                
                FILE* makefile = fopen("Makefile", "w");
                if(makefile == NULL)
                {
                    mb_log(MB_LOG_LEVEL_ERROR, "Makefile not openning");
                    return 0;
                }
  
                GMK_SET(GMK_TYPE__AVR, GMK_ID_PROJECT_NAME, config->project_name, makefile);
                GMK_SET(GMK_TYPE__AVR, GMK_ID_BAUD, config->baud, makefile);
                GMK_SET(GMK_TYPE__AVR, GMK_ID_MCU_NAME, config->mcu_name, makefile);
                GMK_SET(GMK_TYPE__AVR, GMK_ID_MCU_FREQ, config->mcu_freq, makefile);
                GMK_SET(GMK_TYPE__AVR, GMK_ID_COMPILER, config->compiler, makefile);
                GMK_SET(GMK_TYPE__AVR, GMK_ID_LINKER, config->linker, makefile);
                GMK_SET(GMK_TYPE__AVR, GMK_ID_UPLOADER, config->uploader, makefile);
                GMK_SET(GMK_TYPE__AVR, GMK_ID_BINARY_DIR, config->bin_dir, makefile);
                GMK_SET(GMK_TYPE__AVR, GMK_ID_BUILD_DIR, config->build_dir, makefile);
                GMK_SET(GMK_TYPE__AVR, GMK_ID_SOURCE_DIR, config->src_dir, makefile);

                fclose(makefile);

                break;  
            }
        }
        
        return 1;
    }


    mb_log(MB_LOG_LEVEL_ERROR, "[Generator Makefile]\tConfig not found");
    return 0;
}