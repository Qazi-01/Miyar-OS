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
    bool mounted;
} filesystem_t;

void fs_init(void);
const filesystem_t* fs_get(void);
bool fs_mount(const disk_t *disk);

#endif