#ifndef MICROBUILDER_MAIN_H
#define MICROBUILDER_MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/stat.h>

#define MB_FLAGS_RELEASE   "-Wall -Wextra -Werror -O3 -std=c99"
#define MB_FLAGS_DEBUG     "-Wall -Wextra -Werror -O0 -std=c99"

#define MB_AVR_LINKER_FLAGS "-O ihex -R .eeprom"


#ifndef MB_SIZE
#   define MB_SIZE 1024
#endif // MB_SIZE


#define MB_DECLARE_PROJECT(a, b)
#define MB_DECLARE_AVR_CONFIG(a, b, c)

#ifndef MB_DIR_BUILD
#   define MB_DIR_BUILD "build/"
#endif // MB_DIR_BUILD

#ifndef MB_DIR_SRC
#   define MB_DIR_SRC   "src/"
#endif // MB_SRC_DIR


#ifndef MB_AVR_COMPILER
#   define MB_AVR_COMPILER "avr-gcc"
#endif // MB_COMPILER_AVR

#ifndef MB_AVR_LINKER
#   define MB_AVR_LINKER "avr-objcopy"
#endif // MB_AVR_LINKER

#ifndef MB_AVR_UPLOADER
#   define MB_AVR_UPLOADER "avrdude"
#endif // MB_AVR_UPLOADER


#ifndef MBDEF
#   define MBDEF static inline
#endif // MBDEF


#define BLOCK(__f)  do { __f; } while(0)

#define MB_ARRAY_LEN(__arr) (sizeof(__arr) / sizeof(__arr[0]))


typedef enum 
{
    MB_CODE_ERROR = 0,
    MB_CODE_SUCCESS,
} 
MicrobCode;


typedef struct
{
    const char* compiler;
    const char* linker;
    const char* uploader;
} 
MicrobTools;


typedef struct 
{
    const char* compiler;
    const char* linker;
    const char* uploader;
} 
MicrobFlags;


typedef struct
{
    const char* mcu;
    const size_t f_cpu;
    const char* programmer;
    const char* port;
}
MicrobConfig;


typedef enum 
{
    MB_PROJECT_TYPE_EXE,
    MB_PROJECT_TYPE_LIB
} 
MicrobProjectType;


typedef struct 
{
    const char* name;
    const char* path;
    const MicrobProjectType type;
} 
MicrobProjectData;


typedef struct
{
    char** src_files;
    size_t count_src_files;
    char* build_dir;
    MicrobFlags flags;
    MicrobTools tools;
    MicrobProjectData project;
} 
MicrobData;


/// Public functions 
MBDEF MicrobCode mb_init(MicrobData*);
MBDEF MicrobCode mb_build(MicrobData*); 

/// Private functions
MBDEF MicrobCode __mb__mkdir_is_not_exists(const char* dir);
MBDEF MicrobCode __mb__cmd_run(const char* cmd);

/// 

MicrobCode
mb_init(MicrobData* data)
{
    char* path_buffer = getcwd(NULL, 0);
    if(path_buffer != NULL)
    {   
        printf("Current working dir: %s\n", path_buffer);
        data->project.path = path_buffer;
    }
    
    return 1;
}


MicrobCode
mb_build(MicrobData* data)
{   
    if(!__mb__mkdir_is_not_exists(data->build_dir))
    {
        return MB_CODE_ERROR;
    }

    char compile_cmd__obj_to_elf[MB_SIZE];
    snprintf(
        compile_cmd__obj_to_elf,
        MB_SIZE,
        "%s %s -o %s/%s/%s.elf ",
        data->tools.compiler,
        data->flags.compiler,
        data->project.path,
        data->build_dir,
        data->project.name
    );

    printf("[DEBUG] %s\n", compile_cmd__obj_to_elf);

    for(size_t i = 0; i < data->count_src_files; i++)
    {
        char compile_cmd__src_to_obj[MB_SIZE];
        snprintf(
            compile_cmd__src_to_obj,
            MB_SIZE,
            "%s %s -c %s/%s -o %s/%s/%s.o",
            data->tools.compiler,
            data->flags.compiler,
            data->project.path,
            data->src_files[i],
            data->project.path,
            data->build_dir,
            data->src_files[i]
        );

        strcat(compile_cmd__obj_to_elf, data->src_files[i]);
        strcat(compile_cmd__obj_to_elf, " ");

        printf("[DEBUG] %s\n", compile_cmd__obj_to_elf);
        printf("[DEBUG] %s\n", compile_cmd__src_to_obj);

        if(!__mb__cmd_run(compile_cmd__src_to_obj))
        {
            return MB_CODE_ERROR;
        }
    }

    if(!__mb__cmd_run(compile_cmd__obj_to_elf))
    {
        return MB_CODE_ERROR;
    }
    
    char linker_cmd[MB_SIZE];
    snprintf(
        linker_cmd,
        MB_SIZE,
        "%s %s %s/%s%s.elf %s/%s/%s.hex",
        data->tools.linker,
        MB_AVR_LINKER_FLAGS,
        data->project.path,
        data->build_dir,
        data->project.name,
        data->project.path,
        data->build_dir,
        data->project.name
    );

    printf("[DEBUG] %s\n", linker_cmd);

    if(!__mb__cmd_run(linker_cmd))
    {
        return MB_CODE_ERROR;
    }

    return MB_CODE_SUCCESS;
}


MicrobCode
__mb__mkdir_is_not_exists(const char* dir)
{
    if(access(dir, 0) != 0 && mkdir(dir, S_IRWXU | S_IRWXG | S_IRWXO) != 0)
    {
        fprintf(stderr, "Failed to create dir '%s'\n", dir);
        return MB_CODE_ERROR;
    }
    printf("Create dir: %s\n", dir);
    return MB_CODE_SUCCESS;
}


MicrobCode 
__mb__cmd_run(const char *cmd)
{ 
    printf("Exec: %s\n", cmd);

    int result = system(cmd);

    if(result != 0)
    {
        fprintf(stderr, "Command failed with code: %d\n", result);
        return MB_CODE_ERROR;
    }
    return MB_CODE_SUCCESS;
}

#endif // MICROBUILDER_MAIN_H