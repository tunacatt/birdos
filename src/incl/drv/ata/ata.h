/*
 * Adapted from Primus-OS:
 * https://github.com/leo-aa88/primus-os
 *
 * Original copyright and license notices retained.
 * Modified for BirdOS on 2026-10-18.
 *
 * Licensed under GNU GPL v3.0.
 */
#ifndef ATA_H
#define ATA_H

#include <stdint.h>
#include <stdbool.h>
#include <io/stdio.h>

#define ATA_PRI_DATA 0x1F0 // primary data
#define ATA_PRI_SECTORS 0x1F2 // sector count
// LBA - logical block addressing: e.g. LBA0 - bootsector, LBA1 - sector1 (1 sector = 512 bytes, for ATA)
// The sector address is split into four pieces (each is 8 bits), making LBA low, mid, high and the head
#define ATA_PRI_LBA_LOW 0x1F3 // LBA low
#define ATA_PRI_LBA_MID 0x1F4 // LBA mid
#define ATA_PRI_LBA_HIGH 0x1F5 // LBA high
#define ATA_PRI_DRIVE_HEAD 0x1F6 // Drive/head
#define ATA_PRI_STAT 0x1F7 // status
#define ATA_PRI_CMD 0x1F7 // command

#define ATA_BSY 0x80 // is the drive busy?
#define ATA_DRQ 0x08 // is the drive ready for a command?
#define ATA_ERR 0x01 // error

#define ATA_PRI_R 0x20 // read
#define ATA_PRI_W 0x30 // write

void ata_read_sector(uint32_t lba, uint8_t *buffer);
void ata_write_sector(uint32_t lba, uint8_t *buffer);

#endif