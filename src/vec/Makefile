CC=musl-gcc
#CC=cc
EXEC=cvec

DEST=~/bin

FLAGS=-static -Wall -Wextra -pedantic -flto -I../../include/
DFLAGS=$(FLAGS) -g
CFLAGS=$(FLAGS) -O3

INCLUDE=
INPUT=main.c
OBJS=$(INPUT:.c=.o)

DEPS=$(INPUT) $(INCLUDE)

debug: $(DEPS)
	$(CC) $(DFLAGS) -c $(INPUT)
	$(CC) $(DFLAGS) $(OBJS) -o $(EXEC)

all:
	$(CC) $(CFLAGS) -c $(INPUT)
	$(CC) $(CFLAGS) $(OBJS) -o $(EXEC)
	strip -s $(EXEC)

install: all
	install -m 0700 $(EXEC) -t $(DEST)

uninstall:
	rm -f $(DEST)/$(EXEC)

clean:
	rm -rf $(OBJS) $(EXEC)
