CC = gcc
AS = nasm
CFLAGS = -m32 -ffreestanding -fno-stack-protector -nostdlib -O2 -Wall
LDFLAGS = -melf_i386 -nostdlib -Ttext 0x1000

KERNEL_OBJS = kernel/kernel.o kernel/screen.o kernel/input.o

all: czos.bin

kernel/%.o: kernel/%.c
	$(CC) $(CFLAGS) -c $< -o $@

boot/boot.o: boot/boot.asm
	$(AS) -f elf32 $< -o $@

czos.bin: boot/boot.o $(KERNEL_OBJS)
	ld $(LDFLAGS) boot/boot.o $(KERNEL_OBJS) -o czos.elf
	objcopy -O binary czos.elf czos.bin

clean:
	rm -f *.elf *.bin kernel/*.o boot/*.o
