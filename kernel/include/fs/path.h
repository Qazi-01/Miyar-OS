#ifndef PATH_H
#define PATH_H

#include <stdbool.h>
#include "drivers/disk.h"
#include "fs/directory.h"

#define PATH_MAX_COMPONENTS 16
#define PATH_MAX_NAME 64

typedef struct {
    char components[PATH_MAX_COMPONENTS][PATH_MAX_NAME];
    int count;
    bool absolute;
} path_t;

bool path_split(const char *path, path_t *result);
bool path_normalize(path_t *path);
bool path_lookup(const disk_t *disk, const char *path, uint32_t *parent_cluster, fat32_directory_entry_t *entry, char *leaf_name);
bool path_resolve(const disk_t *disk, const char *name, fat32_directory_entry_t *entry);

#endif