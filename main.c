#include "builder.h"
#include "config.h"

#include <unistd.h>

int
main()
{
    char* cur_path = getcwd(NULL, 0);

    struct Config* cfg = mb_config_init(cur_path);
    struct Builder* bd = mb_builder_init(&cfg);

	mb_config_parse(&cfg);

	mb_config_print(&cfg);

	mb_builder_run(&bd, BT_CMDLINE);

	mb_config_free(&cfg);

	mb_builder_free(&bd);

	free(cur_path);

	return 0;
}
