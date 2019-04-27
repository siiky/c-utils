include ../../defaults.mk

EXEC := ftr
INC := -I../../include/
OPT := -g -Og
CFLAGS := $(FLAGS) $(INC) $(OPT)

HEADERS := \
    ../../include/utils/ftr.h

SRC := \
    main.c \

OBJS := $(SRC:.c=.o)
DEPS := $(HEADERS) $(OBJS)

all: $(EXEC)

$(EXEC): $(DEPS)
	$(CC) $(CFLAGS) $(OBJS) -o $(EXEC)

clean:
	$(RM) $(OBJS) $(EXEC)

check: $(SRC) $(HEADERS)
	cppcheck --std=c11 -f --language=c --enable=all $(INC) $(SRC) $(HEADERS)

.PHONY: all check clean
