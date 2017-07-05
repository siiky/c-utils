CC=musl-gcc
#CC=cc
EXEC=llist
INPUT=main.c
OBJS=main.o
DEST=~/bin

FLAGS=-static -Wall -Wextra -Werror -pedantic -I../../include/
DFLAGS=$(FLAGS) -Og -g
CFLAGS=$(FLAGS) -O3

debug:
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
