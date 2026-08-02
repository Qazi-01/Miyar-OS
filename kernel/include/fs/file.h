#ifndef FILE_H
#define FILE_H

#include <stdint.h>
#include <stdbool.h>
#include "drivers/disk.h"
#include "fs/directory.h"

typedef struct {
    const disk_t *disk;

    uint32_t first_cluster;
    uint32_t current_cluster;
    uint32_t current_cluster_index;
    uint32_t size;
    uint32_t position;
    uint32_t parent_cluster;
    fat32_directory_entry_t entry;
} file_t;

bool file_open(const disk_t *disk, uint32_t parent_cluster, const fat32_directory_entry_t *entry, file_t *file);
int file_read(file_t *file, void *buffer, uint32_t size);
bool file_create(const disk_t *disk, const char *name);
bool file_delete(const disk_t *disk, const char *name);
int file_write(file_t *file, const void *buffer, uint32_t size);
int file_append(file_t *file, const void *buffer, uint32_t size);
bool file_copy(const disk_t *disk, const char *source, const char *destination);
bool file_move(const disk_t *disk, const char *old_name, const char *new_name);

#endif