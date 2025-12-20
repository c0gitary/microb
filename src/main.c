#include "config.h"
#include "generator.h"

#include <unistd.h>
#include <string.h>

int
main()
{
    char* cur_path = getcwd(NULL, 0);
    struct config* cfg = mb_config_init(cur_path);
	
	mb_config_parse(&cfg);
	
	mb_generator__makefile(cfg, GMK_TYPE__AVR);

	mb_config_free(&cfg);

	free(cur_path);

	return 0;
}
