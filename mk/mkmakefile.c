#include <stdio.h>

#include <unistd.h>

#include "mkutils.h"

const char * FNAME = "./Makefile";
const char * TXTFMT =
"CC=musl-gcc\n"
"#CC=cc\n"
"EXEC=%s\n"
"INPUT=main.c\n"
"OBJS=main.o\n"
"DEST=~/bin\n"
"\n"
"FLAGS=-static -Wall -Wextra -Werror -pedantic -Iinclude\n"
"DFLAGS=$(FLAGS) -Og -g\n"
"CFLAGS=$(FLAGS) -O3\n"
"\n"
"debug:\n"
"\t$(CC) $(DFLAGS) -c $(INPUT)\n"
"\t$(CC) $(DFLAGS) $(OBJS) -o $(EXEC)\n"
"\n"
"all:\n"
"\t$(CC) $(CFLAGS) -c $(INPUT)\n"
"\t$(CC) $(CFLAGS) $(OBJS) -o $(EXEC)\n"
"\tstrip -s $(EXEC)\n"
"\n"
"install: all\n"
"\tinstall -m 0700 $(EXEC) -t $(DEST)\n"
"\n"
"uninstall:\n"
"\trm -f $(DEST)/$(EXEC)\n"
"\n"
"clean:\n"
"\trm -rf $(OBJS) $(EXEC)\n";

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
    IFNOTNULL(f, fclose);
    return ret;
ko:
    ret = 1;
    goto ret;
}
