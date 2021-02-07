#pragma once

#include <stdint.h>

struct MBRPartition;

typedef struct MBRPartition MBRPartition;

struct MBRPartition
{
    uint8_t attributes;
    uint32_t start_chs;
    uint8_t type;
    uint32_t end_chs;
    uint32_t start_lba;
    uint32_t sector_count;
};

uint16_t ata_detect();
void ata_delay();

void ata_poll();

void* ata_read(uint32_t lba, uint32_t base, uint8_t sc);
void ata_read_to_dest(uint16_t* data, uint32_t lba, uint32_t base, uint8_t sc);

void ata_identify(uint16_t* buffer);

void ata_read_buffer(uint16_t* buffer);

MBRPartition mbr_get_partition(uint8_t* mbr, uint8_t n);
