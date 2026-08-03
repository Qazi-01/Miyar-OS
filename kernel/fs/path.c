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
        if (result->count >= PATH_MAX_COMPONENTS)
        {
            return false;
        }

        int length = 0;

        while (*path && *path != '/')
        {
            if (length >= PATH_MAX_NAME - 1)
            {
                return false;
            }

            result->components[result->count][length++] = *path++;
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

bool path_build_absolute(const char *current_path, const char *input, char *output)
{
    if (current_path == 0 || input == 0 || output == 0)
    {
        return false;
    }

    path_t path;

    if (input[0] == '/')
    {
        if (!path_split(input, &path))
        {
            return false;
        }
    }

    else
    {
        path_t base_path;
        path_t input_path;

        if (!path_split(current_path, &base_path) || !path_split(input, &input_path))
        {
            return false;
        }

        if (base_path.count + input_path.count > PATH_MAX_COMPONENTS)
        {
            return false;
        }

        path.absolute = base_path.absolute;
        path.count = 0;

        for (int i = 0; i < base_path.count; i++)
        {
            strcpy(path.components[path.count++], base_path.components[i]);
        }

        for (int i = 0; i < input_path.count; i++)
        {
            strcpy(path.components[path.count++], input_path.components[i]);
        }
    }

    if (!path_normalize(&path))
    {
        return false;
    }

    const int output_capacity = 256;
    int output_length = 1;
    output[0] = '/';
    output[1] = '\0';

    for (int i = 0; i < path.count; i++)
    {
        int separator_length = (i != 0) ? 1 : 0;
        int component_length = strlen(path.components[i]);

        if (output_length + separator_length + component_length >= output_capacity)
        {
            return false;
        }

        if (i != 0)
        {
            output[output_length++] = '/';
        }

        memcpy(output + output_length, path.components[i], component_length + 1);
        output_length += component_length;
    }

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

    const fat32_filesystem_t *fs = fat32_get_filesystem();

    if (!parsed.absolute && parsed.count == 1 && strcmp(parsed.components[0], "..") == 0 && fs_current_directory() == fs->root_cluster)
    {
        memset(entry, 0, sizeof(*entry));
        entry->attributes = FAT32_ATTR_DIRECTORY;
        directory_set_entry_cluster(entry, fs->root_cluster);

        if (parent_cluster != 0)
        {
            *parent_cluster = fs->root_cluster;
        }

        if (leaf_name != 0)
        {
            strcpy(leaf_name, "..");
        }

        return true;
    }

    if (parsed.count == 0)
    {
        if (!parsed.absolute)
        {
            return false;
        }
        
        if (parent_cluster != 0)
        {
            *parent_cluster = fs->root_cluster;
        }

        if (leaf_name != 0)
        {
            leaf_name[0] = '\0';
        }

        
        memset(entry, 0, sizeof(*entry));
        entry->attributes = FAT32_ATTR_DIRECTORY;
        directory_set_entry_cluster(entry, fs->root_cluster);

        return true;
    }

    if (leaf_name != 0)
    {
        strcpy(leaf_name, parsed.components[parsed.count - 1]);
    }

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