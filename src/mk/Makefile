CC := musl-gcc
#CC := cc

DEST := ~/bin

INCLUDE := \
    -I../../include/ \

HEADERS :=

SRC := \
    mkheader.c   \
    mkmakefile.c \

EXECS := $(SRC:.c=)

OPT := -Og -g

CFLAGS := \
    $(INCLUDE)   \
    $(OPT)       \
    -Wall        \
    -Wconversion \
    -Wextra      \
    -flto        \
    -pedantic    \
    -static      \
    -std=c11     \

all: $(EXECS)

%: %.c
	$(CC) $(CFLAGS) $< -o $@

strip: $(EXECS)
	strip -s $(EXECS)

install: $(EXECS)
	install -m 0700 $(EXECS) -t $(DEST)

clean:
	$(RM) $(EXECS)

check: $(SRC) $(HEADERS)
	cppcheck --std=c11 -f --language=c --enable=all $(INCLUDE) $(SRC) $(HEADERS)

tags: $(SRC) $(HEADERS)
	ctags --language-force=c --tag-relative=yes -f .tags $(SRC) $(HEADERS)

.PHONY: all check clean install strip tags
