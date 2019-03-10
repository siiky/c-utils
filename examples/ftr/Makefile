CC := musl-gcc
#CC := cc
DEST := ~/bin
EXEC := ftr
INCLUDE := -I../../include/
HEADERS := \

SRC := \
    main.c \

OBJS := $(SRC:.c=.o)

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

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(EXEC)

strip: $(EXEC)	strip -s $(EXEC)

clean:
	$(RM) $(OBJS) $(EXEC)

check: $(SRC) $(HEADERS)
	cppcheck --std=c11 -f --language=c --enable=all $(INCLUDE) $(SRC) $(HEADERS)

tags: $(SRC) $(HEADERS)
	ctags --language-force=c --tag-relative=yes -f .tags $(SRC) $(HEADERS)

.PHONY: all check clean strip tags
