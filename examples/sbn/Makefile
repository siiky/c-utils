include ../../defaults.mk

INCDIR := ../../include
INCLUDES := -I$(INCDIR)
EXEC := sbn
OPT := -g -Og
CFLAGS := $(FLAGS) $(INCLUDES) $(OPT)
CPPFLAGS := #-DNDEBUG

HEADERS := $(INCDIR)/utils/sbn.h $(INCDIR)/utils/vec.h

SRC := main.c $(HEADERS)

OBJS := $(SRC:.c=.o)

all: $(EXEC)

$(EXEC): main.c
	$(CC) $(CFLAGS) $< -o $@

main.c: $(HEADERS)

clean:
	$(RM) $(EXEC)

check: $(SRC) $(HEADERS)
	cppcheck --std=c11 -f --language=c --enable=all $(INCLUDES) $(SRC) $(HEADERS) -DSBN_CFG_IMPLEMENTATION -Dsbn_digit='uint32_t' -Dsbn_double_digit='uint64_t' -Dsbn_digit_nbits=32 -Dsbn_double_digit_nbits=64

.PHONY: all check clean
