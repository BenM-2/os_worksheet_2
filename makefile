DRIVERS = drivers/
SRC = source/
ISO = iso/
BOOT = $(ISO)boot/

BUILD_DIR = build/
BUILD_DRIVERS = $(BUILD_DIR)drivers/
BUILD_SOURCE = $(BUILD_DIR)source/

CC = gcc
AS = nasm
ASFLAGS = -f elf
LDFLAGS = -T ./$(SRC)link.ld -melf_i386

INC       := include
INC_DIRS  := $(INC) $(shell find $(INC) -mindepth 1 -type d)

CFLAGS = -m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector -nostartfiles -nodefaultlibs -Wall -Wextra -Werror $(addprefix -I,$(INC_DIRS)) -MMD -MP

ifeq ($(EXPERIMENTAL),1)
CFLAGS += -DFRAME_BUFFER_CUSTOM_CURSOR
endif

# Discover sources and map them to object paths
C_SRC		:= $(shell find $(SRC) -name '*.c')
ASM_SRC		:= $(shell find $(SRC) -name '*.asm')

C_OBJ		:= $(patsubst $(SRC)%.c,$(BUILD_DIR)%.o,$(C_SRC))
ASM_OBJ 	:= $(patsubst $(SRC)%.asm,$(BUILD_DIR)%.o,$(ASM_SRC))

LOADER_OBJ    := $(BUILD_DIR)loader.o
BUILD_OBJECTS := $(LOADER_OBJ) $(filter-out $(LOADER_OBJ),$(ASM_OBJ)) $(C_OBJ)


.PHONY: qemu_run qemu_run_quiet qemu_run_curses telnet iso dirs clean

dirs: 
	mkdir -p $(BUILD_DIR) $(BUILD_DRIVERS) $(BUILD_SOURCE)

qemu_run: | iso
	qemu-system-i386 -boot d -cdrom os.iso -m 32 -d cpu -D logQ.tx

qemu_run_quiet: | iso
	qemu-system-i386 -nographic -boot d -cdrom os.iso -m 32 

qemu_run_curses: | iso
	qemu-system-i386 -display curses \
	-monitor telnet::45454,server,nowait \
	-chardev stdio,id=char0 \
	-serial chardev:char0 \
	-boot d \
	-cdrom os.iso \
	-m 32 \
	-d cpu \
	-no-reboot \
	-no-shutdown \
	-D logQ.txt

telnet:
	telnet localhost 45454

iso: $(BOOT)kernel.elf
	genisoimage -R \
	-b boot/grub/stage2_eltorito \
	-no-emul-boot \
	-boot-load-size 4 \
	-A os \
	-input-charset utf8 \
	-quiet \
	-boot-info-table \
	-o os.iso \
	iso

$(BOOT)kernel.elf: $(BUILD_OBJECTS) | dirs
	ld $(LDFLAGS) $^ -o $@

$(BUILD_DIR)%.o: $(SRC)%.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@ 

$(BUILD_DIR)%.o: $(SRC)%.asm
	mkdir -p $(dir $@)
	$(AS) $(ASFLAGS) $< -o $@

clean:
	rm -rf $(BUILD_DIR)
	rm -f $(BOOT)kernel.elf
	rm -f os.iso

