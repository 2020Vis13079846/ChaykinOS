#!/bin/bash

if [[ -d ~/opt/cross/bin ]]
then
	sleep 0
else
	echo "Can't find compiler"
fi

export PATH=$PATH:~/opt/cross/bin

export PREFIX=i686-elf
export CC=$PREFIX-gcc
export NASM=nasm

mkdir -p build/bin
mkdir -p build/iso/boot/grub
mkdir -p build/objs/init
mkdir -p build/objs/boot
mkdir -p build/objs/kernel
mkdir -p build/releases

$NASM -felf32 boot/boot.asm -o build/objs/boot/boot.o
$CC -c init/main.c -o build/objs/init/main.o -std=gnu99 -ffreestanding -Wall -Wextra -I include
$CC -c kernel/tty.c -o build/objs/kernel/tty.o -std=gnu99 -ffreestanding -Wall -Wextra -I include
$CC -T link.ld -o build/bin/kernel.elf -ffreestanding -nostdlib -lgcc build/objs/boot/boot.o build/objs/init/main.o build/objs/kernel/tty.o
cp build/bin/kernel.elf build/iso/boot/kernel.elf
if grub-file --is-x86-multiboot build/bin/kernel.elf
then
	echo "Multiboot confirmed."
else
	echo "The file isn't multiboot."
	exit
fi

cat << EOF > build/iso/boot/grub/grub.cfg
menuentry "ChaykinOS 0.0.1" {
	multiboot /boot/kernel.elf
	boot
}
EOF

grub-mkrescue -o build/releases/ChaykinOS-0.0.1.iso build/iso
cp build/releases/ChaykinOS-0.0.1.iso ChaykinOS-0.0.1.iso

if [[ $1 = 'run' ]]
then
	qemu-system-i386 -cdrom ChaykinOS-0.0.1.iso
fi
