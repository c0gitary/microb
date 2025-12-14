#include "fileman.h"
#include "config.h"

#include <unistd.h>

int
main(int argc, char** argv)
{
    char* cur_path = getcwd(NULL, 0);

    struct config* cfg = mb_config_init(cur_path);
    // struct fileman* fm = mb_fm_init(cur_path);

	mb_config_parse(&cfg);

	mb_config_print(&cfg);

	// mb_fm_create_makefile(&fm, &cfg);

	// mb_fm_free(&fm);
	mb_config_free(&cfg);

	free(cur_path);

	return 0;
}
