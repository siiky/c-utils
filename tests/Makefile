include bs/bs.mk
include map/map.mk
include vec/vec.mk

# NOTE: CC must be the same used to build CHICKEN
CC := musl-gcc
CSC := csc
CSCFLAGS := \
    -L -Ltheft/build/ \
    -L -ltheft        \
    -lfa2             \
    -static           \

EXEC := tests

C_SRC := \
    $(BS_SRC)         \
    $(MAP_SRC)        \
    $(VEC_SRC)        \
    common/bool.c     \
    common/int.c      \
    common/size_t.c   \
    common/void_ptr.c \

CHICKEN_SRC := \
    main.scm   \

OBJS := $(C_SRC:.c=.o) $(CHICKEN_SRC:.scm=.o)

INCS := \
    -I.           \
    -I../include/ \
    -Itheft/inc/  \

OPTS := -Og -g -ggdb

CFLAGS := \
    $(INCS)      \
    $(OPTS)      \
    -Wall        \
    -Wconversion \
    -Wextra      \
    -static      \

all: $(EXEC)

test: all
	./$(EXEC) $(TEST_ARGS)

$(EXEC): $(OBJS)
	$(CSC) $(CSCFLAGS) -o $(EXEC) $(OBJS)

%.o: %.scm
	$(CSC) $(CSCFLAGS) -o $@ -c $<

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
VALGRIND_LOG := valgrind-$(DATE)-$(CC).log
EXEC_OUT := $(EXEC)-$(DATE)-$(CC).out
valgrind: $(EXEC)
	valgrind --log-file=$(VALGRIND_LOG) --track-origins=yes --leak-check=full -v ./$(EXEC) $(TEST_ARGS) > $(EXEC_OUT)

clean:
	$(RM) $(OBJS) $(EXEC) valgrind-*.log $(EXEC)-*.out *.link

clean_all: clean
	cd theft/ && make clean

.PHONY: all clean clean_all libtheft test theft valgrind
