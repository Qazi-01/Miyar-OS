#ifndef ATA_H
#define ATA_H

#include <stdint.h>
#include <stdbool.h>

typedef enum {
    ATA_PRIMARY = 0,
    ATA_SECONDARY = 1
} ata_channel_t;

typedef enum {
    ATA_MASTER = 0,
    ATA_SLAVE = 1
} ata_drive_t;

typedef struct {
    bool present;

    ata_channel_t channel;
    ata_drive_t drive;

    char model[41];

    uint32_t sector_count;
} ata_device_t;

void ata_init(void);
void ata_detect_devices(void);
const ata_device_t *ata_get_device(uint8_t index);

#endif