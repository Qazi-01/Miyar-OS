#include "fs/fs.h"
#include "fs/fat32.h"

static filesystem_t filesystem;

void fs_init(void)
{
    filesystem.disk = 0;
    filesystem.type = FS_UNKNOWN;
    filesystem.mounted = false;
}

const filesystem_t *fs_get(void)
{
    return &filesystem;
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

        return true;
    }

    filesystem.disk = 0;
    filesystem.type = FS_UNKNOWN;
    filesystem.mounted = false;

    return false;
}