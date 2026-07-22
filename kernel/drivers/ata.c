#include "ata.h"
#include "io.h"
#include "terminal.h"
#include "serial.h"

#define ATA_PRIMARY_IO             0x1F0
#define ATA_SECONDARY_IO           0x170

#define ATA_PRIMARY_CTRL           0x3F6
#define ATA_SECONDARY_CTRL         0x376

#define ATA_REG_DATA               0
#define ATA_REG_ERROR              1
#define ATA_REG_FEATURES           1
#define ATA_REG_SECTOR_COUNT       2
#define ATA_REG_LBA_LOW            3
#define ATA_REG_LBA_MID            4
#define ATA_REG_LBA_HIGH           5
#define ATA_REG_HDDEVSEL           6
#define ATA_REG_COMMAND            7
#define ATA_REG_STATUS             7

#define ATA_CMD_IDENTIFY           0xEC
#define ATA_CMD_READ               0x20
#define ATA_CMD_WRITE              0x30

#define ATA_SR_ERR                 0x01
#define ATA_SR_DRQ                 0x08
#define ATA_SR_DF                  0x20
#define ATA_SR_DRDY                0x40
#define ATA_SR_BSY                 0x80

static ata_device_t ata_devices[4];

static uint16_t ata_base_port(ata_channel_t channel)
{
    if (channel == ATA_PRIMARY)
    {
        return ATA_PRIMARY_IO;
    }

    return ATA_SECONDARY_IO;
}

static uint8_t ata_read_status(ata_channel_t channel)
{
    return inb(ata_base_port(channel) + ATA_REG_STATUS);
}

void ata_init(void)
{
}

void ata_detect_devices(void)
{
}

const ata_device_t *ata_get_devices(uint8_t index)
{
    if (index >= 4)
    {
        return 0;
    }

    return &ata_devices[index];
}