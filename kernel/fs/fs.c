#include "fs/fs.h"
#include "fs/fat32.h"
#include "lib/string.h"

static filesystem_t filesystem;

void fs_init(void)
{
    filesystem.disk = 0;
    filesystem.type = FS_UNKNOWN;
    filesystem.current_directory_cluster = 0;
    strcpy(filesystem.current_path, "/");
    filesystem.mounted = false;
}

const filesystem_t *fs_get(void)
{
    return &filesystem;
}

uint32_t fs_current_directory(void)
{
    return filesystem.current_directory_cluster;
}

const char *fs_get_current_path(void)
{
    return filesystem.current_path;
}

bool fs_set_current_directory(uint32_t cluster, const char *path)
{
    if (path == 0)
    {
        return false;
    }

    filesystem.current_directory_cluster = cluster;
    strcpy(filesystem.current_path, path);

    return true;
}

bool fs_mount(const disk_t *disk)
{
    if (disk == 0)
    {
        return false;
    }

    if (fat32_detect(disk))
    {
        filesystem.disk = disk;
        filesystem.type = FS_FAT32;
        filesystem.mounted = true;

        filesystem.current_directory_cluster = fat32_get_filesystem()->root_cluster;
        strcpy(filesystem.current_path, "/");

        return true;
    }

    filesystem.disk = 0;
    filesystem.type = FS_UNKNOWN;
    filesystem.mounted = false;

    return false;
}