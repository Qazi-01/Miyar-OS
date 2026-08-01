#include "fs/directory.h"
#include "fs/fat32.h"
#include "fs/fs.h"
#include "fs/path.h"
#include "terminal.h"
#include "lib/string.h"

static bool directory_write_entry(const disk_t *disk, uint32_t cluster, const fat32_directory_entry_t *entry);

uint32_t directory_entry_cluster(const fat32_directory_entry_t *entry)
{
    if (entry == 0)
    {
        return 0;
    }
    
    return ((uint32_t)entry->first_cluster_high << 16) | entry->first_cluster_low;
}

void directory_set_entry_cluster(fat32_directory_entry_t *entry, uint32_t cluster)
{
    if (entry == 0)
    {
        return;
    }

    entry->first_cluster_low = cluster & 0xFFFF;
    entry->first_cluster_high = (cluster >> 16) & 0xFFFF;
}

bool directory_get_name(const fat32_directory_entry_t *entry, char *output)
{
    int pos = 0;

    for (int i = 0; i < 8; i++)
    {
        if (entry->name[i] == ' ')
        {
            break;
        }

        output[pos++] = entry->name[i];
    }

    if (entry->name[8] != ' ')
    {
        output[pos++] = '.';

        for (int i = 8; i < 11; i++)
        {
            if (entry->name[i] == ' ')
            {
                break;
            }

            output[pos++] = entry->name[i];
        }
    }

    output[pos] = '\0';
    return true;
}

bool directory_read_root(const disk_t *disk)
{
    directory_t dir;
    fat32_directory_entry_t entry;

    if (!directory_open_root(disk, &dir))
    {
        return false;
    }

    while (directory_next(&dir, &entry))
    {
        char filename[13];

        directory_get_name(&entry, filename);

        terminal_write("Entry: ");
        terminal_write(filename);
        terminal_write("\n");
    }

    return true;
}

bool directory_open(const disk_t *disk, uint32_t cluster, directory_t *dir)
{
    if (disk == 0 || dir == 0)
    {
        return false;
    }

    dir->disk = disk;
    dir->cluster = cluster;
    dir->current_cluster = cluster;
    dir->sector_index = 0;
    dir->entry_index = 0;

    uint32_t sector = fat32_cluster_to_sector(cluster);

    if (disk_read(disk, sector, dir->sector) != 0)
    {
        return false;
    }

    return true;
}

bool directory_open_root(const disk_t *disk, directory_t *dir)
{
    return directory_open(disk, fat32_get_filesystem()->root_cluster, dir);
}

#define DIRECTORY_ENTRIES_PER_SECTOR (512 / sizeof(fat32_directory_entry_t))

bool directory_next(directory_t *dir, fat32_directory_entry_t *entry)
{
    const fat32_filesystem_t *fs = fat32_get_filesystem();

    while (1)
    {
        while (dir->entry_index < DIRECTORY_ENTRIES_PER_SECTOR)
        {
            fat32_directory_entry_t *entries =
                (fat32_directory_entry_t *)dir->sector;

            fat32_directory_entry_t *current =
                &entries[dir->entry_index++];

            if ((uint8_t)current->name[0] == 0x00)
            {
                return false;
            }

            if ((uint8_t)current->name[0] == 0xE5)
            {
                continue;
            }

            if (current->attributes == FAT32_ATTR_LFN)
            {
                continue;
            }

            if (current->attributes & FAT32_ATTR_VOLUME_ID)
            {
                continue;
            }

            *entry = *current;
            return true;
        }

        dir->entry_index = 0;
        dir->sector_index++;

        if (dir->sector_index < fs->sectors_per_cluster)
        {
            uint32_t sector =
                fat32_cluster_to_sector(dir->current_cluster) +
                dir->sector_index;

            if (disk_read(dir->disk, sector, dir->sector) != 0)
            {
                return false;
            }

            continue;
        }

        uint32_t next = fat32_next_cluster(dir->disk, dir->current_cluster);

        if (next == FAT32_INVALID_CLUSTER)
        {
            return false;
        }

        dir->current_cluster = next;
        dir->sector_index = 0;
        dir->entry_index = 0;

        uint32_t sector = fat32_cluster_to_sector(next);

        if (disk_read(dir->disk, sector, dir->sector) != 0)
        {
            return false;
        }
    }
}

bool directory_create_entry_in_cluster(const disk_t *disk, uint32_t cluster, const fat32_directory_entry_t *entry)
{
    if (disk == 0 || entry == 0)
    {
        return false;
    }

    const fat32_filesystem_t *fs = fat32_get_filesystem();
    uint8_t sector[512];

    while (cluster < FAT32_CLUSTER_LAST)
    {
        uint32_t first_sector = fat32_cluster_to_sector(cluster);

        for (uint32_t s = 0; s < fs->sectors_per_cluster; s++)
        {
            if (disk_read(disk, first_sector + s, sector) != 0)
            {
                return false;
            }

            fat32_directory_entry_t *entries = (fat32_directory_entry_t *)sector;

            for (uint32_t i = 0; i < DIRECTORY_ENTRIES_PER_SECTOR; i++)
            {
                uint8_t first = (uint8_t)entries[i].name[0];

                if (first == 0x00 || first == 0xE5)
                {
                    entries[i] = *entry;

                    if (disk_write(disk, first_sector + s, sector) != 0)
                    {
                        return false;
                    }

                    return true;
                }
            }
        }

        cluster = fat32_next_cluster(disk, cluster);
    }

    return false;
}

bool directory_create_entry(const disk_t *disk, const fat32_directory_entry_t *entry)
{
    return directory_create_entry_in_cluster(disk, fs_current_directory(), entry);
}

bool directory_find_in_cluster(const disk_t *disk, uint32_t cluster, const char *name, fat32_directory_entry_t *entry)
{
    if (disk == 0 || name == 0 || entry == 0)
    {
        return false;
    }

    directory_t dir;
    fat32_directory_entry_t current;
    char filename[13];

    if (!directory_open(disk, cluster, &dir))
    {
        return false;
    }

    while (directory_next(&dir, &current))
    {
        directory_get_name(&current, filename);

        if (strcmp(filename, name) == 0)
        {
            *entry = current;
            return true;
        }
    }

    return false;
}

bool directory_find(const disk_t *disk, const char *name, fat32_directory_entry_t *entry)
{
    return directory_find_in_cluster(disk, fs_current_directory(), name, entry);
}

void directory_set_name(fat32_directory_entry_t *entry, const char *name)
{
    if (entry == 0 || name == 0)
    {
        return;
    }

    memset(entry->name, ' ', 11);
    uint32_t i = 0;

    while (*name && *name != '.' && i < 8)
    {
        char c = *name++;

        if (c >= 'a' && c <= 'z')
        {
            c -= 32;
        }

        entry->name[i++] = c;
    }

    if (*name == '.')
    {
        name++;
    }

    i = 8;

    while (*name && i < 11)
    {
        char c = *name++;

        if (c >= 'a' && c <= 'z')
        {
            c -= ('a' - 'A');
        }

        entry->name[i++] = c;
    }
}

static bool directory_write_entry(const disk_t *disk, uint32_t cluster, const fat32_directory_entry_t *entry)
{
    const fat32_filesystem_t *fs = fat32_get_filesystem();
    uint8_t sector[512];
    uint32_t first_sector = fat32_cluster_to_sector(cluster);

    for (uint32_t s = 0; s < fs->sectors_per_cluster; s++)
    {
        if (disk_read(disk, first_sector + s, sector) != 0)
        {
            return false;
        }

        fat32_directory_entry_t *entries = (fat32_directory_entry_t *)sector;

        for (uint32_t i = 0; i < DIRECTORY_ENTRIES_PER_SECTOR; i++)
        {
            uint8_t first = (uint8_t)entries[i].name[0];

            if (first == 0x00 || first == 0xE5)
            {
                entries[i] = *entry;

                return disk_write(disk, first_sector + s, sector) == 0;
            }
        }
    }

    return false;
}

bool directory_create(const disk_t *disk, const char *name)
{
    if (disk == 0 || name == 0)
    {
        return false;
    }

    uint32_t parent_cluster;
    char leaf_name[PATH_MAX_NAME];
    fat32_directory_entry_t existing;

    bool exists = path_lookup(disk, name, &parent_cluster, &existing, leaf_name);

    if (exists)
    {
        return false;
    }
    
    uint32_t cluster = fat32_allocate_cluster(disk);

    if (cluster == 0)
    {
        return false;
    }

    fat32_directory_entry_t entry;
    memset(&entry, 0, sizeof(entry));
    directory_set_name(&entry, leaf_name);

    entry.attributes = FAT32_ATTR_DIRECTORY;
    entry.first_cluster_high = cluster >> 16;
    entry.first_cluster_low = cluster & 0xFFFF;

    if (!directory_create_entry_in_cluster(disk, parent_cluster, &entry))
    {
        fat32_free_cluster_chain(disk, cluster);
        return false;
    }

    fat32_directory_entry_t dot;
    memset(&dot, 0, sizeof(dot));
    memcpy(dot.name, ".          ", 11);

    dot.attributes = FAT32_ATTR_DIRECTORY;

    fat32_directory_entry_t dotdot;
    memset(&dotdot, 0, sizeof(dotdot));
    memcpy(dotdot.name, "..         ", 11);

    dotdot.attributes = FAT32_ATTR_DIRECTORY;

    uint32_t parent = parent_cluster;

    dotdot.first_cluster_high = parent >> 16;
    dotdot.first_cluster_low = parent & 0xFFFF;

    if (!directory_write_entry(disk, cluster, &dot))
    {
        return false;
    }

    if (!directory_write_entry(disk, cluster, &dotdot))
    {
        return false;
    }

    return true;
}

bool directory_delete_in_cluster(const disk_t *disk, uint32_t cluster, const char *name)
{
    if (disk == 0 || name == 0)
    {
        return false;
    }

    const fat32_filesystem_t *fs = fat32_get_filesystem();
    uint8_t sector[512];

    while (cluster < FAT32_CLUSTER_LAST)
    {
        uint32_t first_sector = fat32_cluster_to_sector(cluster);

        for (uint32_t s = 0; s < fs->sectors_per_cluster; s++)
        {
            if (disk_read(disk, first_sector + s, sector) != 0)
            {
                return false;
            }

            fat32_directory_entry_t *entries = (fat32_directory_entry_t *)sector;

            for (uint32_t i = 0; i < DIRECTORY_ENTRIES_PER_SECTOR; i++)
            {
                uint8_t first = (uint8_t)entries[i].name[0];

                if (first == 0x00)
                {
                    return false;
                }

                if (first == 0xE5)
                {
                    continue;
                }

                if (entries[i].attributes == FAT32_ATTR_LFN)
                {
                    continue;
                }

                if (entries[i].attributes == FAT32_ATTR_VOLUME_ID)
                {
                    continue;
                }

                char filename[13];
                directory_get_name(&entries[i], filename);

                if (strcmp(filename, name) == 0)
                {
                    entries[i].name[0] = (char)0xE5;
                    return disk_write(disk, first_sector + s, sector) == 0;
                }
            }
        }

        cluster = fat32_next_cluster(disk, cluster);
    }

    return false;
}

bool directory_delete(const disk_t *disk, const char *name)
{
    return directory_delete_in_cluster(disk, fs_current_directory(), name);
}

bool directory_is_empty(const disk_t *disk, const fat32_directory_entry_t *entry)
{
    if (disk == 0 || entry == 0 )
    {
        return false;
    }

    const fat32_filesystem_t *fs = fat32_get_filesystem();

    uint32_t cluster = ((uint32_t)entry->first_cluster_high << 16) | entry->first_cluster_low;
    uint8_t sector[512];

    while (cluster < FAT32_CLUSTER_LAST)
    {
        uint32_t first_sector = fat32_cluster_to_sector(cluster);

        for (uint32_t s = 0; s < fs->sectors_per_cluster; s++)
        {
            if (disk_read(disk, first_sector + s, sector) != 0)
            {
                return false;
            }

            fat32_directory_entry_t *entries = (fat32_directory_entry_t *)sector;

            for (uint32_t i = 0; i < DIRECTORY_ENTRIES_PER_SECTOR; i++)
            {
                uint8_t first = (uint8_t)entries[i].name[0];

                if (first == 0x00)
                {
                    return true;
                }

                if (first == 0xE5)
                {
                    continue;
                }

                if (entries[i].attributes == FAT32_ATTR_LFN)
                {
                    continue;
                }

                if (entries[i].attributes == FAT32_ATTR_VOLUME_ID)
                {
                    continue;
                }

                char filename[13];
                directory_get_name(&entries[i], filename);

                if (strcmp(filename, ".") == 0)
                {
                    continue;
                }

                if (strcmp(filename, "..") == 0)
                {
                    continue;
                }

                return false;
            }
        }

        cluster = fat32_next_cluster(disk, cluster);
    }

    return true;
}

bool directory_remove(const disk_t *disk, const char *name)
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

    if (!(entry.attributes & FAT32_ATTR_DIRECTORY))
    {
        return false;
    }

    if (!directory_is_empty(disk, &entry))
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

bool directory_update_entry(const disk_t *disk, uint32_t parent_cluster, const fat32_directory_entry_t *entry)
{
    if (disk == 0 || entry == 0)
    {
        return false;
    }

    const fat32_filesystem_t *fs = fat32_get_filesystem();
    uint32_t cluster = parent_cluster;
    uint8_t sector[512];

    while (cluster < FAT32_CLUSTER_LAST)
    {
        uint32_t first_sector = fat32_cluster_to_sector(cluster);

        for (uint32_t s = 0; s < fs->sectors_per_cluster; s++)
        {
            if (disk_read(disk, first_sector + s, sector) != 0)
            {
                return false;
            }

            fat32_directory_entry_t *entries = (fat32_directory_entry_t *)sector;

            for (uint32_t i = 0; i < DIRECTORY_ENTRIES_PER_SECTOR; i++)
            {
                uint8_t first = (uint8_t)entries[i].name[0];

                if (first == 0x00)
                {
                    return false;
                }

                if (first == 0xE5)
                {
                    continue;
                }

                if (entries[i].attributes == FAT32_ATTR_LFN)
                {
                    continue;
                }

                if (entries[i].attributes == FAT32_ATTR_VOLUME_ID)
                {
                    continue;
                }

                if (memcmp(entries[i].name, entry->name, 11) == 0)
                {
                    entries[i] = *entry;
                    return disk_write(disk, first_sector + s, sector) == 0;
                }
            }
        }

        cluster = fat32_next_cluster(disk, cluster);
    }

    return false;
}

bool directory_rename_entry(const disk_t *disk, uint32_t parent_cluster, const char *old_name, const fat32_directory_entry_t *entry)
{
    if (disk == 0 || entry == 0)
    {
        return false;
    }

    const fat32_filesystem_t *fs = fat32_get_filesystem();
    uint32_t cluster = parent_cluster;
    uint8_t sector[512];

    while (cluster < FAT32_CLUSTER_LAST)
    {
        uint32_t first_sector = fat32_cluster_to_sector(cluster);

        for (uint32_t s = 0; s < fs->sectors_per_cluster; s++)
        {
            if (disk_read(disk, first_sector + s, sector) != 0)
            {
                return false;
            }

            fat32_directory_entry_t *entries = (fat32_directory_entry_t *)sector;

            for (uint32_t i = 0; i < DIRECTORY_ENTRIES_PER_SECTOR; i++)
            {
                uint8_t first = (uint8_t)entries[i].name[0];

                if (first == 0x00)
                {
                    return false;
                }

                if (first == 0xE5)
                {
                    continue;
                }

                if (entries[i].attributes == FAT32_ATTR_LFN)
                {
                    continue;
                }

                if (entries[i].attributes == FAT32_ATTR_VOLUME_ID)
                {
                    continue;
                }

                char filename[13];
                directory_get_name(&entries[i], filename);
                
                if (strcmp(filename, old_name) == 0)
                {
                    entries[i] = *entry;
                    return disk_write(disk, first_sector + s, sector) == 0;
                }
            }
        }

        cluster = fat32_next_cluster(disk, cluster);
    }

    return false;
}

bool directory_rename(const disk_t *disk, const char *old_name, const char *new_name)
{
    if (disk == 0 || old_name == 0 || new_name == 0)
    {
        return false;
    }

    fat32_directory_entry_t entry;

    if (!path_resolve(disk, old_name, &entry))
    {
        return false;
    }

    fat32_directory_entry_t existing;

    if (path_resolve(disk, new_name, &existing))
    {
        return false;
    }

    directory_set_name(&entry, new_name);

    return directory_rename_entry(disk, fs_current_directory(), old_name, &entry);
}

bool directory_change(const disk_t *disk, const char *name)
{
    if (disk == 0 || name == 0)
    {
        return false;
    }

    if (strcmp(name, "..") == 0 && strcmp(fs_get_current_path(), "/") == 0)
    {
        return true;
    }

    fat32_directory_entry_t entry;

    if (!directory_find_in_cluster(disk, fs_current_directory(), name, &entry))
    {
        return false;
    }

    if (!(entry.attributes & FAT32_ATTR_DIRECTORY))
    {
        return false;
    }

    uint32_t cluster = ((uint32_t)entry.first_cluster_high << 16) | entry.first_cluster_low;
    char path[256];

    if (strcmp(name, "..") == 0)
    {
        strcpy(path, fs_get_current_path());

        if (strcmp(path, "/") != 0)
        {
            char *last = strrchr(path, '/');

            if (last == path)
            {
                path[1] = '\0';
            }

            else if (last != 0)
            {
                *last = '\0';
            }
        }

        return fs_set_current_directory(cluster, path);
    }

    if (strcmp(fs_get_current_path(), "/") == 0)
    {
        strcpy(path, "/");
        strcat(path, name);
    }

    else
    {
        strcpy(path, fs_get_current_path());
        strcat(path, "/");
        strcat(path, name);
    }

    return fs_set_current_directory(cluster, path);
}

bool directory_move_entry(const disk_t *disk, uint32_t source_cluster, uint32_t destination_cluster, const char *old_name, fat32_directory_entry_t *entry)
{
    if (disk == 0 || old_name == 0 || entry == 0)
    {
        return false;
    }

    if (!directory_create_entry_in_cluster(disk, destination_cluster, entry))
    {
        return false;
    }

    if (!directory_delete_in_cluster(disk, source_cluster, old_name))
    {
        char filename[13];
        directory_get_name(entry, filename);
        directory_delete_in_cluster(disk, destination_cluster, filename);
        return false;
    }

    return true;
}