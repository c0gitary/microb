#include "builder.h"

struct Builder*
mb_builder_init(struct Config** config)
{
    struct Builder* builder = (struct Builder*)malloc(sizeof(struct Builder));
    if (!builder) return NULL;

    builder->files = mb_files_init(config);
    builder->config = *config;

    if(builder->config->path == NULL)
        return NULL;

    DIR* dir = (DIR*)opendir(builder->config->path);
    struct dirent* entry;

    while((entry = readdir(dir)))
    {
        if(utils_is_back(&entry)) continue;

        if(utils_is_src_file(&entry))
        {
            mb_files_append(&builder->files, entry->d_name);

            printf("src: %s\t obj: %s\n",
                builder->files->src[builder->files->count_src_files - 1],
                builder->files->obj[builder->files->count_obj_files - 1]
            );
        }
    }
    closedir(dir);
    return builder;
}


int
mb_builder_run(struct Builder** builder, enum BuilderType btype)
{
    switch(btype)
    {
    case BT_MAKEFILE:
    {

        return -1;
    }

    case BT_CMAKE:
    {

        return -1;
    }

    case BT_CMDLINE:
    {

        return mb_builder_cmdline(builder);
    }

    default:
    {
        return -1;
    }

    }
}


int
mb_builder_cmdline(struct Builder** builder)
{
    if(!builder || !*builder)
        return -1;

    char cc[512];
    char cc_elf[512];
    char ld[512];

    const char* cflags = "-Wall -Wextra -Werror -std=c99 -Os -mmcu=";

    snprintf(
        cc_elf,
        sizeof(cc_elf),
        "%s %s%s -DF_CPU=%s -o %s.elf ",
        (*builder)->config->cc != NULL ? (*builder)->config->cc : "avr-gcc",
        cflags,
        (*builder)->config->mcu,
        (*builder)->config->freq,
        (*builder)->config->project_name
    );

    snprintf(
        ld,
        sizeof(ld),
        "%s -O ihex -R .eeprom %s.elf %s.hex",
        (*builder)->config->ld != NULL ? (*builder)->config->ld : "avr-objcopy",
        (*builder)->config->project_name,
        (*builder)->config->project_name
    );

    for(int i = 0; i < (*builder)->files->count_src_files; i++)
    {
        snprintf(
            cc,
            sizeof(cc),
            "%s %s%s -DF_CPU=%s -c %s -o %s",
            (*builder)->config->cc != NULL ? (*builder)->config->cc : "avr-gcc",
            cflags,
            (*builder)->config->mcu,
            (*builder)->config->freq,
            (*builder)->files->src[i],
            (*builder)->files->obj[i]
        );

        strcat(cc_elf, " ");
        strcat(cc_elf, (*builder)->files->obj[i]);

        if(mb_builder_exec(cc))
        {
            perror("Compiled error!\n");
            return -1;
        }

    }

    mb_builder_exec(cc_elf);
    mb_builder_exec(ld);

    return 0;
}


int
mb_builder_exec(const char* cmd)
{
    printf("Executing command: %s\n", cmd);
    int status = system(cmd);
    if(WIFEXITED(status))
        return WEXITSTATUS(status);
    return -1;
}


int
mb_builder_free(struct Builder** builder)
{
    if(!builder || !*builder) return -1;

    mb_files_free(&(*builder)->files);

    free(*builder);
    *builder = NULL;

    return 0;
}
