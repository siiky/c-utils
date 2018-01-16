CC=musl-gcc
#CC=cc
EXEC=cvec

FLAGS=-std=c11 -static -Wall -Wextra -pedantic -flto -I../../include/
DFLAGS=$(FLAGS) -g
CFLAGS=$(FLAGS) -O3

HEADERS=char_vec.h int_vec.h gen_vec.h
SRC=main.c char_vec.c int_vec.c
OBJS=$(SRC:.c=.o)

DEPS=$(SRC) $(HEADERS)

debug: $(DEPS)
	$(CC) $(DFLAGS) -c $(SRC)
	$(CC) $(DFLAGS) $(OBJS) -o $(EXEC)

all:
	$(CC) $(CFLAGS) -c $(SRC)
	$(CC) $(CFLAGS) $(OBJS) -o $(EXEC)
	strip -s $(EXEC)

clean:
	rm -rf $(OBJS) $(EXEC)
