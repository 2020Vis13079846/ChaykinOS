# Makefile

AUTHOR     := Simon Chaykin
VERSION    := 0.0.1
LICENSE    := WTFPL

CROSS_ARCH  := i686
CROSS_DIR   ?= ~/opt/cross/lib/gcc/i686-elf/7.5.0

SOURCE_DIR  := src
INCLUDE_DIR := include

LIBC        := libcmembrane.a
LIBK        := libmembrane.a

AR          := $(CROSS_ARCH)-elf-ar
CC          := $(CROSS_ARCH)-elf-gcc

ARFLAGS     := -rcs
CFLAGS      := -std=gnu99 -ffreestanding -Wall -Wextra -g -I $(INCLUDE_DIR) -w

STRING_OBJS := $(patsubst %.c,%.o,$(wildcard $(SOURCE_DIR)/string/*.c))
STDLIB_OBJS := $(patsubst %.c,%.o,$(wildcard $(SOURCE_DIR)/stdlib/*.c))
STDIO_OBJS  := $(patsubst %.c,%.o,$(wildcard $(SOURCE_DIR)/stdio/*.c))
MATH_OBJS   := $(patsubst %.c,%.o,$(wildcard $(SOURCE_DIR)/math/*.c))

LIB_OBJS    := $(STRING_OBJS) \
			   $(STDLIB_OBJS) \
			   $(STDIO_OBJS) \
			   $(MATH_OBJS)

LIBC_OBJS   := $(LIB_OBJS)
LIBK_OBJS   := $(LIB_OBJS:.o=.libk.o)

%.o: %.c
	@$(CC) -c $< -o $@ $(CFLAGS) -D__is_libc

%.libk.o: %.c
	@$(CC) -c $< -o $@ $(CFLAGS) -D__is_libk -D__is_libc

all: install

install-headers:
	@cp -r $(INCLUDE_DIR) $(CROSS_DIR)

install-libs: $(LIBK) $(LIBC)
	@cp $(LIBC) $(CROSS_DIR)
	@cp $(LIBK) $(CROSS_DIR)

install: install-headers install-libs

$(LIBK): $(LIBK_OBJS)
	@$(AR) $(ARFLAGS) $@ $(LIBK_OBJS)

$(LIBC): $(LIBC_OBJS)
	@$(AR) $(ARFLAGS) $@ $(LIBC_OBJS)

clean:
	@rm -rf $(LIBK) $(LIBC) $(LIB_OBJS) $(LIBK_OBJS) $(LIBC_OBJS)
