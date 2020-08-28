# Makefile

# OS information

AUTHOR      = Simon Chaykin
VERSION     = 0.7.4
LICENSE     = WTFPL

# Makefile configuration

BUILD_DIR   = build
INCLUDE_DIR = include
ISO_DIR     = $(BUILD_DIR)/iso

# Files

LSCRIPT     = link.ld
GRUB_CONFIG = grub.cfg
ISOFILE     = ChaykinOS-$(VERSION).iso
BINFILE     = kernel.$(BINFORMAT)
LOGFILE     = chaykinos.log

# Compiler settings

BINFORMAT   = elf

# Compilers and emulators

CC          = gcc
GAS         = as # AT&T-syntax assembler
NASM        = nasm # Intel-syntax
EMU         = qemu-system-i386
GRUB        = grub-mkrescue

# Compilers' and emulators' flags

CFLAGS      = -m32 -std=gnu99 -ffreestanding -Wall -Wextra -I $(INCLUDE_DIR) -D __is_kernel -D __is_libk -Wall -Wextra -pedantic-errors
GASFLAGS    = --32
NASMFLAGS   = -f $(BINFORMAT)32
EMUFLAGS    = -m 256 -serial file:$(LOGFILE) -nic none
LDFLAGS     = -m32 -T$(LSCRIPT) -ffreestanding -nostdlib -lgcc -I $(INCLUDE_DIR)

# Sources

SOURCES     += $(shell find -name "*.c" -not -path "./membrane/*" -print -type f)
SOURCES     += $(shell find -name "*.s" -type f -print)
SOURCES     += $(shell find -name "*.asm" -type f -print)

OBJS        := $(patsubst %.c, %.o, $(SOURCES))
OBJS        := $(patsubst %.s, %.o, $(OBJS))
OBJS        := $(patsubst %.asm, %.o, $(OBJS))

SOURCE_DIRS := boot init kernel lib mm drivers

CRTBEGIN    := `$(CC) $(CFLAGS) $(LDFLAGS) -print-file-name=crtbegin.o`
CRTEND      := `$(CC) $(CFLAGS) $(LDFLAGS) -print-file-name=crtend.o`

all: #directories kernel grub iso
	@echo $(SOURCES)

directories:
	@mkdir -p $(BUILD_DIR)/bin
	@mkdir -p $(BUILD_DIR)/iso/boot/grub
	@mkdir -p $(patsubst %,"$(BUILD_DIR)/objs/%",$(SOURCE_DIRS))

kernel: $(OBJS)
	@$(CC) -o $(BUILD_DIR)/bin/$(BINFILE) $(LDFLAGS) "$(CRTBEGIN)" $(patsubst %.o,./$(BUILD_DIR)/objs/%.o,$(OBJS)) "$(CRTEND)"

grub:
	@echo "menuentry \"ChaykinOS $(VERSION)\" {" > $(BUILD_DIR)/iso/boot/grub/grub.cfg
	@echo "	multiboot /boot/kernel.elf" >> $(BUILD_DIR)/iso/boot/grub/grub.cfg
	@echo "	boot" >> $(BUILD_DIR)/iso/boot/grub/grub.cfg
	@echo "}" >> $(BUILD_DIR)/iso/boot/grub/grub.cfg

iso: grub kernel
	@cp $(BUILD_DIR)/bin/$(BINFILE) $(BUILD_DIR)/iso/boot/$(BINFILE)
	@$(GRUB) -o $(ISOFILE) $(BUILD_DIR)/iso

run: iso
	@$(EMU) -cdrom $(ISOFILE) $(EMUFLAGS)

clean:
	@rm -rf $(patsubst %.o,./$(BUILD_DIR)/objs/%.o,$(OBJS)) $(BUILD_DIR)

# Compilation

%.o: %.c
	@$(CC) -c $< -o $(BUILD_DIR)/objs/$@ $(CFLAGS)

%.o: %.s
	@$(GAS) $< -o $(BUILD_DIR)/objs/$@ $(GASFLAGS)

%.o: %.asm
	@$(NASM) $< -o $(BUILD_DIR)/objs/$@ $(NASMFLAGS)
