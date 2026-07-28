#include "drivers/disk.h"
#include "fs/file.h"
#include "fs/fat32.h"
#include "lib/string.h"
#include "fs/directory.h"

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

    if (file->position >= file->size)
    {
        return 0;
    }

    uint32_t remaining = file->size - file->position;

    if (size > remaining)
    {
        size = remaining;
    }

    const fat32_filesystem_t *fs = fat32_get_filesystem();
    uint32_t cluster_size = fs->bytes_per_sector * fs->sectors_per_cluster;
    uint8_t cluster_buffer[4096];
    uint8_t *destination = (uint8_t *)buffer;
    uint32_t bytes_read = 0;

    while (bytes_read < size)
    {
        if (!fat32_read_cluster(file->disk, file->current_cluster, cluster_buffer))
        {
            return (bytes_read > 0) ? (int)bytes_read : -1;
        }

        uint32_t offset = file->position % cluster_size;
        uint32_t available = cluster_size - offset;
        uint32_t to_copy = size - bytes_read;

        if (to_copy > available)
        {
            to_copy = available;
        }

        memcpy(destination + bytes_read, cluster_buffer + offset, to_copy);
        bytes_read += to_copy;
        file->position += to_copy;

        if ((file->position % cluster_size) == 0 && bytes_read < size)
        {
            uint32_t next = fat32_next_cluster(file->disk, file->current_cluster);

            if (next >= FAT32_CLUSTER_LAST)
            {
                break;
            }

            file->current_cluster = next;
        }
    }

    return (int)bytes_read;
}

bool file_create(const disk_t *disk, const char *name)
{
    if (disk == 0 || name == 0)
    {
        return false;
    }

    fat32_directory_entry_t existing;

    if (directory_find(disk, name, &existing))
    {
        return false;
    }

    fat32_directory_entry_t entry;
    memset(&entry, 0, sizeof(entry));
    directory_set_name(&entry, name);

    entry.attributes = 0x20;

    entry.first_cluster_high = 0;
    entry.first_cluster_low = 0;
    entry.file_size = 0;

    return directory_create_entry(disk, &entry);
}

bool file_delete(const disk_t *disk, const char *name)
{
    if (disk == 0 || name == 0)
    {
        return false;
    }

    fat32_directory_entry_t entry;

    if (!directory_find(disk, name, &entry))
    {
        return false;
    }

    if (entry.attributes & FAT32_ATTR_DIRECTORY)
    {
        return false;
    }

    uint32_t first_cluster = ((uint32_t)entry.first_cluster_high << 16) | entry.first_cluster_low;

    if (first_cluster != 0)
    {
        if (!fat32_free_cluster_chain(disk, first_cluster))
        {
            return false;
        }
    }

    return directory_delete(disk, name);
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