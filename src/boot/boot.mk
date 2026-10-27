CC = i686-elf-gcc
CC64 = x86_64-elf-gcc
AS = nasm
LD = $(CC64)

ROOT_DIR := $(abspath $(dir $(lastword $(MAKEFILE_LIST)))/../..)
BUILD_DIR := $(ROOT_DIR)/build
SRC_DIR := $(ROOT_DIR)/src
BOOTSRC_DIR := $(SRC_DIR)/boot
OBJ_DIR   := $(BUILD_DIR)/objs/boot
ISO_DIR   := $(BUILD_DIR)/output/isodir
OUT_DIR   := $(BUILD_DIR)/output/iso
LINKER    := $(BUILD_DIR)/conf/linker.ld

CFLAGS = -std=gnu99 -ffreestanding -fno-pie -fno-stack-protector -mno-sse -mno-sse2 -mno-mmx -msoft-float -O2 -Wall -Wextra -I$(SRC_DIR)/incl -I$(SRC_DIR)/kernel -MMD -MP

BOOT_OBJS = \
	$(OBJ_DIR)/boot_entry.o \
	$(OBJ_DIR)/boot_longmode.o \
	$(OBJ_DIR)/boot_paging.o \
	$(OBJ_DIR)/boot_check.o \
	$(OBJ_DIR)/boot_gdt.o 

DEBUG_SOURCES_AS32 = \
$(BOOTSRC_DIR)/boot.asm \
$(BOOTSRC_DIR)/paging.asm \
$(SRC_DIR)/arch/x86/cpu/gdt/gdt64.asm

DEBUG_SOURCES_AS64 = $(BOOTSRC_DIR)/longmode.asm

DEBUG_SOURCES_C32 = $(BOOTSRC_DIR)/check.c


.PHONY: all clean

all: $(OBJ_DIR)/boot.o

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(OBJ_DIR)/boot_entry.o: $(SRC_DIR)/boot/boot.asm | $(OBJ_DIR)
	$(AS) -f elf64 $< -o $@

$(OBJ_DIR)/boot_longmode.o: $(SRC_DIR)/boot/longmode.asm | $(OBJ_DIR)
	$(AS) -f elf64 $< -o $@

$(OBJ_DIR)/boot_paging.o: $(SRC_DIR)/boot/paging.asm | $(OBJ_DIR)
	$(AS) -f elf64 $< -o $@

$(OBJ_DIR)/boot_check.o: $(SRC_DIR)/boot/check.c | $(OBJ_DIR)
	$(CC64) $(CFLAGS) -m64 -c $< -o $@

$(OBJ_DIR)/boot_gdt.o: $(SRC_DIR)/arch/x86/kernel/cpu/gdt/gdt64.asm | $(OBJ_DIR)
	$(AS) -f elf64 $< -o $@

$(OBJ_DIR)/boot.o: $(BOOT_OBJS)
	$(LD) -r -o $@ $^



clean:
	rm -f $(BOOT_OBJS) $(OBJ_DIR)/boot.o
