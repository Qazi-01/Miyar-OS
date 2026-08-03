#ifndef FS_H
#define FS_H

#include <stdbool.h>
#include "drivers/disk.h"

typedef enum {
    FS_UNKNOWN = 0,
    FS_FAT32
} filesystem_type_t;

typedef struct {
    const disk_t *disk;
    filesystem_type_t type;
    uint32_t current_directory_cluster;
    char current_path[256];
    bool mounted;
} filesystem_t;

void fs_init(void);
const filesystem_t* fs_get(void);
bool fs_mount(const disk_t *disk);
uint32_t fs_current_directory(void);
bool fs_set_current_directory(uint32_t cluster, const char *path);
const char *fs_get_current_path(void);

#endif