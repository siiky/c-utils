include vec/vec.mk
include map/map.mk

CC := musl-gcc

EXEC := tests

SRC := \
    $(MAP_SRC)        \
    $(VEC_SRC)        \
    common/bool.c     \
    common/int.c      \
    common/size_t.c   \
    common/void_ptr.c \
    main.c            \

OBJS := $(SRC:.c=.o)

INCS := \
    -I.           \
    -I../include/ \
    -Itheft/inc/  \

LIBS := \
    -Ltheft/build/ \
    -ltheft        \

OPTS := -Og -g

CFLAGS := \
    $(INCS)      \
    $(LIBS)      \
    $(OPTS)      \
    -Wall        \
    -Wconversion \
    -Wextra      \
    -ggdb        \
    -static      \

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

libtheft: theft
	@echo "### BUILDING THEFT ###"
	cd theft && make CC=$(CC)

DATE := $(shell date +"%Y%m%d-%H%M")
VALGRIND_LOG := valgrind-$(DATE).log
EXEC_OUT := $(EXEC)-$(DATE).out
valgrind: $(EXEC)
	valgrind --log-file=$(VALGRIND_LOG) --track-origins=yes --leak-check=full -v ./$(EXEC) > $(EXEC_OUT)

clean:
	$(RM) $(OBJS) $(EXEC) valgrind-*.log $(EXEC)-*.out

clean_all: clean
	cd theft/ && make clean

.PHONY: all clean clean_all libtheft test theft valgrind
