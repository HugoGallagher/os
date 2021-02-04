#pragma once

#include <stdint.h>

uint16_t ata_detect();
void ata_delay();

void ata_poll();

void* ata_read(uint32_t lba, uint8_t sc);

void ata_identify(uint16_t* buffer);

void ata_read_buffer(uint16_t* buffer);
