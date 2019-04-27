include ../../defaults.mk

DEST := ~/bin
INC := -I../../include/
OPT := -Os
CFLAGS := $(FLAGS) $(INC) $(OPT)

HEADERS := mkutils.h

SRC := \
    mkheader.c   \
    mkmakefile.c \

EXECS := $(SRC:.c=)

all: $(EXECS)

%: %.c
	$(CC) $(CFLAGS) $< -o $@

strip: $(EXECS)
	strip -s $(EXECS)

install: strip
	install -m 0700 $(EXECS) -t $(DEST)

clean:
	$(RM) $(EXECS)

check: $(SRC) $(HEADERS)
	cppcheck --std=c11 -f --language=c --enable=all $(INC) $(SRC) $(HEADERS)

.PHONY: all check clean install strip
