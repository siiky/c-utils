#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>

#include "mkutils.h"

const char * TXTFMT =
"#ifndef _%s_H\n"
"#define _%s_H\n"
"\n"
"\n"
"#endif /* _%s_H */\n";

int main (int argc, char ** argv)
{
    int ret = 0;
    FILE * f = NULL;
    char * tmp = NULL;

    ABORT(ko, argc != 2, "%s [HEADER_NAME]\n", argv[0]);

    size_t len = strlen(argv[1]) + 1;
    tmp = calloc(len + 2, sizeof(char));

    ABORT(ko, tmp == NULL,
          "Could not allocate memory, aborting!\n");

    sprintf(tmp, "%s.h", argv[1]);

    ABORT(ko, file_exists(tmp),
          "File already exists, aborting!\n");

    f = fopen(tmp, "w");
    ABORT(ko, f == NULL,
          "Could not open file for writing, aborting!\n");

    for (size_t i = 0; i < len; i++)
        tmp[i] = toupper(tmp[i]);
    tmp[len - 1] = '\0';

    ABORT(ko, fprintf(f, TXTFMT, tmp, tmp, tmp) < 0,
        "An error occurred while writing to file!\n");

ret:
    IFNOTNULL(f, fclose);
    IFNOTNULL(tmp, free);
    return ret;
ko:
    ret = 1;
    goto ret;
}
