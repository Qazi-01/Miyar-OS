#ifndef DIRECTORY_H
#define DIRECTORY_H

#include <stdint.h>
#include <stdbool.h>

#include "drivers/disk.h"

#define FAT32_ATTR_READ_ONLY  0x01
#define FAT32_ATTR_HIDDEN     0x02
#define FAT32_ATTR_SYSTEM     0x04
#define FAT32_ATTR_VOLUME_ID  0x08
#define FAT32_ATTR_DIRECTORY  0x10
#define FAT32_ATTR_ARCHIVE    0x20
#define FAT32_ATTR_LFN        0x0F

#pragma pack(push, 1)

typedef struct
{
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

typedef struct
{
    const disk_t *disk;

    uint32_t cluster;
    uint32_t current_cluster;

    uint8_t sector[512];

    uint32_t sector_index;
    uint32_t entry_index;
} directory_t;

bool directory_get_name(const fat32_directory_entry_t *entry, char *output);
bool directory_read_root(const disk_t *disk);
bool directory_open_root(const disk_t *disk, directory_t *dir);
bool directory_open(const disk_t *disk, uint32_t cluster, directory_t *dir);
bool directory_next(directory_t *dir, fat32_directory_entry_t *entry);
bool directory_create_entry(const disk_t *disk, const fat32_directory_entry_t *entry);
bool directory_find(const disk_t *disk, const char *name, fat32_directory_entry_t *entry);
void directory_set_name(fat32_directory_entry_t *entry, const char *name);
bool directory_create(const disk_t *disk, const char *name);
bool directory_delete(const disk_t *disk, const char *name);
bool directory_is_empty(const disk_t *disk, const fat32_directory_entry_t *entry);
bool directory_remove(const disk_t *disk, const char *name);
bool directory_update_entry(const disk_t *disk, uint32_t parent_cluster, const fat32_directory_entry_t *entry);
bool directory_move_entry(const disk_t *disk, uint32_t source_cluster, uint32_t destination_cluster, const char *old_name, fat32_directory_entry_t *entry);
bool directory_rename_entry(const disk_t *disk, uint32_t parent_cluster, const char *old_name, const fat32_directory_entry_t *entry);
bool directory_rename(const disk_t *disk, const char *old_name, const char *new_name);
bool directory_find_in_cluster(const disk_t *disk, uint32_t cluster, const char *name, fat32_directory_entry_t *entry);
bool directory_change(const disk_t *disk, const char *name);
uint32_t directory_entry_cluster(const fat32_directory_entry_t *entry);
void directory_set_entry_cluster(fat32_directory_entry_t *entry, uint32_t cluster);

#endif