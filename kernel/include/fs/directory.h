#ifndef DIRECTORY_H
#define DIRECTORY_H

#include <stdint.h>
#include "drivers/disk.h"

#pragma pack(push, 1)

typedef struct {
    char name[11];
    uint8_t attributes;
    uint8_t reserved;
    uint8_t creation_time_tenths;
    uint16_t creation_time;
    uint16_t creation_date;
    uint16_t last_access_date;
    uint16_t first_cluster_high;
    uint16_t write_time;
    uint16_t write_date;
    uint16_t first_cluster_low;
    uint32_t file_size;
} fat32_directory_entry_t;

#pragma pack(pop)

typedef struct {
    const disk_t *disk;

    uint32_t cluster;
    uint32_t current_cluster;
    uint8_t sector[512];

    int index;
} directory_t;

bool directory_get_name(const fat32_directory_entry_t *entry, char *output);
bool directory_read_root(const disk_t *disk);
bool directory_open_root(const disk_t *disk, directory_t *dir);
bool directory_next(directory_t *dir, fat32_directory_entry_t *entry);
bool directory_create_entry(const disk_t *disk, const fat32_directory_entry_t *entry);

#endif