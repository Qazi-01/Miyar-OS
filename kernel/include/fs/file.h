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
    uint32_t size;
    uint32_t position;
} file_t;

bool file_open(const disk_t *disk, const fat32_directory_entry_t *entry, file_t *file);
int file_read(file_t *file, void *buffer, uint32_t size);

#endif