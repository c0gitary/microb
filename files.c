#include "files.h"


struct Files*
mb_files_init(struct Config** config)
{
    struct Files* files = malloc(sizeof(struct Files));
    if (!files) return NULL;

    files->src = NULL;
    files->obj = NULL;
    files->count_src_files = 0;
    files->count_obj_files = 0;
    return files;
}


int
mb_files_append(struct Files** files, const char* name)
{
    if (!files || !name) return -1;

    if (!(*files)) return -1;

    (*files)->src = (char**)realloc((*files)->src, sizeof(char*) * ((*files)->count_src_files + 1));
    if (!(*files)->src) return -1;

    (*files)->obj = (char**)realloc((*files)->obj, sizeof(char*) * ((*files)->count_obj_files + 1));
    if (!(*files)->obj) return -1;

    (*files)->src[(*files)->count_src_files++] = strdup(name);
    (*files)->obj[(*files)->count_obj_files++] = strdup(utils_change_ext(name, ".o"));

    return 0;
}


void
mb_files_free(struct Files** files)
{
    if (!files || !*files) return;

    for (int i = 0; i < (*files)->count_src_files; ++i)
        free((*files)->src[i]);
    free((*files)->src);

    for (int i = 0; i < (*files)->count_obj_files; ++i)
        free((*files)->obj[i]);
    free((*files)->obj);

    free(*files);
    *files = NULL;
}
