#include "fs/directory.h"
#include "fs/path.h"
#include "lib/string.h"

bool path_parse(const char *path, path_t *result)
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

        result->components[result->count][length] = '\0';

        if (result->count >= PATH_MAX_COMPONENTS)
        {
            return false;
        }

        if (*path == '/')
        {
            path++;
        }
    }

    return true;
}

bool path_resolve_root(const disk_t *disk, const char *name, fat32_directory_entry_t *entry)
{
    directory_t dir;

    if (!directory_open_root(disk, &dir))
    {
        return false;
    }

    while (directory_next(&dir, entry))
    {
        char filename[13];
        directory_get_name(entry, filename);

        if (strcmp(filename, name) == 0)
        {
            return true;
        }
    }
    
    return false;
}