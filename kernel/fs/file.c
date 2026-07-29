#include "drivers/disk.h"
#include "fs/file.h"
#include "fs/fat32.h"
#include "lib/string.h"
#include "fs/directory.h"

static bool file_read_cluster(const disk_t *disk, uint32_t cluster, void *buffer)
{
    if (disk == 0 || buffer == 0)
    {
        return false;
    }

    const fat32_filesystem_t *fs = fat32_get_filesystem();
    uint32_t first_sector = fat32_cluster_to_sector(cluster);

    for (uint32_t i = 0; i < fs->sectors_per_cluster; i++)
    {
        if (disk_read(disk, first_sector + i, (uint8_t *)buffer + (i * fs->bytes_per_sector)) != 0)
        {
            return false;
        }
    }

    return true;
}

static bool file_write_cluster(const disk_t *disk, uint32_t cluster, const void *buffer)
{
    if (disk == 0 || buffer == 0)
    {
        return false;
    }

    const fat32_filesystem_t *fs = fat32_get_filesystem();
    uint32_t first_sector = fat32_cluster_to_sector(cluster);

    for (uint32_t i = 0; i < fs->sectors_per_cluster; i++)
    {
        if (disk_write(disk, first_sector + i, (const uint8_t *)buffer + (i * fs->bytes_per_sector)) != 0)
        {
            return false;
        }
    }

    return true;
}

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
    file->entry = *entry;

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

    if (cluster_size > 4096)
    {
        return -1;
    }

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

            if (next >= FAT32_INVALID_CLUSTER)
            {
                return (int)bytes_read;
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

static int file_write_internal(file_t *file, const void *buffer, uint32_t size)
{
    if (file == 0 || buffer == 0)
    {
        return -1;
    }

    const fat32_filesystem_t *fs = fat32_get_filesystem();
    uint32_t cluster_size = fs->bytes_per_sector * fs->sectors_per_cluster;

    if (file->first_cluster == 0)
    {
        uint32_t cluster = fat32_allocate_cluster(file->disk);

        if (cluster == 0)
        {
            return -1;
        }

        file->first_cluster = cluster;
        file->current_cluster = cluster;
        file->entry.first_cluster_high = cluster >> 16;
        file->entry.first_cluster_low = cluster & 0xFFFF;
    }

    uint8_t cluster_buffer[4096];

    if (cluster_size > 4096)
    {
        return -1;
    }

    const uint8_t *source = (const uint8_t *)buffer;
    uint32_t bytes_written = 0;

    while (bytes_written < size)
    {
        if (!file_read_cluster(file->disk, file->current_cluster, cluster_buffer))
        {
            return (bytes_written > 0) ? (int)bytes_written : -1;
        }

        uint32_t offset = file->position % cluster_size;
        uint32_t available = cluster_size - offset;
        uint32_t to_copy = size - bytes_written;

        if (to_copy > available)
        {
            to_copy = available;
        }

        memcpy(cluster_buffer + offset, source + bytes_written, to_copy);

        if (!file_write_cluster(file->disk, file->current_cluster, cluster_buffer))
        {
            return (bytes_written > 0) ? (int)bytes_written : -1;
        }

        bytes_written += to_copy;
        file->position += to_copy;

        if (bytes_written == size)
        {
            break;
        }

        uint32_t next = fat32_next_cluster(file->disk, file->current_cluster);

        if (next == FAT32_INVALID_CLUSTER)
        {
            next = fat32_allocate_cluster(file->disk);

            if (next == 0)
            {
                return (int)bytes_written;
            }

            if (!fat32_append_cluster(file->disk, file->current_cluster, next))
            {
                return(int)bytes_written;
            }
        }

        file->current_cluster = next;
    }

    if (file->position > file->size)
    {
        file->size = file->position;
    }

    file->entry.file_size = file->size;

    if (!directory_update_entry(file->disk, &file->entry))
    {
        return -1;
    }

    return (int)bytes_written;
}

int file_write(file_t *file, const void *buffer, uint32_t size)
{
    if (file == 0)
    {
        return -1;
    }

    file->position = 0;
    file->current_cluster = file->first_cluster;

    return file_write_internal(file, buffer, size);
}

int file_append(file_t *file, const void *buffer, uint32_t size)
{
    if (file == 0 || buffer == 0)
    {
        return -1;
    }

    file->position = file->size;
    file->current_cluster = file->first_cluster;

    while (1)
    {
        uint32_t next = fat32_next_cluster(file->disk, file->current_cluster);

        if (next == FAT32_INVALID_CLUSTER)
        {
            break;
        }

        file->current_cluster = next;
    }

    return file_write_internal(file, buffer, size);
}

bool file_copy(const disk_t *disk, const char *source, const char *destination)
{
    if (disk == 0 || source == 0 || destination == 0)
    {
        return false;
    }

    fat32_directory_entry_t source_entry;

    if (!directory_find(disk, source, &source_entry))
    {
        return false;
    }

    if (source_entry.attributes & FAT32_ATTR_DIRECTORY)
    {
        return false;
    }

    if (!file_create(disk, destination))
    {
        return false;
    }

    fat32_directory_entry_t destination_entry;

    if (!directory_find(disk, destination, &destination_entry))
    {
        return false;
    }

    file_t source_file;
    file_t destination_file;

    if (!file_open(disk, &source_entry, &source_file))
    {
        return false;
    }

    if (!file_open(disk, &destination_entry, &destination_file))
    {
        return false;
    }

    const fat32_filesystem_t *fs = fat32_get_filesystem();
    uint32_t cluster_size = fs->bytes_per_sector * fs->sectors_per_cluster;

    uint8_t buffer[4096];

    if (cluster_size > sizeof(buffer))
    {
        return false;
    }

    int bytes;

    while ((bytes = file_read(&source_file, buffer, cluster_size)) > 0)
    {
        if (file_append(&destination_file, buffer, (uint32_t)bytes) != bytes)
        {
            return false;
        }
    }

    return bytes >= 0;
}

bool file_move(const disk_t *disk, const char *old_name, const char *new_name)
{
    if (disk == 0 || old_name == 0 || new_name == 0)
    {
        return false;
    }

    fat32_directory_entry_t entry;

    if (directory_find(disk, new_name, &entry))
    {
        return false;
    }

    return directory_rename(disk, old_name, new_name);
}