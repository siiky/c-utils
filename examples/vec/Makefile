include ../../defaults.mk

EXEC := cvec
INC := -I../../include/
OPT := -g -Og
CFLAGS := $(FLAGS) $(INC) $(OPT)

HEADERS := \
    ../../include/utils/vec.h \
    vec.h                     \
    vec/cvec.h                \
    vec/ivec.h                \

SRC := \
    main.c     \
    vec/cvec.c \
    vec/ivec.c \

OBJS=$(SRC:.c=.o)
DEPS=$(HEADERS) $(OBJS)

all: $(EXEC)

$(EXEC): $(DEPS)
	$(CC) $(CFLAGS) $(OBJS) -o $(EXEC)

clean:
	rm -rf $(OBJS) $(EXEC)

check: $(SRC) $(HEADERS)
	cppcheck --std=c11 -f --language=c --enable=all $(INC) $(SRC) $(HEADERS)

.PHONY: all check clean
