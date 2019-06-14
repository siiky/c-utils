CC := cc
FLAGS := \
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
    -std=c18                              \

# -Wpadded
