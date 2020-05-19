# Makefile

# OS information

AUTHOR      = Simon Chaykin
VERSION     = 0.0.2
LICENSE     = WTFPL

# Makefiel configuration

BOOT_DIR    = boot
INIT_DIR    = init
KERNEL_DIR  = kernel
BUILD_DIR   = build
INCLUDE_DIR = include
ISO_DIR     = $(BUILD_DIR)/iso

# Files

LSCRIPT     = link.ld
GRUB_CONFIG = grub.cfg
ISOFILE     = ChaykinOS-$(VERSION).iso
BINFILE     = kernel.$(BINFORMAT)

# Cross-Compiler settings

ARCH        = i686
BINFORMAT   = elf
CROSS       = ~/opt/cross

# Compilers and emulators

CC          = $(CROSS)/bin/$(ARCH)-$(BINFORMAT)-gcc
GAS         = $(CROSS)/bin/$(ARCH)-$(BINFORMAT)-as # AT&T-syntax assembler
NASM        = nasm # Intel-syntax
EMU         = qemu-system-i386
GRUB        = grub-mkrescue

# Compilers' and emulators' flags

CFLAGS      = -g -std=gnu99 -ffreestanding -Wall -Wextra -I $(INCLUDE_DIR)
GASFLAGS    =
NASMFLAGS   = -f $(BINFORMAT)32
EMUFLAGS    = -m 256
LDFLAGS     = -T$(LSCRIPT) -ffreestanding -nostdlib -lgcc -I $(INCLUDE_DIR) -g

# Sources

SOURCES     += $(shell find -name "*.c" -type f -print)
SOURCES     += $(shell find -name "*.s" -type f -print)
SOURCES     += $(shell find -name "*.asm" -type f -print)

OBJS        := $(patsubst %.c, %.o, $(SOURCES))
OBJS        := $(patsubst %.s, %.o, $(OBJS))
OBJS        := $(patsubst %.asm, %.o, $(OBJS))

SOURCE_DIRS := boot init kernel

CRTBEGIN    := `$(CC) $(CFLAGS) $(LDFLAGS) -print-file-name=crtbegin.o`
CRTEND      := `$(CC) $(CFLAGS) $(LDFLAGS) -print-file-name=crtend.o`

all: chkcross directories kernel grub iso run

chkcross:
	chmod +x ./tools/chkcross.sh
	./tools/chkcross.sh $(CROSS)/bin $(ARCH)-$(BINFORMAT)

directories:
	mkdir -p $(BUILD_DIR)/bin
	mkdir -p $(BUILD_DIR)/iso/boot/grub
	mkdir -p $(BUILD_DIR)/releases
	mkdir -p $(patsubst %,"$(BUILD_DIR)/objs/%",$(SOURCE_DIRS))

kernel: $(OBJS)
	$(CC) -o $(BUILD_DIR)/bin/$(BINFILE) $(LDFLAGS) $(CRTBEGIN) $(patsubst %.o,./$(BUILD_DIR)/objs/%.o,$(OBJS)) $(CRTEND)

grub:
	echo "menuentry \"ChaykinOS $(VERSION)\" {" > $(BUILD_DIR)/iso/boot/grub/grub.cfg
	echo "	multiboot /boot/kernel.elf" >> $(BUILD_DIR)/iso/boot/grub/grub.cfg
	echo "	boot" >> $(BUILD_DIR)/iso/boot/grub/grub.cfg
	echo "}" >> $(BUILD_DIR)/iso/boot/grub/grub.cfg

iso: grub kernel
	cp $(BUILD_DIR)/bin/$(BINFILE) $(BUILD_DIR)/iso/boot/$(BINFILE)
	$(GRUB) -o $(BUILD_DIR)/releases/$(ISOFILE) $(BUILD_DIR)/iso
	cp $(BUILD_DIR)/releases/$(ISOFILE) $(ISOFILE)

run: iso
	$(EMU) -cdrom $(ISOFILE) $(EMUFLAGS)

clean:
	rm -rf $(patsubst %.o,./$(BUILD_DIR)/objs/%.o,$(OBJS)) $(BUILD_DIR)

# Compilation

%.o: %.c
	$(CC) -c $< -o $(BUILD_DIR)/objs/$@ $(CFLAGS)

%.o: %.s
	$(GAS) $< -o $(BUILD_DIR)/objs/$@ $(GASFLAGS)

%.o: %.asm
	$(NASM) $< -o $(BUILD_DIR)/objs/$@ $(NASMFLAGS)
