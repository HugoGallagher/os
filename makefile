AS = i686-elf-as
CC = i686-elf-gcc

CFLAGS = -ffreestanding -O0 -g -Wall -Wextra -Ikernel/include/

SRC_DIR = ./kernel/src
INC_DIR = ./kernel/include
BUILD_DIR = ./kernel/build

ASRC = $(shell find $(SRC_DIR) -name *.S)
NASRC = $(shell find $(SRC_DIR) -name *.asm)
CSRC = $(shell find $(SRC_DIR) -name *.c)
HSRC = $(shell find $(INC_DIR) -name *.h)
AOBJ = $(subst $(SRC_DIR), $(BUILD_DIR), $(ASRC:.S=.o))
NAOBJ = $(subst $(SRC_DIR), $(BUILD_DIR), $(NASRC:.asm=.o))
COBJ = $(subst $(SRC_DIR), $(BUILD_DIR), $(CSRC:.c=.o))
OBJS = $(AOBJ) $(NAOBJ) $(COBJ)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c $(NAOBJ)
	mkdir -p $(@D)
	$(CC) -c $< -o $@ $(CFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.S
	mkdir -p $(@D)
	$(AS) -c $< -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.asm $(COBJ)
	mkdir -p $(@D)
	nasm -felf32 $< -o $@

kernel: clean $(OBJS) $(HSRC)
	$(CC) -T $(SRC_DIR)/linker.ld -o $(BUILD_DIR)/os.elf -ffreestanding -O2 -nostdlib $(OBJS) -lgcc
	cp $(BUILD_DIR)/os.elf isodir/boot/os.elf
	cp grub.cfg isodir/boot/grub/grub.cfg
	grub-mkrescue -o os.iso isodir

run: kernel
	qemu-system-i386 -m 1024 -cdrom os.iso

debug: kernel
	objcopy --only-keep-debug isodir/boot/os.elf isodir/boot/os.sym
	objcopy --strip-debug isodir/boot/os.elf
	qemu-system-i386 -s -S -m 1024 -cdrom os.iso

.PHONY: clean

clean:
	rm -r -f $(BUILD_DIR)/*
