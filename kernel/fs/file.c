#include "drivers/disk.h"
#include "fs/file.h"
#include "fs/fat32.h"
#include "lib/string.h"

bool file_open(const disk_t *disk, const fat32_directory_entry_t *entry, file_t *file)
{
    if (disk == 0 || entry == 0 || file == 0)
    {
        return false;
    }

    file->disk = disk;
    file->first_cluster = ((uint32_t)entry->first_cluster_high << 16) | entry->first_cluster_low;
    file->current_cluster = file->first_cluster;
    file->size = entry->file_size;
    file->position = 0;

    return true;
}

int file_read(file_t *file, void *buffer, uint32_t size)
{
    if (file == 0 || buffer == 0)
    {
        return -1;
    }

    uint8_t sector[512];
    uint32_t sector_number = fat32_cluster_to_sector(file->current_cluster);

    if (disk_read(file->disk, sector_number, sector) != 0)
    {
        return -1;
    }

    uint32_t bytes = size;

    if (bytes > file->size)
    {
        bytes = file->size;
    }

    memcpy(buffer, sector, bytes);
    file->position += bytes;

    return bytes;
}

bool file_create(const disk_t *disk, const char *name)
{
    (void)disk;
    (void)name;
    return false;
}

bool file_delete(const disk_t *disk, const char *name)
{
    (void)disk;
    (void)name;
    return false;
}

int file_write(file_t *file, const void *buffer, uint32_t size)
{
    (void)file;
    (void)buffer;
    (void)size;
    return -1;
}

int file_append(file_t *file, const void *buffer, uint32_t size)
{
    (void)file;
    (void)buffer;
    (void)size;
    return -1;
}

bool file_copy(const disk_t *disk, const char *source, const char *destination)
{
    (void)disk;
    (void)source;
    (void)destination;
    return false;
}

bool file_move(const disk_t *disk, const char *old_name, const char *new_name)
{
    (void)disk;
    (void)old_name;
    (void)new_name;
    return false;
}