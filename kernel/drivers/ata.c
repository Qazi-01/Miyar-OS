#include "drivers/ata.h"
#include "drivers/io.h"
#include "terminal.h"
#include "drivers/serial.h"
#include "lib/string.h"

#include <stdint.h>
#include <stdbool.h>

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

static void ata_delay_400ns(ata_channel_t channel);
static int ata_poll(ata_channel_t channel);
static void ata_select_drive(ata_channel_t channel, ata_drive_t drive);

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

static void ata_delay(ata_channel_t channel)
{
    for (int i = 0; i < 4; i++)
    {
        ata_read_status(channel);
    }
}

static void ata_select_drive(ata_channel_t channel, ata_drive_t drive)
{
    uint16_t base = ata_base_port(channel);

    outb(base + ATA_REG_HDDEVSEL, 0xA0 | (drive << 4));
    ata_delay(channel);
}

static int ata_wait_busy(ata_channel_t channel)
{
    uint8_t status;

    do
    {
        status = ata_read_status(channel);
    }
    while (status & ATA_SR_BSY);

    return 0;
}

static int ata_identify(ata_channel_t channel, ata_drive_t drive)
{
    uint16_t base = ata_base_port(channel);
    
    ata_select_drive(channel, drive);

    outb(base + ATA_REG_SECTOR_COUNT, 0);
    outb(base + ATA_REG_LBA_LOW, 0);
    outb(base + ATA_REG_LBA_MID, 0);
    outb(base + ATA_REG_LBA_HIGH, 0);
    outb(base + ATA_REG_COMMAND, ATA_CMD_IDENTIFY);

    io_wait();

    if (inb(base + ATA_REG_STATUS) == 0)
    {
        return 0;
    }

    ata_wait_busy(channel);
    return 1;
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

static void ata_delay_400ns(ata_channel_t channel)
{
    uint16_t io = ata_base_port(channel);

    inb(io + ATA_REG_STATUS);
    inb(io + ATA_REG_STATUS);
    inb(io + ATA_REG_STATUS);
    inb(io + ATA_REG_STATUS);
}

static void ata_select_drive(ata_channel_t channel, ata_drive_t drive)
{
    uint16_t io = ata_base_port(channel);

    outb(io + ATA_REG_HDDEVSEL, 0xA0 | (drive << 4));

    ata_delay_400ns(channel);
}