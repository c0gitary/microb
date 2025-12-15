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
#define GMK_MCU_NAME__AVR "MCU_NAME"
#define GMK_MCU_CODE__AVR "MCU_CODE"

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
#define GMK_MCU__CC 


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


static const char* const gmk_data__toolchain[][3] = {
    { // __AVR
        GMK_DEFAULT_COMPILER__AVR,
        GMK_DEFAULT_LINKER__AVR,
        GMK_DEFAULT_UPLOADER__AVR
    }
};

#define GMK_GET__TOOLCHAIN(__gtype, __id) gmk_data__toolchain[__gtype][__id - GMK_ID_COMPILER]

#define GMK_WRITE_VAR(__attr, __val, __mk) fprintf(__mk, "%s = %s\n\n", __attr, __val)


static inline void 
gmk_set(const enum gmk_type gtype, const enum gmk_id id, const char* attr, const struct mcu_data mcu, FILE* mk)
{
    if(!attr)
    {
        if(gmk_data[id][1] == NULL)
        {
            if(id >= GMK_ID_COMPILER)
            {
                mb_log(MB_LOG_LEVEL_WARN, "Generator", "%s not found, set default '%s'", gmk_data[id][0], GMK_GET__TOOLCHAIN(gtype, id));         
                GMK_WRITE_VAR(gmk_data[id][0], GMK_GET__TOOLCHAIN(gtype, id), mk);
                return;        
            }

            else 
            {
                mb_log(MB_LOG_LEVEL_ERROR, "Generator", "Value not found '%s'", gmk_data[id][0]);
                return;
            }
        }
        
        else
        {
            mb_log(MB_LOG_LEVEL_WARN, "Generator", "%s not found, set default '%s'", gmk_data[id][0], gmk_data[id][1]);         
            GMK_WRITE_VAR(gmk_data[id][0], gmk_data[id][1], mk);
            return;
        }
    }

    if(id == GMK_ID_MCU_NAME)
    {
        GMK_WRITE_VAR(GMK_MCU_NAME__AVR, mcu.name, mk);
        GMK_WRITE_VAR(GMK_MCU_CODE__AVR, mcu.code, mk);
        return;
    }

    GMK_WRITE_VAR(gmk_data[id][0], attr, mk);
}


int 
mb_generator__makefile(const struct config* config, enum gmk_type gtype)
{
    if(config)
    {
        switch (gtype)
        {
            case GMK_TYPE__AVR:
            {                
                mb_log(MB_LOG_LEVEL_INFO, "Generator", "Generator Makefile type 'AVR'");
                FILE* makefile = fopen("Makefile", "w");
                if(makefile == NULL)
                {
                    mb_log(MB_LOG_LEVEL_ERROR,"Generator", "Makefile not openning");
                    return 0;
                }

                const struct mcu_data mcu = get_mcu(config->mcu_name);

                if(mcu.code == 0 || mcu.name == 0)
                {
                    return 0;
                }
  
                gmk_set(GMK_TYPE__AVR, GMK_ID_PROJECT_NAME, config->project_name, mcu, makefile);
                gmk_set(GMK_TYPE__AVR, GMK_ID_BAUD, config->baud, mcu, makefile);
                gmk_set(GMK_TYPE__AVR, GMK_ID_MCU_NAME, config->mcu_name, mcu, makefile);
                gmk_set(GMK_TYPE__AVR, GMK_ID_MCU_FREQ, config->mcu_freq, mcu, makefile);
                gmk_set(GMK_TYPE__AVR, GMK_ID_COMPILER, config->compiler, mcu, makefile);
                gmk_set(GMK_TYPE__AVR, GMK_ID_LINKER, config->linker, mcu, makefile);
                gmk_set(GMK_TYPE__AVR, GMK_ID_UPLOADER, config->uploader, mcu, makefile);
                gmk_set(GMK_TYPE__AVR, GMK_ID_BINARY_DIR, config->bin_dir, mcu, makefile);
                gmk_set(GMK_TYPE__AVR, GMK_ID_BUILD_DIR, config->build_dir, mcu, makefile);
                gmk_set(GMK_TYPE__AVR, GMK_ID_SOURCE_DIR, config->src_dir, mcu, makefile);

                fclose(makefile);

                break;  
            }
        }
        
        return 1;
    }


    mb_log(MB_LOG_LEVEL_ERROR, "Generator", "Config not found");
    return 0;
}