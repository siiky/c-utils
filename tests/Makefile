include ../defaults.mk
include common/common.mk
include bs/bs.mk
include map/map.mk
include vec/vec.mk

# NOTE: CC must be the same used to build CHICKEN and Theft
CSC := csc
CSCFLAGS := \
    -L -Ltheft/build/ \
    -L -ltheft        \
    -L -static        \
    -lfa2             \
    -static           \

EXEC := tests
INC := -I. -I../include/ -Itheft/inc/
OPT := -Og -g -ggdb
CFLAGS := $(FLAGS) $(INC) $(OPT)

HEADERS := \
    $(BS_HEADERS)     \
    $(COMMON_HEADERS) \
    $(MAP_HEADERS)    \
    $(VEC_HEADERS)    \

C_SRC := \
    $(BS_SRC)     \
    $(COMMON_SRC) \
    $(MAP_SRC)    \
    $(VEC_SRC)    \

CHICKEN_SRC := main.scm

OBJS := $(C_SRC:.c=.o) $(CHICKEN_SRC:.scm=.o)
DEPS := $(HEADERS) $(OBJS)

all: $(EXEC)

test: all
	./$(EXEC) $(TEST_ARGS)

$(EXEC): $(DEPS)
	$(CSC) $(CSCFLAGS) -o $(EXEC) $(OBJS)

%.o: %.scm
	$(CSC) $(CSCFLAGS) -o $@ -c $<

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
	make -C theft/ CC=$(CC)

DATE := $(shell date +"%Y%m%d-%H%M")
VALGRIND_LOG := valgrind-$(DATE)-$(CC).log
EXEC_OUT := $(EXEC)-$(DATE)-$(CC).out
valgrind: $(EXEC)
	valgrind --log-file=$(VALGRIND_LOG) --track-origins=yes --leak-check=full -v ./$(EXEC) $(TEST_ARGS) > $(EXEC_OUT)

clean:
	$(RM) $(OBJS) $(EXEC) valgrind-*.log $(EXEC)-*.out *.link

clean_all: clean
	make -C theft/ clean

.PHONY: all clean clean_all libtheft test theft valgrind
