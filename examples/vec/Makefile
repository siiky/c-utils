CC=musl-gcc
#CC=cc
EXEC=cvec

FLAGS=-std=c11 -static -Wall -Wextra -pedantic -flto -I../../include/
CFLAGS=$(FLAGS) -Og -g

HEADERS := \
    vec.h      \
    vec/cvec.h \
    vec/ivec.h \

SRC := \
    main.c     \
    vec/cvec.c \
    vec/ivec.c \

OBJS=$(SRC:.c=.o)

DEPS=$(HEADERS) $(OBJS)

all: $(EXEC)

$(EXEC): $(DEPS)
	$(CC) $(CFLAGS) $(OBJS) -o $(EXEC)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -rf $(OBJS) $(EXEC)
