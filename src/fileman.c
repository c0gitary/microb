#include "fileman.h"


#define fm_template_makefile_mcu_name(__mcu) "MCU = " __mcu
#define fm_template_makefile_mcu_freq(__mcu) 


#define fm_tmk_fwarn "CFWARN = -Wall -Wstrict-prototypes -Wl, --gc-sections -Wl, --relax\n"
#define fm_tmk_fopt "CFOPT = -O3 -lm\n"
#define fm_tmk_obj "__OBJ__ = $(__SRC__:.c=.o)\n"
#define fm_tmk_target_build "$(__TARGET__): $(__OBJ__)\n\tavr-objcopy -O ihex $(__OBJ__) $(__TARGET__)\n"
#define fm_tmk_src_to_obj "$(__OBJ__): $(__SRC__)\n\tavr-gcc $(CFWARN) $(CFOPT) $< -o $@\n"
#define fm_tmk_flash "flash: $(__TARGET__)\n\tavrdude -p $(__MCU__) -c $(__PROGRAMMER__) -U flash:w:$(__TARGET__)\n"
#define fm_tmk_clean "clean:\n\trm -f $(__OBJ__) $(__TARGET__)\n"
#define fm_tmk_phony ".PHONY: clean flash"

int
mb_fm_create_makefile(struct fileman** fm, struct config** cfg)
{
    if(fm && *fm)
    {
        FILE* mkfile = fopen("makefile", "w");

        fputs(fm_tmk_fwarn, mkfile);
		fputs(fm_tmk_fopt, mkfile);

		fputs("__PROJECT__ = ", mkfile);
		fputs((*cfg)->project_name, mkfile);
		fputc('\n', mkfile);
		fputs("__MCU__ = ", mkfile);
		fputs((*cfg)->mcu_name, mkfile);
		fputc('\n', mkfile);
		fputs("__PROGRAMMER__ = ", mkfile);
		fputs((*cfg)->programmer, mkfile);
		fputc('\n', mkfile);
		fputs("__TARGET__ = $(__PROJECT__).hex\n", mkfile);
		fputs("__SRC__ = ", mkfile);

		DIR* dir = opendir((*fm)->path);
		struct dirent* entry;
		while((entry = readdir(dir)))
		{
			if(utils_is_src_file(&entry))
			{
				fputs(entry->d_name, mkfile);
				fputs(" ", mkfile);
			}
		}
		fputs("\n", mkfile);
		fputs(fm_tmk_obj, mkfile);
		fputs(fm_tmk_target_build, mkfile);
		fputs(fm_tmk_src_to_obj, mkfile);
		fputs(fm_tmk_flash, mkfile);
		fputs(fm_tmk_clean, mkfile);
		fputs(fm_tmk_phony, mkfile);

		closedir(dir);
		fclose(mkfile);

        return 1;
    }

    return 0;
}


struct fileman*
mb_fm_init(const char* path)
{
	if(!path) return NULL;

	struct fileman* fm = (struct fileman*)malloc(sizeof(struct fileman));

	fm->path = path;
	fm->count_files = fm->count_src_files = 0;

	DIR* dir = opendir(fm->path);

	struct dirent* entry;

	while((entry = readdir(dir)))
	{
		if(utils_is_back(&entry)) continue;

		if(utils_is_src_file(&entry))
		    fm->count_src_files++;

		fm->count_files++;
	}

	return fm;
};

void
mb_fm_print(struct fileman **fm)
{
    if(fm && *fm)
    {
        DIR* dir = opendir((*fm)->path);
        struct dirent* entry;
        while((entry = readdir(dir)))
        {
            if(utils_is_src_file(&entry))
                printf("%s\n", entry->d_name);
        }
        closedir(dir);
    }
}

void
mb_fm_free(struct fileman** fm)
{
	if(fm && *fm)
	{
		(*fm)->count_files = 0;
		free(*fm);
		*fm = NULL;
	}
}
