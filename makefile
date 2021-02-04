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

run: kernel
	sudo cp $(BUILD_DIR)/os.elf /mnt/os/boot/os.elf
	sudo cp grub.cfg /mnt/os/boot/grub/grub.cfg
	sudo umount /mnt/os
	sudo mount /dev/loop41 /mnt/os
	qemu-system-i386 -m 1024 -drive format=raw,file=disk.img

debug: kernel
	sudo cp $(BUILD_DIR)/os.elf /mnt/os/boot/os.elf
	sudo cp grub.cfg /mnt/os/boot/grub/grub.cfg
	sudo umount /mnt/os
	sudo mount /dev/loop41 /mnt/os
	objcopy --only-keep-debug $(BUILD_DIR)/os.elf $(BUILD_DIR)/os.sym
	objcopy --strip-debug $(BUILD_DIR)/os.elf
	qemu-system-i386 -m 1024 -drive format=raw,file=disk.img

mount:
	sudo losetup /dev/loop40 disk.img
	sudo losetup /dev/loop41 disk.img -o 1048576
	sudo mkdosfs -F32 -f 2 /dev/loop41
	sudo mount /dev/loop41 /mnt/os
	sudo grub-install --boot-directory /mnt/os/boot --no-floppy --modules="normal part_msdos fat configfile multiboot biosdisk" /dev/loop40

.PHONY: clean

clean:
	rm -r -f $(BUILD_DIR)/*
