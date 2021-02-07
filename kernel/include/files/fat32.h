#pragma once

#include <stdint.h>

#include "files/ata.h"

struct FAT32BPB;
struct FAT32EBPB;
struct FAT32FSInfo;

struct FAT32DirEntry;
struct FAT32DirLFNEntry;

struct FAT32FS;

typedef struct FAT32BPB FAT32BPB;
typedef struct FAT32EBPB FAT32EBPB;
typedef struct FAT32FSInfo FAT32FSInfo;

typedef struct FAT32DirEntry FAT32DirEntry;
typedef struct FAT32DirLFNEntry FAT32DirLFNEntry;

typedef struct FAT32FS FAT32FS;

struct FAT32BPB
{
    uint16_t sector_size;
    uint8_t cluster_size;
    uint16_t reserved_sectors;
    uint8_t fat_count;
    uint16_t dir_entry_count;
    uint16_t sector_count;
    uint8_t media_descriptor;
    uint32_t hidden_sectors;
    uint32_t sector_count_high;
} __attribute__((packed));
struct FAT32EBPB
{
    uint32_t fat_size;
    uint16_t flags;
    uint8_t version_low;
    uint8_t version_high;
    uint32_t root_cluster_number;
    uint16_t fs_info_sector;
    uint16_t backup_boot_sector;
    uint8_t drive_number;
    uint8_t signature;
    char label[11];
} __attribute__((packed));
struct FAT32FSInfo
{
    uint32_t free_clusters;
    uint32_t first_cluster;
} __attribute__((packed));

struct FAT32DirEntry
{
    char name[11];
    uint8_t attributes;
    uint8_t reserved;
    uint8_t time_to_create;
    uint16_t creation_time;
    uint16_t creation_date;
    uint16_t last_accessed;
    uint16_t cluster_number_high;
    uint16_t last_modified_time;
    uint16_t last_modified_date;
    uint16_t cluster_number_low;
    uint32_t size;
} __attribute__((packed));
struct FAT32DirLFNEntry
{
    uint8_t order;
    uint16_t first_chars[5];
    uint8_t attribute;
    uint8_t type;
    uint8_t checksum;
    uint16_t late_chars[6];
    uint16_t zero;
    uint16_t last_chars[2];
} __attribute__((packed));

struct FAT32FS
{
    FAT32BPB bpb;
    FAT32EBPB ebpb;
    FAT32FSInfo fs_info;

    uint32_t start;
    uint32_t true_start;

    uint32_t sector_fat;
    uint32_t sector_data;
};

void fat32_init(FAT32FS* fs, MBRPartition partition);

void fat32_read(FAT32FS* fs, uint8_t* buffer, char* path, uint8_t path_size);

uint32_t fat32_get_next_cluster(FAT32FS* fs, uint32_t cc);
uint32_t fat32_get_sector_from_cluster(FAT32FS* fs, uint32_t c);

FAT32DirEntry fat32_get_file_dir_entry(FAT32FS* fs, char* path, uint8_t path_size);
FAT32DirEntry fat32_get_dir_entry(FAT32FS* fs, uint32_t c, char* n);
