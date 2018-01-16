#include <stdio.h>

#include <unistd.h>

#include "mkutils.h"

const char * FNAME = "./Makefile";
const char * TXTFMT =
"CC=musl-gcc\n"
"#CC=cc\n"
"DEST=~/bin\n"
"EXEC=%s\n"
"INCLUDE=-Iincude/\n"
"HEADERS=\n"
"SRC=main.c\n"
"OBJS=$(SRC:.c=.o)\n"
"\n"
"CFLAGS=\\\n"
"       -Wall        \\\n"
"       -Wconversion \\\n"
"       -Wextra      \\\n"
"       -flto        \\\n"
"       -pedantic    \\\n"
"       -static      \\\n"
"       -std=c11     \\\n"
"       $(INCLUDE)   \\\n"
"\n"
"debug: $(SRC)\n"
"\t$(CC) $(CFLAGS) -Og -g -c $(SRC)\n"
"\t$(CC) $(CFLAGS) -Og -g $(OBJS) -o $(EXEC)\n"
"\n"
"all: $(SRC)\n"
"\t$(CC) $(CFLAGS) -O3 -c $(SRC)\n"
"\t$(CC) $(CFLAGS) -O3 $(OBJS) -o $(EXEC)\n"
"\tstrip -s $(EXEC)\n"
"\n"
"install: all\n"
"\tinstall -m 0700 $(EXEC) -t $(DEST)\n"
"\n"
"uninstall:\n"
"\trm -f $(DEST)/$(EXEC)\n"
"\n"
"clean:\n"
"\trm -rf $(OBJS) $(EXEC)\n"
"\n"
"check: $(SRC) $(HEADERS)\n"
"\tcppcheck --std=c11 -f --language=c --enable=all $(SRC) $(HEADERS)\n"
"\n"
"tags: $(SRC) $(HEADERS)\n"
"\tctags --language-force=c --tag-relative=yes -f .tags $(SRC) $(HEADERS)\n"
;

int main (int argc, char ** argv)
{
    int ret = 0;
    FILE * f = NULL;

    ABORT(ko, argc != 2, "%s [PROG_NAME]\n", argv[0]);

    ABORT(ko, file_exists(FNAME),
          "File already exists, aborting!");

    f = fopen(FNAME, "w");
    ABORT(ko, f == NULL,
          "Could not open file for writing, aborting!");

    ABORT(ko, fprintf(f, TXTFMT, argv[1]) < 0,
          "An error occurred while writing to file!");

ret:
    ifnotnull(f, fclose);
    return ret;
ko:
    ret = 1;
    goto ret;
}
