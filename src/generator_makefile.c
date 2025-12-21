#include "generator.h"
#include "logger.h"
#include "mcu.h"

#define EQCONCAT(__a, __b) __a " = " __b

#define GMK_DEFAULT_PROJECT_NAME "firmware"
#define GMK_DEFAULT_BINARY_DIR "bin"
#define GMK_DEFAULT_BUILD_DIR "build"
#define GMK_DEFAULT_INCLUDE_DIR "include"
#define GMK_DEFAULT_SOURCE_DIR "src"
#define GMK_DEFAULT_BAUD "9600"

#define GMK_DEFAULT_COMPILER__AVR "avr-gcc"
#define GMK_DEFAULT_ASM__AVR "avr-as"
#define GMK_DEFAULT_OBJCOPY__AVR "avr-objcopy"
#define GMK_DEFAULT_UPLOADER__AVR "avrdude"
#define GMK_DEFAULT_CFLAGS__AVR "-Os -Wall -Wextra"
#define GMK_DEFAULT_LDFLAGS__AVR "--data-sections -mrelax --strub=all -lm"
#define GMK_DEFAULT_CXXFLAGS__AVR "-Os -Wall -Wextra"

#define GMK_PROJECT_NAME "PROJECT_NAME"
#define GMK_PROGRAMMER "PROGRAMMER"
#define GMK_BINARY_DIR "BIN_DIR"
#define GMK_BUILD_DIR "BUILD_DIR"
#define GMK_SOURCE_DIR "SOURCE_DIR"
#define GMK_PROJECT_BUILD_DIR "PROJECT_BUILD_DIR"
#define GMK_BAUD "BAUD"
#define GMK_PORT "PORT"
#define GMK_MCU_NAME "MCU"
#define GMK_MCU_CODE "MCU_CODE"
#define GMK_MCU_FREQ "MCU_FREQ"
#define GMK_CFLAGS "CFLAGS"
#define GMK_LDFLAGS "LDFLAGS"
#define GMK_CXXFLAGS "CXXFLAGS"
#define GMK_COMPILER "CC"
#define GMK_ASM "ASM"
#define GMK_OBJCOPY "OBJCOPY"
#define GMK_UPLOADER "UPLOADER"
#define GMK_SRCS "SRCS"
#define GMK_OBJS "OBJS"
#define GMK_DEPS "DEPS"
#define GMK_TARGET_ELF "ELF"
#define GMK_TARGET_HEX "HEX"


enum gmk_id
{
    GMK_ID_NULL = -1,
    GMK_ID_PROJECT_NAME,
    GMK_ID_BINARY_DIR,
    GMK_ID_BUILD_DIR,
    GMK_ID_SOURCE_DIR,
    GMK_ID_BAUD,
    GMK_ID_PORT,
    GMK_ID_PROGRAMMER,
    GMK_ID_MCU_NAME,
    GMK_ID_MCU_FREQ,
    GMK_ID_CFLAGS,
    GMK_ID_LDFLAGS,
    GMK_ID_CXXFLAGS,
    GMK_ID_COMPILER,
    GMK_ID_OBJCOPY,
    GMK_ID_UPLOADER
};


static const char* gmk_data[][2] = {
    {GMK_PROJECT_NAME, GMK_DEFAULT_PROJECT_NAME},
    {GMK_BINARY_DIR, GMK_DEFAULT_BINARY_DIR},
    {GMK_BUILD_DIR, GMK_DEFAULT_BUILD_DIR},
    {GMK_SOURCE_DIR, GMK_DEFAULT_SOURCE_DIR},
    {GMK_BAUD, GMK_DEFAULT_BAUD},
    {GMK_PORT, NULL},
    {GMK_PROGRAMMER, NULL},
    {GMK_MCU_NAME, NULL},
    {GMK_MCU_FREQ, NULL},
    {GMK_CFLAGS, NULL},
    {GMK_LDFLAGS, NULL},
    {GMK_CXXFLAGS, NULL},
    {GMK_COMPILER, NULL},
    {GMK_OBJCOPY, NULL},
    {GMK_UPLOADER, NULL}
};


static const char* const gmk_data__toolchain[][3] = {
    { // AVR
        GMK_DEFAULT_COMPILER__AVR,
        GMK_DEFAULT_OBJCOPY__AVR,
        GMK_DEFAULT_UPLOADER__AVR
    }
};


static const char* const gmk_data__flags[][3] = {
    { // AVR
        GMK_DEFAULT_CFLAGS__AVR,
        GMK_DEFAULT_LDFLAGS__AVR,
        GMK_DEFAULT_CXXFLAGS__AVR
    }
};


#define GMK_GET__TOOLCHAIN(__gtype, __id) gmk_data__toolchain[__gtype][__id - GMK_ID_COMPILER]

#define GMK_GET__FLAGS(__gtype, __id) gmk_data__flags[__gtype][__id - GMK_ID_CFLAGS]

#define GMK_WRITE_VAR(__attr, __val, __mk) fprintf(__mk, "%s = %s\n\n", __attr, __val)


static inline 
int 
gmk_set(
    const enum gmk_type gtype, 
    const enum gmk_id id, 
    const char* attr, 
    const struct mcu_data* mcu, FILE* mk)
{
    if(!attr)
    {
        if(gmk_data[id][1] == NULL)
        {
            if(id >= GMK_ID_COMPILER)
            {
                mb_log(MB_LOG_LEVEL_WARN, "Generator", "%s not found, set default '%s'", gmk_data[id][0], GMK_GET__TOOLCHAIN(gtype, id));         
                GMK_WRITE_VAR(gmk_data[id][0], GMK_GET__TOOLCHAIN(gtype, id), mk);
                return 1;
            }

            else if(id >= GMK_ID_CFLAGS && id <= GMK_ID_CXXFLAGS && gtype == GMK_TYPE__AVR)
            {
                mb_log(MB_LOG_LEVEL_WARN, "Generator", "%s not found, set default '%s'", gmk_data[id][0], GMK_GET__FLAGS(gtype, id));         
                GMK_WRITE_VAR(gmk_data[id][0], GMK_GET__FLAGS(gtype, id), mk);
                return 1;
            }

            else 
            {
                mb_log(MB_LOG_LEVEL_ERROR, "Generator", "Value not found '%s'", gmk_data[id][0]);
                return 0;
            }
        }
        
        else
        {
            mb_log(MB_LOG_LEVEL_WARN, "Generator", "%s not found, set default '%s'", gmk_data[id][0], gmk_data[id][1]);         
            GMK_WRITE_VAR(gmk_data[id][0], gmk_data[id][1], mk);
            return 1;
        }
    }

    if(gtype == GMK_TYPE__AVR)
    {

    }
    
    
    GMK_WRITE_VAR(GMK_MCU_NAME, mcu->name, mk);
    GMK_WRITE_VAR(GMK_MCU_CODE, mcu->code, mk);
    GMK_WRITE_VAR(gmk_data[id][0], attr, mk);
    return 1;
}


static inline
void
gmk_set_paths(FILE* mk)
{
    fprintf(
        mk, 
        "%s = $(%s)/$(%s)\n"
        "%s = $(wildcard $(%s)/*.c)\n" 
        "%s = $(patsubst $(%s)/%s.c, $(%s)/%s.o, $(%s))\n"
        "%s = $(%s:.c=.d)\n\n"
        "TARGET = $(%s)/$(%s).hex\n"
        "%s = $(%s)/$(%s).elf\n\n",
        GMK_PROJECT_BUILD_DIR, GMK_BUILD_DIR, GMK_PROJECT_NAME,
        GMK_SRCS, GMK_SOURCE_DIR,
        GMK_OBJS, GMK_SOURCE_DIR, "%", GMK_PROJECT_BUILD_DIR, "%", GMK_SRCS,
        GMK_DEPS, GMK_SRCS,
        GMK_BINARY_DIR, GMK_PROJECT_NAME,
        GMK_TARGET_ELF, GMK_BINARY_DIR, GMK_PROJECT_NAME
    );
}


#define GMK_SET_TARGET(t, d, f) t ": " d "\n\t" f

static inline 
void
gmk_write_targets(const enum gmk_type gtype, FILE* mk)
{
    switch(gtype)
    {
        case GMK_TYPE__AVR:
        {
            fprintf(
                mk,
                ".PHONY: all flash clean build\n"
                "all: dirs build\n\n"
                "dirs:\n\t@mkdir -p $(%s)\n\t@mkdir -p $(%s)\n\t@mkdir -p $(%s)\n\n"
                "build: $(TARGET)\n\n"
                "$(TARGET): $(%s)\n\t"
                "$(%s) -O ihex -R .eeprom $< $@\n\n"
                "$(%s): $(%s)\n\t"
                "@mkdir -p $(%s)\n\t"
                "$(%s) -mmcu=$(%s) -DF_CPU=$(%s) $(%s) -o $@ $^\n\n"
                "$(%s)/%s.o: $(%s)/%s.c\n\t"
                "@mkdir -p $(%s)\n\t"
                "$(%s) -mmcu=$(%s) -DF_CPU=$(%s) -MMD -MF $(%s)/$*.d $(%s) -c $< -o $@\n\n"
                "-include $(%s)\n\n"
                "flash: build\n\t"
                "$(%s) -c $(%s) -p $(%s) -U flash:w:$(TARGET):i\n\n"
                "clean:\n\t"
                "rm -rf $(TARGET) $(%s)\n\n",
                GMK_BINARY_DIR, GMK_BUILD_DIR, GMK_PROJECT_BUILD_DIR,
                GMK_OBJS,
                GMK_OBJCOPY,
                GMK_TARGET_ELF, GMK_OBJS,
                GMK_BINARY_DIR,
                GMK_COMPILER, GMK_MCU_NAME, GMK_MCU_FREQ, GMK_LDFLAGS,
                GMK_PROJECT_BUILD_DIR, "%",
                GMK_SOURCE_DIR, "%",
                GMK_PROJECT_BUILD_DIR,
                GMK_COMPILER, GMK_MCU_NAME, GMK_MCU_FREQ, GMK_PROJECT_BUILD_DIR, GMK_CFLAGS,
                GMK_DEPS,
                GMK_UPLOADER, GMK_PROGRAMMER, GMK_PORT,
                GMK_PROJECT_BUILD_DIR
            );
            
            return;
        }

        default:
        {
            return;
        }
    }
}


int 
mb_generator__makefile(const struct config* config, enum gmk_type gtype)
{
    if(config)
    {
        mb_log(MB_LOG_LEVEL_INFO, "Generator", "Generator Makefile type 'AVR'");
        FILE* makefile = fopen("makefile", "w");
        if(makefile == NULL)
        {
            mb_log(MB_LOG_LEVEL_ERROR,"Generator", "Makefile not openning");
            return 0;
        }
    
        const struct mcu_data mcu = get_mcu(config->mcu_name);
    
        if(!mcu.code || !mcu.name)
        {
            fclose(makefile);
            remove("Makefile");
            mb_log(MB_LOG_LEVEL_ERROR, "Generator", "Fauiled to generate Makefile");
            return 0;
        }
    
        const uint8_t status =
            gmk_set(gtype, GMK_ID_PROJECT_NAME, config->project_name, &mcu, makefile) &&
            gmk_set(gtype, GMK_ID_PROGRAMMER, config->programmer, &mcu, makefile) &&
            gmk_set(gtype, GMK_ID_PORT, config->port, &mcu, makefile) &&
            gmk_set(gtype, GMK_ID_BAUD, config->baud, &mcu, makefile) &&
            gmk_set(gtype, GMK_ID_MCU_NAME, config->mcu_name, &mcu, makefile) &&
            gmk_set(gtype, GMK_ID_MCU_FREQ, config->mcu_freq, &mcu, makefile) &&
            gmk_set(gtype, GMK_ID_COMPILER, config->compiler, &mcu, makefile) &&
            gmk_set(gtype, GMK_ID_OBJCOPY, config->objcopy, &mcu, makefile) &&
            gmk_set(gtype, GMK_ID_UPLOADER, config->uploader, &mcu, makefile) &&
            gmk_set(gtype, GMK_ID_BINARY_DIR, config->bin_dir, &mcu, makefile) &&
            gmk_set(gtype, GMK_ID_BUILD_DIR, config->build_dir, &mcu, makefile) &&
            gmk_set(gtype, GMK_ID_SOURCE_DIR, config->src_dir, &mcu, makefile) &&
            gmk_set(gtype, GMK_ID_CFLAGS, config->cflags, &mcu, makefile) &&
            gmk_set(gtype, GMK_ID_LDFLAGS, config->ldflags, &mcu, makefile) &&
            gmk_set(gtype, GMK_ID_CXXFLAGS, config->cxxflags, &mcu, makefile);
    
        if(!status)
        {
            fclose(makefile);
            remove("makefile");
            mb_log(MB_LOG_LEVEL_ERROR, "Generator", "Fauiled to generate Makefile");
            return 0;
        }
    
        mb_log(MB_LOG_LEVEL_INFO, "Generator", "%s", config->src_dir);
    
        gmk_set_paths(makefile);
        gmk_write_targets(gtype, makefile);
    
        mb_log(MB_LOG_LEVEL_INFO, "Generator", "Makefile generated successfully");
        fclose(makefile);
        return 1;
        
    }


    mb_log(MB_LOG_LEVEL_ERROR, "Generator", "Config not found");
    return 0;
}