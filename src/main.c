#include "fileman.h"
#include "config.h"
#include "generator.h"

#include <unistd.h>
#include <string.h>

int
main()
{
    char* cur_path = getcwd(NULL, 0);

    // struct config* cfg = mb_config_init(cur_path);
	printf("121\n");
	printf("%s\n", strcat(cur_path, "/src"));
    struct fileman* fm = mb_fm_init(cur_path);
	
	// mb_config_parse(&cfg);
	
	// mb_generator__makefile(cfg, GMK_TYPE__AVR);

	// mb_config_print(&cfg);
	// mb_fm_create_makefile(&fm, &cfg);

	// mb_fm_free(&fm);
	// mb_config_free(&cfg);

	free(cur_path);

	return 0;
}
