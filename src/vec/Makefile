CC=musl-gcc
#CC=cc
EXEC=cvec

FLAGS=-std=c11 -static -Wall -Wextra -pedantic -flto -I../../include/
DFLAGS=$(FLAGS) -g
CFLAGS=$(FLAGS) -O3

HEADERS := \
    vec/char_vec.h \
    vec/int_vec.h  \
    vec.h          \

SRC := \
    vec/char_vec.c \
    vec/int_vec.c  \
    main.c         \

OBJS=$(SRC:.c=.o)

DEPS=$(HEADERS) $(OBJS)

all: $(EXEC)

$(EXEC): $(DEPS)
	$(CC) $(CFLAGS) $(OBJS) -o $(EXEC)
	strip -s $(EXEC)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -rf $(OBJS) $(EXEC)
