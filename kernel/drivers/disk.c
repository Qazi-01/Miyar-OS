#include "drivers/ata.h"
#include "drivers/disk.h"

static disk_t disks[4];

void disk_init(void)
{
    for (int i = 0; i < 4; i++)
    {
        disks[i].device = ata_get_device(i);
    }
}

const disk_t *disk_get(uint8_t index)
{
    if (index >= 4)
    {
        return 0;
    }

    if (disks[index].device == 0)
    {
        return 0;
    }

    if (!disks[index].device->present)
    {
        return 0;
    }

    return &disks[index];
}

int disk_read(const disk_t *disk, uint32_t lba, uint8_t *buffer)
{
    if (disk == 0)
    {
        return -1;
    }

    return ata_read_sector(disk->device, lba, buffer);
}

int disk_write(const disk_t *disk, uint32_t lba, const uint8_t *buffer)
{
    if (disk == 0)
    {
        return -1;
    }

    return ata_write_sector(disk->device, lba, buffer);
}