include ../../defaults.mk

INCDIR := ../../include
INCLUDES := -I$(INCDIR)
SBN := sbn
SBN_TESTER := sbn-tester
OPT := -g -Og
CFLAGS := $(FLAGS) $(INCLUDES) $(OPT)
CPPFLAGS := #-DNDEBUG

HEADERS := $(INCDIR)/utils/sbn.h $(INCDIR)/utils/vec.h

SRC := main.c sbn-tester.c $(HEADERS)

OBJS := $(SRC:.c=.o)

all: $(SBN) $(SBN_TESTER)

$(SBN): main.c $(HEADERS)
	$(CC) $(CFLAGS) $< -o $@

$(SBN_TESTER): sbn-tester.c $(HEADERS)
	$(CC) $(CFLAGS) $< -o $@

test: $(SBN_TESTER) $(SBN_TESTER).scm
	./$(SBN_TESTER).scm

example: $(SBN)
	./$(SBN)

clean:
	$(RM) $(SBN)

check: $(SRC) $(HEADERS)
	cppcheck --std=c11 -f --language=c --enable=all $(INCLUDES) $(SRC) $(HEADERS) -DSBN_CFG_IMPLEMENTATION -Dsbn_digit='uint32_t' -Dsbn_double_digit='uint64_t' -Dsbn_digit_nbits=32 -Dsbn_double_digit_nbits=64

.PHONY: all check clean example test
