include vec.mk

CC := cc

EXEC := tests

SRC := \
    $(VEC_SRC) \
    main.c     \

OBJS := $(SRC:.c=.o)

INCS := \
    -I../include/ \
    -Itheft/inc/  \

LIBS := \
    -Ltheft/build/ \
    -ltheft        \

CFLAGS := \
    $(INCS) \
    $(LIBS) \
    -Wall   \
    -Wextra \

all: $(EXEC)

test: all
	./$(EXEC)

$(EXEC): $(OBJS)
	$(CC) -o $@ $(OBJS) $(CFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<

theft:
	@if [ -d theft ]; then \
	    echo "### GIT: Updating theft ###";                       \
	    cd theft && git pull;                                     \
	    else                                                      \
	    echo "### GIT: Getting theft ###";                        \
	    git clone --depth=1 git://github.com/silentbicycle/theft; \
	    fi

libtheft:
	@echo "### BUILDING THEFT ###"
	cd theft && make

clean:
	$(RM) $(OBJS) $(EXEC)

clean_all: clean
	cd theft/ && make clean

.PHONY: all clean clean_all test libtheft
