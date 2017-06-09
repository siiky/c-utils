CC=musl-gcc
#CC=cc
CFLAGS=-static -Wall -Wextra -Werror -pedantic -O3 -I../../include/
EXECS=mkheader mkmakefile
DEST=~/bin/

all: $(EXECS)

mkheader: mkheader.c
	$(CC) $(CFLAGS) mkheader.c -o mkheader

mkmakefile: mkmakefile.c
	$(CC) $(CFLAGS) mkmakefile.c -o mkmakefile

install: all
	strip -s $(EXECS)
	install -m 0700 $(EXECS) -t $(DEST)

uninstall:
	cd $(DEST)
	rm -f $(EXECS)

clean:
	rm -rf $(EXECS)
