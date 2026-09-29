DRIVERS = drivers/
SRC = source/
ISO = iso/
BOOT = $(ISO)boot/

BUILD_DIR = build/
BUILD_DRIVERS = $(BUILD_DIR)drivers/
BUILD_SOURCE = $(BUILD_DIR)source/


.PHONY: qemu_run qemu_run_quiet iso dirs clean

dirs: 
	mkdir -p $(BUILD_DIR) $(BUILD_DRIVERS) $(BUILD_SOURCE)

qemu_run: | iso
	qemu-system-i386 -nographic -boot d -cdrom os.iso -m 32 -d cpu -D logQ.tx

qemu_run_quiet: | iso
	qemu-system-i386 -nographic -boot d -cdrom os.iso -m 32 

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

$(BOOT)kernel.elf: $(BUILD_SOURCE)loader.o | dirs
	ld -T ./$(SRC)link.ld -melf_i386 $(BUILD_SOURCE)loader.o -o $(BOOT)kernel.elf

$(BUILD_SOURCE)loader.o: | dirs
	nasm -f elf $(SRC)loader.asm -o $(BUILD_SOURCE)loader.o

clean:
	rm -rf $(BUILD_DIR)
	rm -f $(BOOT)kernel.elf
	rm -f os.iso

