#include "fs/fat32.h"
#include "lib/string.h"

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

    fat32_boot_sector_t *boot = (fat32_boot_sector_t *)sector;

    if (boot->boot_signature != 0x29)
    {
        return false;
    }

    if (memcmp(boot->fs_type, "FAT32   ", 8) != 0)
    {
        return false;
    }
    
    fat32_fs.bytes_per_sector = boot->bytes_per_sector;
    fat32_fs.sectors_per_cluster = boot->sectors_per_cluster;
    fat32_fs.reserved_sector_count = boot->reserved_sector_count;
    fat32_fs.fat_count = boot->fat_count;
    fat32_fs.fat_size = boot->fat_size32;
    fat32_fs.root_cluster = boot->root_cluster;
    fat32_fs.first_data_sector = boot->reserved_sector_count + (boot->fat_count * boot->fat_size32);
    fat32_fs.fat_start_sector = boot->reserved_sector_count;
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
    return fat32_fs.first_data_sector + ((cluster - 2) * fat32_fs.sectors_per_cluster);
}

uint32_t fat32_read_fat_entry(const disk_t *disk, uint32_t cluster)
{
    uint8_t sector[512];

    uint32_t fat_sector = fat32_fs.reserved_sector_count + ((cluster * 4) / fat32_fs.bytes_per_sector);

    if (disk_read(disk, fat_sector, sector) != 0)
    {
        return 0xFFFFFFFF;
    }

    uint32_t offset = (cluster * 4) % fat32_fs.bytes_per_sector;
    uint32_t value = *(uint32_t *)(sector + offset);

    return value & 0xFFFFFFFF;
}

uint32_t fat32_next_cluster(const disk_t *disk, uint32_t cluster)
{
    uint32_t next = fat32_read_fat_entry(disk, cluster);

    if (next >= 0x0FFFFFF8)
    {
        return 0xFFFFFFFF;
    }

    return next;
}

uint32_t fat32_find_free_cluster(void)
{
    const disk_t *disk = disk_get(0);

    if (disk == 0)
    {
        return 0;
    }

    for (uint32_t cluster = 2; cluster < fat32_fs.total_clusters + 2; cluster++)
    {
        uint32_t value = fat32_read_fat_entry(disk, cluster);

        if (value == 0)
        {
            return cluster;
        }
    }

    return 0;
}

bool fat32_write_fat_entry(const disk_t *disk, uint32_t cluster, uint32_t value)
{
    uint8_t sector[512];

    uint32_t fat_sector = fat32_fs.fat_start_sector + ((cluster * 4) / fat32_fs.bytes_per_sector);

    if (disk_read(disk, fat_sector, sector) != 0)
    {
        return false;
    }

    uint32_t offset = (cluster * 4) % fat32_fs.bytes_per_sector;
    *(uint32_t *)(sector + offset) = value;

    if (disk_write(disk, fat_sector, sector) != 0)
    {
        return false;
    }

    return true;
}