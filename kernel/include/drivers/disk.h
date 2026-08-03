#ifndef DISK_H
#define DISK_H

#include <stdint.h>
#include "ata.h"

typedef struct {
    const ata_device_t* device;
} disk_t;

void disk_init(void);
const disk_t *disk_get(uint8_t index);
int disk_read(const disk_t *disk, uint32_t lba, uint8_t *buffer);
int disk_write(const disk_t *disk, uint32_t lba, const uint8_t *buffer);

#endif