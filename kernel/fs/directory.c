#include "fs/directory.h"
#include "fs/fat32.h"
#include "terminal.h"

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

bool directory_open_root(const disk_t *disk, directory_t *dir)
{
    if (disk == 0 || dir == 0)
    {
        return false;
    }

    dir->disk = disk;

    const fat32_filesystem_t *fs = fat32_get_filesystem();

    dir->cluster = fs->root_cluster;
    dir->current_cluster = fs->root_cluster;
    dir->index = 0;

    uint32_t sector = fat32_cluster_to_sector(dir->cluster);

    return disk_read(disk, sector, dir->sector) == 0;
}

bool directory_next(directory_t *dir, fat32_directory_entry_t *entry)
{
    while (dir->index < 16)
    {
        fat32_directory_entry_t *entries = (fat32_directory_entry_t *)dir->sector;
        fat32_directory_entry_t *current = &entries[dir->index++];

        if ((uint8_t)current->name[0] == 0x00)
        {
            return false;
        }

        if ((uint8_t)current->name[0] == 0xE5)
        {
            continue;
        }

        if (current->attributes == 0x0F)
        {
            continue;
        }

        if (current->attributes & 0x08)
        {
            continue;
        }

        *entry = *current;
        return true;
    }
    
    uint32_t next = fat32_next_cluster(dir->disk, dir->current_cluster);
    
    if (next == 0)
    {
        return false;
    }

    dir->current_cluster = next;
    dir->index = 0;

    uint32_t sector = fat32_cluster_to_sector(next);

    if (disk_read(dir->disk, sector, dir->sector) != 0)
    {
        return false;
    }

    return directory_next(dir, entry);
}