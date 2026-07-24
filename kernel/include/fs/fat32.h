#ifndef FAT32_H
#define FAT32_H

#include <stdint.h>
#include <stdbool.h>
#include "drivers/disk.h"

#pragma pack(push, 1)

typedef struct
{
    uint8_t jump[3];
    char oem[8];

    uint16_t bytes_per_sector;
    uint8_t sectors_per_cluster;
    uint16_t reserved_sector_count;
    uint8_t fat_count;
    uint16_t root_entry_count;
    uint16_t total_sectors16;
    uint8_t media_type;
    uint16_t fat_size16;
    uint16_t sectors_per_track;
    uint16_t head_count;
    uint32_t hidden_sectors;
    uint32_t total_sectors32;

    uint32_t fat_size32;
    uint16_t ext_flags;
    uint16_t fs_version;
    int32_t root_cluster;
    uint16_t fs_info;
    uint16_t backup_boot_sector;

    uint8_t reserved[12];

    uint8_t drive_number;
    uint8_t reserved1;
    uint8_t boot_signature;
    uint32_t volume_id;
    char volume_label[11];
    char fs_type[8];
} fat32_boot_sector_t;

#pragma pack(pop)

bool fat32_detect(const disk_t *disk);

#endif