#include "drivers/disk.h"
#include "fs/file.h"
#include "fs/fat32.h"
#include "fs/fs.h"
#include "fs/path.h"
#include "lib/string.h"
#include "fs/directory.h"

static bool file_resolve_destination(const disk_t *disk, const char *new_name, uint32_t *parent_cluster, char *leaf_name)
{
    if (disk == 0 || new_name == 0 || parent_cluster == 0 || leaf_name == 0)
    {
        return false;
    }

    path_t parsed;

    if (!path_split(new_name, &parsed) || !path_normalize(&parsed))
    {
        return false;
    }

    if (parsed.count == 0)
    {
        return false;
    }

    if (parsed.count == 1)
    {
        *parent_cluster = fs_current_directory();
        strcpy(leaf_name, parsed.components[0]);
        return true;
    }

    char parent_path[256];
    int pos = 0;

    if (parsed.absolute)
    {
        parent_path[pos++] = '/';
    }

    for (int i = 0; i < parsed.count - 1; i++)
    {
        if (i > 0 || parsed.absolute)
        {
            parent_path[pos++] = '/';
        }

        int component_length = strlen(parsed.components[i]);

        if (pos + component_length >= (int)sizeof(parent_path))
        {
            return false;
        }

        memcpy(parent_path + pos, parsed.components[i], component_length);
        pos += component_length;
    }

    parent_path[pos] = '\0';

    if (parent_path[0] == '\0')
    {
        strcpy(parent_path, ".");
    }

    fat32_directory_entry_t parent_entry;

    if (!path_lookup(disk, parent_path, parent_cluster, &parent_entry, 0))
    {
        return false;
    }

    strcpy(leaf_name, parsed.components[parsed.count - 1]);
    return true;
}

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

bool file_open(const disk_t *disk, uint32_t parent_cluster, const fat32_directory_entry_t *entry, file_t *file)
{
    if (disk == 0 || entry == 0 || file == 0)
    {
        return false;
    }

    file->disk = disk;
    file->parent_cluster = parent_cluster;
    file->first_cluster = directory_entry_cluster(entry);
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

    if (cluster_size == 0 || cluster_size > sizeof(cluster_buffer))
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

    uint32_t parent_cluster = 0;
    char leaf_name[PATH_MAX_NAME] = {0};
    fat32_directory_entry_t existing;

    bool exists = path_lookup(disk, name, &parent_cluster, &existing, leaf_name);

    if (exists)
    {
        return false;
    }

    if (parent_cluster < 2 || leaf_name[0] == '\0')
    {
        return false;
    }

    fat32_directory_entry_t entry;
    memset(&entry, 0, sizeof(entry));
    if (!directory_set_name(&entry, leaf_name))
    {
        return false;
    }

    entry.attributes = 0x20;

    entry.first_cluster_high = 0;
    entry.first_cluster_low = 0;
    entry.file_size = 0;

    return directory_create_entry_in_cluster(disk, parent_cluster, &entry);
}

bool file_delete(const disk_t *disk, const char *name)
{
    if (disk == 0 || name == 0)
    {
        return false;
    }

    uint32_t parent_cluster;
    char leaf_name[64];
    fat32_directory_entry_t entry;

    if (!path_lookup(disk, name, &parent_cluster, &entry, leaf_name))
    {
        return false;
    }

    if (entry.attributes & FAT32_ATTR_DIRECTORY)
    {
        return false;
    }

    uint32_t first_cluster = directory_entry_cluster(&entry);

    if (first_cluster != 0)
    {
        if (!fat32_free_cluster_chain(disk, first_cluster))
        {
            return false;
        }
    }

    return directory_delete_in_cluster(disk, parent_cluster, leaf_name);
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
            return -101;
        }

        file->first_cluster = cluster;
        file->current_cluster = cluster;
        file->entry.first_cluster_high = cluster >> 16;
        file->entry.first_cluster_low = cluster & 0xFFFF;
    }

    uint8_t cluster_buffer[4096];

    if (cluster_size > 4096)
    {
        return -102;
    }

    const uint8_t *source = (const uint8_t *)buffer;
    uint32_t bytes_written = 0;

    while (bytes_written < size)
    {
        if (!file_read_cluster(file->disk, file->current_cluster, cluster_buffer))
        {
            return (bytes_written > 0) ? (int)bytes_written : -103;
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
            return (bytes_written > 0) ? (int)bytes_written : -104;
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

    if (!directory_update_entry(file->disk, file->parent_cluster, &file->entry))
    {
        return -105;
    }

    return (int)bytes_written;
}

int file_write(file_t *file, const void *buffer, uint32_t size)
{
    if (file == 0 || buffer == 0)
    {
        return -1;
    }

    file->position = 0;
    file->size = 0;
    file->current_cluster = file->first_cluster;
    file->entry.file_size = 0;
    
    if (!directory_update_entry(file->disk, file->parent_cluster, &file->entry))
    {
        return -1;
    }

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

    uint32_t source_parent;
    fat32_directory_entry_t source_entry;

    if (!path_lookup(disk, source, &source_parent, &source_entry, 0))
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

    uint32_t destination_parent;
    fat32_directory_entry_t destination_entry;

    if (!path_lookup(disk, destination, &destination_parent, &destination_entry, 0))
    {
        return false;
    }

    file_t source_file;
    file_t destination_file;

    if (!file_open(disk, source_parent, &source_entry, &source_file))
    {
        return false;
    }

    if (!file_open(disk, destination_parent, &destination_entry, &destination_file))
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

    uint32_t source_parent = 0;
    char source_leaf[PATH_MAX_NAME] = {0};
    fat32_directory_entry_t entry;

    if (!path_lookup(disk, old_name, &source_parent, &entry, source_leaf))
    {
        return false;
    }

    uint32_t destination_parent = 0;
    char destination_leaf[PATH_MAX_NAME] = {0};
    fat32_directory_entry_t existing;

    if (!file_resolve_destination(disk, new_name, &destination_parent, destination_leaf))
    {
        return false;
    }

    if (path_lookup(disk, new_name, 0, &existing, 0))
    {
        return false;
    }

    if (source_leaf[0] == '\0' || destination_leaf[0] == '\0')
    {
        return false;
    }

    if (destination_parent < 2)
    {
        return false;
    }

    if (!directory_set_name(&entry, destination_leaf))
    {
        return false;
    }

    if (source_parent == destination_parent)
    {
        return directory_rename_entry(disk, source_parent, source_leaf, &entry);
    }

    return directory_move_entry(disk, source_parent, destination_parent, source_leaf, &entry);
}