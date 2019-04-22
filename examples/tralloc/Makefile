CC := musl-gcc
#CC := cc
DEST := ~/bin
EXEC := tralloc
INCLUDE := \
    -I../../include/ \

HEADERS := ../../include/tralloc.h
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
    -std=c18     \

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(EXEC)

clean:
	$(RM) $(OBJS) $(EXEC)

check: $(SRC) $(HEADERS)
	cppcheck --std=c11 -f --language=c --enable=all $(INCLUDE) $(SRC) $(HEADERS)

.PHONY: all check clean install strip tags uninstall
