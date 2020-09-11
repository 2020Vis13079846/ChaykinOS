# Makefile

# OS information

AUTHOR      = Simon Chaykin
VERSION     = 0.7.4
LICENSE     = WTFPL

# Makefile configuration

BUILD_DIR   = build
INCLUDE_DIR = include
ISO_DIR     = $(BUILD_DIR)/iso
Q           = @

# Files

LSCRIPT     = link.ld
GRUB_CONFIG = grub.cfg
ISOFILE     = ChaykinOS-$(VERSION).iso
BINFILE     = kernel.$(BINFORMAT)
LOGFILE     = chaykinos.log

# Compiler settings

BINFORMAT   = elf

# Compilers and emulators

MSG         = echo
CC          = gcc
GAS         = as # AT&T-syntax assembler
NASM        = nasm # Intel-syntax
EMU         = qemu-system-i386
GRUB        = grub-mkrescue

# Compilers' and emulators' flags

CFLAGS      = -m32 -std=c99 -ffreestanding -Wall -Wextra -I $(INCLUDE_DIR) -D __is_kernel -D __is_libk -Wall -Wextra -pedantic-errors
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

all: directories kernel grub iso

directories:
	$(Q) mkdir -p $(BUILD_DIR)/bin
	$(Q) mkdir -p $(BUILD_DIR)/iso/boot/grub
	$(Q) mkdir -p $(patsubst %,"$(BUILD_DIR)/objs/%",$(SOURCE_DIRS))

kernel: $(OBJS)
	$(Q) $(CC) -o $(BUILD_DIR)/bin/$(BINFILE) $(LDFLAGS) "$(CRTBEGIN)" $(patsubst %.o,./$(BUILD_DIR)/objs/%.o,$(OBJS)) "$(CRTEND)"

grub:
	$(Q) $(MSG) "menuentry \"ChaykinOS $(VERSION)\" {" > $(BUILD_DIR)/iso/boot/grub/grub.cfg
	$(Q) $(MSG) "	multiboot /boot/kernel.elf" >> $(BUILD_DIR)/iso/boot/grub/grub.cfg
	$(Q) $(MSG) "	boot" >> $(BUILD_DIR)/iso/boot/grub/grub.cfg
	$(Q) $(MSG) "}" >> $(BUILD_DIR)/iso/boot/grub/grub.cfg

iso: grub kernel
	$(Q) cp $(BUILD_DIR)/bin/$(BINFILE) $(BUILD_DIR)/iso/boot/$(BINFILE)
	$(Q) $(GRUB) -o $(ISOFILE) $(BUILD_DIR)/iso

run: iso
	$(Q) $(EMU) -cdrom $(ISOFILE) $(EMUFLAGS)

clean:
	$(Q) rm -rf $(patsubst %.o,./$(BUILD_DIR)/objs/%.o,$(OBJS)) $(BUILD_DIR)

# Compilation

%.o: %.c
	$(Q) $(MSG) [Compile] $<
	$(Q) $(CC) -c $< -o $(BUILD_DIR)/objs/$@ $(CFLAGS)

%.o: %.s
	$(Q) $(MSG) [Compile] $<
	$(Q) $(GAS) $< -o $(BUILD_DIR)/objs/$@ $(GASFLAGS)

%.o: %.asm
	$(Q) $(MSG) [Compile] $<
	$(Q) $(NASM) $< -o $(BUILD_DIR)/objs/$@ $(NASMFLAGS)
