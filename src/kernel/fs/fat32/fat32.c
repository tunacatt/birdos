/*
 * Adapted from Primus-OS:
 * https://github.com/leo-aa88/primus-os
 *
 * Original copyright and license notices retained.
 * Modified for BirdOS on 2026-10-18.
 *
 * Licensed under GNU GPL v3.0.
 */
#include <fs/fat32/fat32.h>
#include <drv/ata/ata.h>
#include <stdint.h>

static fat32_bpb_t bpb;
static uint32_t fat_start;
static uint32_t data_start;
static uint32_t root_cluster;

static uint32_t to_lba(uint32_t cluster) {
    return data_start + (cluster - 2) * bpb.sectors_per_cluster;
}

static uint32_t next_cluster(uint32_t cluster) {
    uint8_t sector[512];
    uint32_t fat_offset = cluster * 4;
    uint32_t fat_sector = fat_start + (fat_offset / 512);
    uint32_t offset = fat_offset % 512;

    ata_read_sector(fat_sector, sector);
    return *((uint32_t *)&sector[offset]) & 0x0FFFFFFF;
}

static void to_83_sfn(const char *n, char *out) {
    int i, j;
    for (i = 0; i < 11; i++) out[i] = ' ';
    for (i = 0, j = 0; n[i] && n[i] != '.' && j < 8; i++, j++)
        out[j] = (n[i] >= 'a' && n[i] <= 'z') ? n[i] - 32 : n[i];
    while (n[i] && n[i] != '.') i++;
    if (n[i] == '.') {
        i++;
        for (j = 8; n[i] && j < 11; i++, j++)
            out[j] = (n[i] >= 'a' && n[i] <= 'z') ? n[i] - 32 : n[i];
    }
    
}

int fat32_read_file(const char *name, uint8_t *buffer, uint32_t max_size) {
    char fname[11];
    to_83_sfn(name, fname);

    uint32_t cluster = root_cluster;
    uint8_t sector[512];

    while (cluster < 0x0FFFFFF8) {
        uint32_t lba = to_lba(cluster);
        for (uint32_t s = 0; s < bpb.sectors_per_cluster; s++) {
            ata_read_sector(lba + s, sector);
            fat32_entry_t *entry = (fat32_entry_t *)sector;
            for (int i = 0; i < 512 / 32; i++) {
                if (entry[i].name[0] == 0x00) return -1; // end of dir
                if (entry[i].name[0] == 0xE5) continue; // deleted
                if (entry[i].attributes & 0x0F) continue; // LFN
                int name_matches = 1;
                for (int j = 0; j < 11; j++) {
                    if (entry[i].name[j] != fname[j]) {
                        name_matches = 0;
                        break;
                    }
                }
                if (name_matches) {
                    // found — read data
                    uint32_t file_cluster = ((uint32_t)entry[i].cluster_high << 16) | entry[i].cluster_low;
                    uint32_t remaining = max_size;
                    uint32_t offset = 0;
                    while (file_cluster < 0x0FFFFFF8 && remaining > 0) {
                        uint32_t flba = to_lba(file_cluster);
                        for (uint32_t fs = 0; fs < bpb.sectors_per_cluster && remaining > 0; fs++) {
                            ata_read_sector(flba + fs, sector);
                            uint32_t to_copy = remaining > 512 ? 512 : remaining;
                            for (uint32_t b = 0; b < to_copy; b++)
                                buffer[offset++] = sector[b];
                            remaining -= to_copy;
                        }
                        file_cluster = next_cluster(file_cluster);
                    }
                    return offset;
                }
            }
        }
        cluster = next_cluster(cluster);
    }
    return -1;
}

int fat32_write_file(const char *name, uint8_t *buffer, uint32_t size) {
    char fname[11];
    to_83_sfn(name, fname);

    uint32_t cluster = root_cluster;
    uint8_t sector[512];

    while (cluster < 0x0FFFFFF8) {
        uint32_t lba = to_lba(cluster);
        for (uint32_t s = 0; s < bpb.sectors_per_cluster; s++) {
            ata_read_sector(lba + s, sector);
            fat32_entry_t *entry = (fat32_entry_t *)sector;
            for (int i = 0; i < 512 / 32; i++)
            {
                if (entry[i].name[0] == 0x00) return -1;
                if (entry[i].name[0] == 0xE5) continue;
                if (entry[i].attributes & 0x0F) continue; 
                int name_matches = 1;
                for (int j = 0; j < 11; j++) {
                    if (entry[i].name[j] != fname[j]) {
                        name_matches = 0;
                        break;
                    }
                }
                if (name_matches) {
                    uint32_t file_cluster = ((uint32_t)entry[i].cluster_high << 16) | entry[i].cluster_low;
                    uint32_t flba = to_lba(file_cluster);
                    uint8_t write_sector[512];
                    uint32_t offset = 0;
                    while (file_cluster < 0x0FFFFFF8 && offset < size) {
                        for (uint32_t fs = 0; fs < bpb.sectors_per_cluster && offset < size; fs++) {
                            for (int b = 0; b < 512; b++)
                                write_sector[b] = (offset < size) ? buffer[offset++] : 0;
                            ata_write_sector(flba + fs, write_sector);
                        }
                        file_cluster = next_cluster(file_cluster);
                        if (file_cluster < 0x0FFFFFF8) flba = to_lba(file_cluster);
                    }
                    return offset;
                }
            }
        }
        cluster = next_cluster(cluster);
    }
    return -1;
}
