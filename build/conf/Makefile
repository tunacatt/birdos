# -----------------------------------
# BirdOS Makefile - building the OS
# -----------------------------------

# makefile looks like a language i would program the OS in lol
# (i kinda am though, it is automation for building :)
CC = i686-elf-gcc
CC64 = x86_64-elf-gcc
AS = nasm

ROOT_DIR  := $(abspath $(dir $(lastword $(MAKEFILE_LIST)))/../..)
SRC_DIR   := $(ROOT_DIR)/src
KRNLSRC_DIR := $(SRC_DIR)/kernel
BUILD_DIR := $(ROOT_DIR)/build
OBJ_DIR   := $(BUILD_DIR)/objs
ISO_DIR   := $(BUILD_DIR)/output/isodir
OUT_DIR   := $(BUILD_DIR)/output/iso
LINKER    := $(ROOT_DIR)/build/conf/linker.ld

CFLAGS  = -std=gnu99 -ffreestanding -O2 -Wall -Wno-switch -Wextra -m64 -fno-stack-protector -mno-red-zone -mcmodel=kernel -I$(SRC_DIR)/incl -I$(KRNLSRC_DIR) -MMD -MP
LDFLAGS = -ffreestanding -O2 -nostdlib

OBJS = \
	$(OBJ_DIR)/boot.o \
	$(OBJ_DIR)/gdt.o \
	$(OBJ_DIR)/idt.o \
	$(OBJ_DIR)/isr.o \
	$(OBJ_DIR)/pic.o \
	$(OBJ_DIR)/keyboard.o \
	$(OBJ_DIR)/kblayout.o \
	$(OBJ_DIR)/us_qwerty_layout.o \
	$(OBJ_DIR)/kernel_main.o \
	$(OBJ_DIR)/kernel_load.o \
	$(OBJ_DIR)/control.o \
	$(OBJ_DIR)/io.o \
	$(OBJ_DIR)/vga_terminal.o \
	$(OBJ_DIR)/sysinfo.o \
	$(OBJ_DIR)/kshell.o \
	$(OBJ_DIR)/qemupwr.o \
	$(OBJ_DIR)/panic.o \
	$(OBJ_DIR)/crashtest.o \
	$(OBJ_DIR)/check.o \
	$(OBJ_DIR)/tss.o

AGGREGATE_OBJS = \
	$(OBJ_DIR)/sysinfo_cpuid.o \
	$(OBJ_DIR)/sysinfo_impl.o \
	$(OBJ_DIR)/io_stdio.o \
	$(OBJ_DIR)/io_qemu.o

DEPS = $(OBJS:.o=.d) $(AGGREGATE_OBJS:.o=.d)
DEBUG_C_SOURCES = \
	$(KRNLSRC_DIR)/cpu/interrupt/isr.c \
	$(KRNLSRC_DIR)/cpu/interrupt/pic.c \
	$(KRNLSRC_DIR)/drv/ps2/keyboard/keyboard.c \
	$(KRNLSRC_DIR)/drv/ata/ata.c \
	$(KRNLSRC_DIR)/fs/fat32/fat32.c \
	$(SRC_DIR)/etc/kblayout/kblayout.c \
	$(SRC_DIR)/etc/kblayout/us_qwerty/layout.c \
	$(KRNLSRC_DIR)/core/kernel_main.c \
	$(KRNLSRC_DIR)/core/kernel_load.c \
	$(KRNLSRC_DIR)/cpu/control.c \
	$(KRNLSRC_DIR)/io/stdio.c \
	$(KRNLSRC_DIR)/qemu/io.c \
	$(KRNLSRC_DIR)/cpu/cpuid.c \
	$(SRC_DIR)/etc/sysinfo/sysinfo.c \
	$(SRC_DIR)/shell/vga_term/terminal.c \
	$(SRC_DIR)/shell/kshell/shell.c \
	$(KRNLSRC_DIR)/kernel/panic.c \
	$(SRC_DIR)/etc/crashtest/crashtest.c \
	$(KRNLSRC_DIR)/cpu/check.c \
	$(KRNLSRC_DIR)/cpu/tss/tss.c \
DEBUG_ASM_SOURCES = \
	$(SRC_DIR)/boot/boot.asm \
	$(KRNLSRC_DIR)/cpu/gdt.asm \
	$(KRNLSRC_DIR)/cpu/idt.asm \
	$(KRNLSRC_DIR)/qemu/pwr.asm

.PHONY: all iso build clean run debug

all: $(BUILD_DIR)/birdos.bin

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(OBJ_DIR)/boot.o: $(SRC_DIR)/boot/boot.asm | $(OBJ_DIR)
	$(AS) -f elf32 $< -o $@

$(OBJ_DIR)/gdt.o: $(KRNLSRC_DIR)/cpu/gdt.asm | $(OBJ_DIR)
	$(AS) -f elf32 $< -o $@

$(OBJ_DIR)/idt.o: $(KRNLSRC_DIR)/cpu/idt.asm | $(OBJ_DIR)
	$(AS) -f elf32 $< -o $@

$(OBJ_DIR)/isr.o: $(KRNLSRC_DIR)/cpu/interrupt/isr.c | $(OBJ_DIR)
	$(CC64) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/pic.o: $(KRNLSRC_DIR)/cpu/interrupt/pic.c | $(OBJ_DIR)
	$(CC64) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/keyboard.o: $(KRNLSRC_DIR)/drv/ps2/keyboard/keyboard.c | $(OBJ_DIR)
	$(CC64) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/kblayout.o: $(SRC_DIR)/etc/kblayout/kblayout.c | $(OBJ_DIR)
	$(CC64) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/us_qwerty_layout.o: $(SRC_DIR)/etc/kblayout/us_qwerty/layout.c | $(OBJ_DIR)
	$(CC64) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/kernel_main.o: $(KRNLSRC_DIR)/core/kernel_main.c | $(OBJ_DIR)
	$(CC64) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/kernel_load.o: $(KRNLSRC_DIR)/core/kernel_load.c | $(OBJ_DIR)
	$(CC64) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/control.o: $(KRNLSRC_DIR)/cpu/control.c | $(OBJ_DIR)
	$(CC64) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/io_stdio.o: $(KRNLSRC_DIR)/io/stdio.c | $(OBJ_DIR)
	$(CC64) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/io_qemu.o: $(KRNLSRC_DIR)/qemu/io.c | $(OBJ_DIR)
	$(CC64) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/io.o: $(OBJ_DIR)/io_stdio.o $(OBJ_DIR)/io_qemu.o
	$(CC64) -r -o $@ $^

$(OBJ_DIR)/sysinfo_cpuid.o: $(KRNLSRC_DIR)/cpu/cpuid.c | $(OBJ_DIR)
	$(CC64) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/sysinfo_impl.o: $(SRC_DIR)/etc/sysinfo/sysinfo.c | $(OBJ_DIR)
	$(CC64) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/sysinfo.o: $(OBJ_DIR)/sysinfo_cpuid.o $(OBJ_DIR)/sysinfo_impl.o
	$(CC64) -r -o $@ $^

$(OBJ_DIR)/vga_terminal.o: $(SRC_DIR)/shell/vga_term/terminal.c | $(OBJ_DIR)
	$(CC64) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/kshell.o: $(SRC_DIR)/shell/kshell/shell.c | $(OBJ_DIR)
	$(CC64) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/qemupwr.o: $(KRNLSRC_DIR)/qemu/pwr.asm | $(OBJ_DIR)
	$(AS) -f elf32 $< -o $@

$(OBJ_DIR)/panic.o: $(KRNLSRC_DIR)/kernel/panic.c | $(OBJ_DIR)
	$(CC64) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/crashtest.o: $(SRC_DIR)/etc/crashtest/crashtest.c | $(OBJ_DIR)
	$(CC64) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/check.o: $(KRNLSRC_DIR)/cpu/check.c | $(OBJ_DIR)
	$(CC64) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/tss.o: $(KRNLSRC_DIR)/cpu/tss/tss.c | $(OBJ_DIR)
	$(CC64) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/birdos.bin: $(OBJS) $(LINKER)
	$(CC64) -T $(LINKER) -o $@ $(LDFLAGS) $(OBJS) -lgCC64

iso: $(BUILD_DIR)/birdos.bin
	mkdir -p $(ISO_DIR)/boot/grub
	mkdir -p $(OUT_DIR)
	cp $(BUILD_DIR)/birdos.bin $(ISO_DIR)/boot/birdos.bin
	cp $(SRC_DIR)/boot/grub.cfg $(ISO_DIR)/boot/grub/grub.cfg
	grub2-mkrescue -o $(OUT_DIR)/birdos.iso $(ISO_DIR)

build: iso

clean:
	rm -rf $(BUILD_DIR)/objs $(BUILD_DIR)/birdos.bin $(BUILD_DIR)/output

run:
	qemu-system-i386 -cdrom $(OUT_DIR)/birdos.iso -debugcon stdio

debug:
	@status=0; \
	for source in $(DEBUG_C_SOURCES); do \
		echo "Checking $$source"; \
		$(CC64) $(filter-out -MMD -MP,$(CFLAGS)) -fsyntax-only "$$source" || status=1; \
	done; \
	for source in $(DEBUG_ASM_SOURCES); do \
		echo "Checking $$source"; \
		$(AS) -f elf32 "$$source" -o /dev/null || status=1; \
	done; \
	exit $$status

-include $(DEPS)