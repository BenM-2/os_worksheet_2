DRIVERS = drivers/
SRC = source/
ISO = iso/
BOOT = $(ISO)boot/

BUILD_DIR = build/
BUILD_DRIVERS = $(BUILD_DIR)drivers/
BUILD_SOURCE = $(BUILD_DIR)source/

CC = gcc
CFLAGS = -m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector -nostartfiles -nodefaultlibs -Wall -Wextra -Werror -I$(DRIVERS)/Include
ifeq ($(EXPERIMENTAL),1)
CFLAGS += -DFRAME_BUFFER_CUSTOM_CURSOR
endif
AS = nasm
ASFLAGS = -f elf
LDFLAGS = -T ./$(SRC)link.ld -melf_i386

# Discover sources and map them to object paths
SRC_C_FILES     := $(wildcard $(SRC)*.c)
DRIVERS_C_FILES := $(wildcard $(DRIVERS)*.c)

SRC_OBJECTS     := $(patsubst $(SRC)%.c,$(BUILD_SOURCE)%.o,$(SRC_C_FILES))
DRIVERS_OBJECTS := $(patsubst $(DRIVERS)%.c,$(BUILD_DRIVERS)%.o,$(DRIVERS_C_FILES))

LOADER_OBJ      := $(BUILD_SOURCE)loader.o
SRC_ASM_FILES   := $(filter-out $(SRC)loader.asm,$(wildcard $(SRC)*.asm))
SRC_ASM_OBJECTS := $(patsubst $(SRC)%.asm,$(BUILD_SOURCE)%.o,$(SRC_ASM_FILES))

DRIVERS_ASM_FILES := $(wildcard $(DRIVERS)*.asm)
DRIVERS_ASM_OBJECTS := $(patsubst $(DRIVERS)%.asm,$(BUILD_DRIVERS)%.o,$(DRIVERS_ASM_FILES))

BUILD_OBJECTS = $(LOADER_OBJ) $(SRC_ASM_OBJECTS) $(SRC_OBJECTS) $(DRIVERS_ASM_OBJECTS) $(DRIVERS_OBJECTS)

.PHONY: qemu_run qemu_run_quiet qemu_run_curses telnet iso dirs clean

dirs: 
	mkdir -p $(BUILD_DIR) $(BUILD_DRIVERS) $(BUILD_SOURCE)

qemu_run: | iso
	qemu-system-i386 -boot d -cdrom os.iso -m 32 -d cpu -D logQ.tx

qemu_run_quiet: | iso
	qemu-system-i386 -nographic -boot d -cdrom os.iso -m 32 

# qemu_run_curses: | iso
# 	qemu-system-i386 -display curses \
#   -monitor telnet::45454,server,nowait \
#   -serial mon::stdin \
#   -boot d -cdrom os.iso -m 32 \
#   -d cpu -D logQ.txt
# qemu_run_curses: | iso
# 	qemu-system-i386 -display curses \
# 		-monitor telnet::45454,server,nowait \
# 		-serial telnet::45455,server,nowait \
# 		-boot d -cdrom os.iso -m 32 \
# 		-d int,cpu_reset -D logQ.txt \
# 		-trace 'ps2_*' -trace 'input_event_*'
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

$(BUILD_SOURCE)%.o: $(SRC)%.c | dirs
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_SOURCE)%.o: $(SRC)%.asm | dirs
	$(AS) $(ASFLAGS) $< -o $@

$(BUILD_DRIVERS)%.o: $(DRIVERS)%.c | dirs
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DRIVERS)%.o: $(DRIVERS)%.asm | dirs
	$(AS) $(ASFLAGS) $< -o $@


clean:
	rm -rf $(BUILD_DIR)
	rm -f $(BOOT)kernel.elf
	rm -f os.iso

