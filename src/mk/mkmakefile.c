#include <stdio.h>

#include <unistd.h>

#include "mkutils.h"

const char * FNAME = "Makefile";
const char * TXTFMT1 =
"CC := cc\n"
"DEST := ~/bin\n"
"EXEC := %s\n"
"HEADERS :=\n"
"SRC := \\\n"
;

const char * TXTFMT2 =
"\n"
"OBJS := $(SRC:.c=.o)\n"
"\n"
"INC := -Iinclude/ -I/usr/local/include/\n"
"OPT := -Og -g\n"
"CFLAGS := \\\n"
"\t$(INC)                                \\\n"
"\t$(OPT)                                \\\n"
"\t-Wall                                 \\\n"
"\t-Wcast-align                          \\\n"
"\t-Wcast-qual                           \\\n"
"\t-Wconversion                          \\\n"
"\t-Werror-implicit-function-declaration \\\n"
"\t-Wextra                               \\\n"
"\t-Winit-self                           \\\n"
"\t-Wpointer-arith                       \\\n"
"\t-Wshadow                              \\\n"
"\t-Wstrict-prototypes                   \\\n"
"\t-Wundef                               \\\n"
"\t-Wuninitialized                       \\\n"
"\t-Wunreachable-code                    \\\n"
"\t-Wwrite-strings                       \\\n"
"\t-flto                                 \\\n"
"\t-fno-common                           \\\n"
"\t-march=native                         \\\n"
"\t-pedantic                             \\\n"
"\t-static                               \\\n"
"\t-std=c18                              \\\n"
"\n"
"all: $(EXEC)\n"
"\n"
"$(EXEC): $(HEADERS) $(OBJS)\n"
"\t$(CC) $(CFLAGS) $(OBJS) -o $(EXEC)\n"
"\n"
"strip: $(EXEC)\n"
"\tstrip -s $(EXEC)\n"
"\n"
"install: strip\n"
"\tinstall -m 0700 $(EXEC) -t $(DEST)\n"
"\n"
"uninstall:\n"
"\t$(RM) $(DEST)/$(EXEC)\n"
"\n"
"clean:\n"
"\t$(RM) $(OBJS) $(EXEC)\n"
"\n"
"check: $(SRC) $(HEADERS)\n"
"\tcppcheck --std=c11 -f --language=c --enable=all $(INC) $(SRC) $(HEADERS)\n"
"\n"
"tags: $(SRC) $(HEADERS)\n"
"\tctags --language-force=c --tag-relative=yes -f .tags $(SRC) $(HEADERS)\n"
"\n"
".PHONY: all check clean install strip tags uninstall\n"
;

int main (int argc, char ** argv)
{
    int ret = 0;
    FILE * f = NULL;

    ABORT(ko, argc < 2, "Usage: %s PROG_NAME [SRC]...\n", argv[0]);

    ABORT(ko, file_exists(FNAME),
            "File already exists, aborting!");

    f = fopen(FNAME, "w");
    ABORT(ko, f == NULL,
            "Could not open file for writing, aborting!");

    ABORT(ko, fprintf(f, TXTFMT1, argv[1]) < 0,
            "An error occurred while writing to file!");

    for (int i = 2; i < argc; i++)
        ABORT(ko, fprintf(f, "\t%s \\\n", argv[i]) < 0,
                "An error occurred while writing to file!");

    ABORT(ko, fprintf(f, TXTFMT2) < 0,
            "An error occurred while writing to file!");

ret:
    ifnotnull(f, fclose);
    return ret;
ko:
    ret = 1;
    goto ret;
}
