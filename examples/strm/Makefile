CC := cc
EXEC := strm
INCLUDE := -I../../include/
SRC := \
    main.c \
    strm.c \

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

.PHONY: all clean
