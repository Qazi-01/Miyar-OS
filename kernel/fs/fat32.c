#include "fs/fat32.h"
#include "memory/heap.h"
#include "lib/string.h"

#define FAT32_INVALID_CLUSTER 0xFFFFFFFF
#define FAT32_CLUSTER_FREE    0x00000000
#define FAT32_CLUSTER_EOC     0x0FFFFFFF
#define FAT32_CLUSTER_LAST    0x0FFFFFF8

static fat32_filesystem_t fat32_fs;

bool fat32_detect(const disk_t *disk)
{
    if (disk == 0)
    {
        return false;
    }

    uint8_t sector[512];

    if (disk_read(disk, 0, sector) != 0)
    {
        return false;
    }

    const fat32_boot_sector_t *boot = (const fat32_boot_sector_t *)sector;

    if (boot->boot_signature != 0x28 &&
        boot->boot_signature != 0x29)
    {
        return false;
    }

    if (memcmp(boot->fs_type, "FAT32   ", 8) != 0)
    {
        return false;
    }

    if (boot->bytes_per_sector != 512)
    {
        return false;
    }

    if (boot->sectors_per_cluster == 0)
    {
        return false;
    }

    if (boot->fat_count == 0)
    {
        return false;
    }

    if (boot->fat_size32 == 0)
    {
        return false;
    }

    fat32_fs.bytes_per_sector = boot->bytes_per_sector;
    fat32_fs.sectors_per_cluster = boot->sectors_per_cluster;
    fat32_fs.reserved_sector_count = boot->reserved_sector_count;
    fat32_fs.fat_count = boot->fat_count;
    fat32_fs.fat_size = boot->fat_size32;
    fat32_fs.root_cluster = boot->root_cluster;
    fat32_fs.first_data_sector = fat32_fs.reserved_sector_count + (fat32_fs.fat_count * fat32_fs.fat_size);
    fat32_fs.fat_start_sector = fat32_fs.reserved_sector_count;
    uint32_t data_sectors = boot->total_sectors32 - fat32_fs.first_data_sector;
    fat32_fs.total_clusters = data_sectors / boot->sectors_per_cluster;

    return true;
}

const fat32_filesystem_t *fat32_get_filesystem(void)
{
    return &fat32_fs;
}

uint32_t fat32_cluster_to_sector(uint32_t cluster)
{
    if (cluster < 2)
    {
        return 0;
    }

    return fat32_fs.first_data_sector +
           ((cluster - 2) * fat32_fs.sectors_per_cluster);
}

uint32_t fat32_read_fat_entry(const disk_t *disk, uint32_t cluster)
{
    if (disk == 0)
{
        return FAT32_INVALID_CLUSTER;
    }

    if (cluster < 2 || cluster >= fat32_fs.total_clusters + 2)
    {
        return FAT32_INVALID_CLUSTER;
    }

    uint8_t sector[512];

    uint32_t fat_sector = fat32_fs.fat_start_sector + ((cluster * 4) / fat32_fs.bytes_per_sector);

    if (disk_read(disk, fat_sector, sector) != 0)
    {
        return FAT32_INVALID_CLUSTER;
    }

    uint32_t offset = (cluster * 4) % fat32_fs.bytes_per_sector;
    uint32_t value = *(uint32_t *)(sector + offset);

    return value & 0x0FFFFFFF;
}

uint32_t fat32_next_cluster(const disk_t *disk, uint32_t cluster)
{
    if (disk == 0)
    {
        return FAT32_INVALID_CLUSTER;
    }

    uint32_t next = fat32_read_fat_entry(disk, cluster);

    if (next >= FAT32_CLUSTER_LAST)
    {
        return FAT32_INVALID_CLUSTER;
    }

    return next;
}

uint32_t fat32_find_free_cluster(const disk_t *disk)
{
    if (disk == 0)
    {
        return 0;
    }

    for (uint32_t cluster = 2; cluster < fat32_fs.total_clusters + 2; cluster++)
    {
        if (fat32_read_fat_entry(disk, cluster) == FAT32_CLUSTER_FREE)
        {
            return cluster;
        }
    }

    return 0;
}

bool fat32_write_fat_entry(const disk_t *disk, uint32_t cluster, uint32_t value)
{
    if (disk == 0)
    {
        return false;
    }

    if (cluster < 2 || cluster >= fat32_fs.total_clusters + 2)
    {
        return false;
    }

    uint8_t sector[512];

    uint32_t fat_sector = fat32_fs.fat_start_sector + ((cluster * 4) / fat32_fs.bytes_per_sector);

    if (disk_read(disk, fat_sector, sector) != 0)
    {
        return false;
    }

    uint32_t offset = (cluster * 4) % fat32_fs.bytes_per_sector;
    uint32_t old = *(uint32_t *)(sector + offset);
    *(uint32_t *)(sector + offset) = (old & 0xF0000000) | (value & 0x0FFFFFFF);

    if (disk_write(disk, fat_sector, sector) != 0)
    {
        return false;
    }

    return true;
}

uint32_t fat32_allocate_cluster(const disk_t *disk)
{
    uint32_t cluster = fat32_find_free_cluster(disk);

    if (cluster == 0)
    {
        return 0;
    }

    if (!fat32_write_fat_entry(disk, cluster, FAT32_CLUSTER_EOC))
    {
        return 0;
    }

    if (!fat32_zero_cluster(disk, cluster))
    {
        fat32_write_fat_entry(disk, cluster, FAT32_CLUSTER_FREE);
        return 0;
    }

    return cluster;
}

uint32_t fat32_allocate_cluster_chain(const disk_t *disk, uint32_t cluster_count)
{
    if (cluster_count == 0)
    {
        return 0;
    }

    uint32_t *allocated = kmalloc(cluster_count * sizeof(uint32_t));

    if (allocated == 0)
    {
        return 0;
    }

    uint32_t allocated_count = 0;
    uint32_t first_cluster = 0;
    uint32_t previous_cluster = 0;

    for (uint32_t i = 0; i < cluster_count; i++)
    {
        uint32_t new_cluster = fat32_allocate_cluster(disk);

        if (new_cluster == 0)
        {
            goto rollback;
        }

        allocated[allocated_count++] = new_cluster;

        if (first_cluster == 0)
        {
            first_cluster = new_cluster;
        }

        if (previous_cluster != 0)
        {
            if (!fat32_write_fat_entry(disk, previous_cluster, new_cluster))
            {
                goto rollback;
            }
        }

        previous_cluster = new_cluster;
    }

    kfree(allocated);
    return first_cluster;


rollback:

    for (uint32_t i = 0; i < allocated_count; i++)
    {
        fat32_write_fat_entry(disk, allocated[i], FAT32_CLUSTER_FREE);
    }

    kfree(allocated);
    return 0;
}

bool fat32_free_cluster_chain(const disk_t *disk, uint32_t first_cluster)
{
    uint32_t cluster = first_cluster;
    uint32_t freed = 0;

    while (cluster >= 2 && cluster < fat32_fs.total_clusters + 2)
    {
        if (++freed > fat32_fs.total_clusters)
        {
            return false;
        }

        uint32_t next = fat32_read_fat_entry(disk, cluster);

        if (next == FAT32_INVALID_CLUSTER)
        {
            return false;
        }

        if (!fat32_write_fat_entry(disk, cluster, FAT32_CLUSTER_FREE))
        {
            return false;
        }

        if (next >= FAT32_CLUSTER_LAST)
        {
            break;
        }

        cluster = next;
    }

    return true;
}

bool fat32_zero_cluster(const disk_t *disk, uint32_t cluster)
{
    if (disk == 0)
    {
        return false;
    }

    uint8_t buffer[512] = {0};

    uint32_t first_sector = fat32_cluster_to_sector(cluster);

    if (first_sector == 0)
    {
        return false;
    }

    for (uint32_t i = 0; i < fat32_fs.sectors_per_cluster; i++)
    {
        if (disk_write(disk, first_sector + i, buffer) != 0)
        {
            return false;
        }
    }

    return true;
}