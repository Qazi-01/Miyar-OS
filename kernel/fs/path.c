#include "fs/directory.h"
#include "fs/fs.h"
#include "fs/fat32.h"
#include "fs/path.h"
#include "lib/string.h"

bool path_split(const char *path, path_t *result)
{
    if (path == 0 || result == 0)
    {
        return false;
    }

    result->count = 0;
    result->absolute = false;

    if (*path == '/')
    {
        result->absolute = true;
        path++;
    }

    while (*path)
    {
        int length = 0;

        while (*path && *path != '/')
        {
            if (length >= PATH_MAX_NAME - 1)
            {
                return false;
            }

            result->components[result->count][length++] = *path++;
        }

        if (result->count >= PATH_MAX_COMPONENTS)
        {
            return false;
        }

        result->components[result->count][length] = '\0';
        result->count++;

        if (*path == '/')
        {
            path++;
        }
    }

    return true;
}

bool path_normalize(path_t *path)
{
    if (path == 0)
    {
        return false;
    }

    int write = 0;

    for (int read = 0; read < path->count; read++)
    {
        char *component = path->components[read];

        if (strcmp(component, ".") == 0)
        {
            continue;
        }

        if (strcmp(component, "..") == 0)
        {
            if (write > 0 && strcmp(path->components[write - 1], "..") != 0)
            {
                write--;
            }

            else if (!path->absolute)
            {
                if (write != read)
                {
                    strcpy(path->components[write], component);
                }

                write++;
            }

            continue;
        }

        if (write != read)
        {
            strcpy(path->components[write], component);
        }

        write++;
    }

    path->count = write;
    return true;
}

bool path_lookup(const disk_t *disk, const char *path, uint32_t *parent_cluster, fat32_directory_entry_t *entry, char *leaf_name)
{
    if (disk == 0 || path == 0 || entry == 0)
    {
        return false;
    }

    path_t parsed;

    if (!path_split(path, &parsed))
    {
        return false;
    }

    if (!path_normalize(&parsed))
    {
        return false;
    }

    if (parsed.count == 0)
    {
        return false;
    }

    if (leaf_name != 0)
    {
        strcpy(leaf_name, parsed.components[parsed.count - 1]);
    }

    const fat32_filesystem_t *fs = fat32_get_filesystem();
    uint32_t cluster = parsed.absolute ? fs->root_cluster : fs_current_directory();
    fat32_directory_entry_t current;

    for (int i = 0; i < parsed.count; i++)
    {
        if (i == parsed.count - 1 && parent_cluster != 0)
        {
            *parent_cluster = cluster;
        }

        if (!directory_find_in_cluster(disk, cluster, parsed.components[i], &current))
        {
            return false;
        }

        if (i != parsed.count - 1)
        {
            if (!(current.attributes & FAT32_ATTR_DIRECTORY))
            {
                return false;
            }

            cluster = directory_entry_cluster(&current);
        }
    }

    *entry = current;
    return true;
}

bool path_resolve(const disk_t *disk, const char *path, fat32_directory_entry_t *entry)
{
    return path_lookup(disk, path, 0, entry, 0);
}