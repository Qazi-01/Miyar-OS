#include "fs/fat32.h"
#include "lib/string.h"

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

    terminal_write("Boot signature: ");
    terminal_write_hex(boot->boot_signature);
    terminal_write("\n");

    terminal_write("FS type: ");
    for (int i = 0; i < 8; i++)
    {
        terminal_write((char[]){boot->fs_type[i], '\0'});
    }
    terminal_write("\n");

    if (boot->boot_signature != 0x29)
    {
        return false;
    }

    if (memcmp(boot->fs_type, "FAT32  ", 8) != 0)
    {
        return false;
    }
    
    return true;
}