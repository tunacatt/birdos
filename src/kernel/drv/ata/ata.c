/*
 * Adapted from Primus-OS:
 * https://github.com/leo-aa88/primus-os
 *
 * Original copyright and license notices retained.
 * Modified for BirdOS on 2026-10-18.
 *
 * Licensed under GNU GPL v3.0.
 */
#include <io/stdio.h>
#include <stdint.h>
#include <drv/ata/ata.h>

static void ata_wait_bsy(void)
{
    while (inb(ATA_PRI_STAT) & ATA_BSY)
        ;
}

static void ata_wait_drq(void)
{
    while (!(inb(ATA_PRI_STAT) & ATA_DRQ))
        ;
}

void ata_read_sector(uint32_t lba, uint8_t *buffer) {
    ata_wait_bsy();
    outb(ATA_PRI_DRIVE_HEAD, 0xE0 | ((lba >> 24) & 0x0F));
    outb(ATA_PRI_SECTORS, 1);
    outb(ATA_PRI_LBA_LOW, (uint8_t)(lba));
    outb(ATA_PRI_LBA_MID, (uint8_t)(lba >> 8));
    outb(ATA_PRI_LBA_HIGH, (uint8_t)(lba >> 16));
    outb(ATA_PRI_CMD, ATA_PRI_R);
    ata_wait_bsy();
    ata_wait_drq();

    uint16_t *buf16 = (uint16_t *)buffer;
    for (int i = 0; i < 256; i++) {
        uint16_t data;
        __asm__ volatile ("inw %1, %0" : "=a"(data) : "Nd"((uint16_t)ATA_PRI_DATA));
        buf16[i] = data;
    }
}

void ata_write_sector(uint32_t lba, uint8_t *buffer) {
    ata_wait_bsy();
    outb(ATA_PRI_DRIVE_HEAD, 0xE0 | ((lba >> 24) & 0x0F));
    outb(ATA_PRI_SECTORS, 1);
    outb(ATA_PRI_LBA_LOW, (uint8_t)(lba));
    outb(ATA_PRI_LBA_MID, (uint8_t)(lba >> 8));
    outb(ATA_PRI_LBA_HIGH, (uint8_t)(lba >> 16));
    outb(ATA_PRI_CMD, ATA_PRI_W);
    ata_wait_bsy();
    ata_wait_drq();

    uint16_t *buf16 = (uint16_t *)buffer;
    for (int i = 0; i < 256; i++) {
        __asm__ volatile ("outw %0, %1" : : "a"(buf16[i]), "Nd"((uint16_t)ATA_PRI_DATA));
    }
}