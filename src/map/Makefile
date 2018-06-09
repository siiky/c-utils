CC := musl-gcc
#CC := cc
EXEC := map

FLAGS := \
    -I../../include/                      \
    -Wall                                 \
    -Wcast-align                          \
    -Wcast-qual                           \
    -Wconversion                          \
    -Werror-implicit-function-declaration \
    -Wextra                               \
    -Winit-self                           \
    -Wpointer-arith                       \
    -Wshadow                              \
    -Wstrict-prototypes                   \
    -Wundef                               \
    -Wuninitialized                       \
    -Wunreachable-code                    \
    -Wwrite-strings                       \
    -flto                                 \
    -fno-common                           \
    -march=native                         \
    -pedantic                             \
    -static                               \
    -std=c11                              \

OPT := -g

CFLAGS := $(OPT) $(FLAGS)

HEADERS := \
    ii_map.h \

SRC := \
    ii_map.c \
    main.c   \

OBJS := $(SRC:.c=.o)

DEPS := $(HEADERS) $(OBJS)

all: $(EXEC)

$(EXEC): $(DEPS)
	$(CC) $(CFLAGS) $(OBJS) -o $(EXEC)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -rf $(OBJS) $(EXEC)
