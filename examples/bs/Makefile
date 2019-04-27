include ../../defaults.mk

EXEC := bs
INC := -I../../include/
OPT := -g -Og
CFLAGS := $(FLAGS) $(INC) $(OPT)
CPPFLAGS := -DNDEBUG

HEADERS := \
    ../../include/utils/bs.h

SRC := \
    bs.c   \
    main.c \

OBJS := $(SRC:.c=.o)
DEPS := $(HEADERS) $(OBJS)

all: $(EXEC)

$(EXEC): $(DEPS)
	$(CC) $(CFLAGS) $(OBJS) -o $@

clean:
	$(RM) $(OBJS) $(EXEC)

check: $(SRC) $(HEADERS)
	cppcheck --std=c11 -f --language=c --enable=all $(INC) $(SRC) $(HEADERS)

.PHONY: all check clean
