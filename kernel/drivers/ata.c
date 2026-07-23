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
static uint16_t ata_identify_buffer[256];

static void ata_delay_400ns(ata_channel_t channel);
static int ata_poll(ata_channel_t channel);
static void ata_select_drive(ata_channel_t channel, ata_drive_t drive);
static uint16_t ata_read_data(uint16_t base);

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

static int ata_identify(ata_channel_t channel, ata_drive_t drive)
{
    uint16_t base = ata_base_port(channel);
    
    ata_select_drive(channel, drive);

    ata_delay_400ns(channel);

    outb(base + ATA_REG_SECTOR_COUNT, 0);
    outb(base + ATA_REG_LBA_LOW, 0);
    outb(base + ATA_REG_LBA_MID, 0);
    outb(base + ATA_REG_LBA_HIGH, 0);
    outb(base + ATA_REG_COMMAND, ATA_CMD_IDENTIFY);

    uint8_t status = ata_read_status(channel);
    if (status == 0)
    {
        return 0;
    }

    while (ata_read_status(channel) & ATA_SR_BSY)
    {
    }

    if (inb(base + ATA_REG_LBA_MID) != 0 || inb(base + ATA_REG_LBA_HIGH) != 0)
    {
        return 0;
    }

    io_wait();

    while (1)
    {
        uint8_t status = ata_read_status(channel);

        if (status & ATA_SR_ERR)
        {
            return 0;
        }

        if (status & ATA_SR_DRQ)
        {
            break;
        }
    }

    for (int i = 0; i < 256; i++)
    {
        ata_identify_buffer[i] = ata_read_data(base);
    }
    
    return 1;
}

void ata_init(void)
{
    memset(ata_devices, 0, sizeof(ata_devices));
    memset(ata_identify_buffer, 0, sizeof(ata_identify_buffer));
}

void ata_detect_devices(void)
{
    uint8_t index = 0;

    for (ata_channel_t channel = ATA_PRIMARY; channel <= ATA_SECONDARY; channel++)
    {
        for (ata_drive_t drive = ATA_MASTER; drive <= ATA_SLAVE; drive++)
        {
            ata_devices[index].channel = channel;
            ata_devices[index].drive = drive;

            if (ata_identify(channel, drive))
            {
                ata_devices[index].present = true;
            
                terminal_write("ATA: Device Detected\n");
            }

            else
            {
                ata_devices[index].present = false;
            }

            index++;
        }
    }
}

const ata_device_t *ata_get_device(uint8_t index)
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

static int ata_poll(ata_channel_t channel)
{
    uint8_t status;
    ata_delay_400ns(channel);

    do
    {
        status = ata_read_status(channel);
    }
    while (status & ATA_SR_BSY);

    if (status & ATA_SR_ERR)
    {
        return -1;
    }

    if (status & ATA_SR_DF)
    {
        return -1;
    }

    if (!(status & ATA_SR_DRQ))
    {
        return -1;
    }

    return 0;
}

static uint16_t ata_read_data(uint16_t base)
{
    uint16_t value;

    __asm__ volatile ("inw %1, %0" : "=a"(value) : "Nd"(base));

    return value;
}