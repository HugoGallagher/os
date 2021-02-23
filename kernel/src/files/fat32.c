#include <stdbool.h>

#include "files/fat32.h"

#include "lib/mem.h"

void fat32_init(FAT32FS* fs, MBRPartition partition)
{
    fs->true_start = partition.start_lba;

    uint8_t* br = ata_read(0, fs->true_start, 1);

    fs->bpb.sector_size = *(uint16_t*)(br + 11);
    fs->bpb.cluster_size = *(uint8_t*)(br + 13);
    fs->bpb.reserved_sectors = *(uint16_t*)(br + 14);
    fs->bpb.fat_count = *(uint8_t*)(br + 16);
    fs->bpb.dir_entry_count = *(uint16_t*)(br + 17);
    fs->bpb.sector_count = *(uint16_t*)(br + 19);
    fs->bpb.media_descriptor = *(uint8_t*)(br + 20);
    fs->bpb.hidden_sectors = *(uint32_t*)(br + 28);
    fs->bpb.sector_count_high = *(uint32_t*)(br + 32);

    fs->ebpb.fat_size = *(uint32_t*)(br + 36);
    fs->ebpb.flags = *(uint16_t*)(br + 40);
    fs->ebpb.version_low = *(uint8_t*)(br + 42);
    fs->ebpb.version_high = *(uint8_t*)(br + 43);
    fs->ebpb.root_cluster_number = *(uint32_t*)(br + 44);
    fs->ebpb.fs_info_sector = *(uint16_t*)(br + 48);
    fs->ebpb.backup_boot_sector = *(uint16_t*)(br + 50);
    fs->ebpb.drive_number = *(uint8_t*)(br + 64);
    fs->ebpb.signature = *(uint8_t*)(br + 66);

    memcpy(fs->ebpb.label, br + 71, 11);

    fs->start = fs->true_start + fs->bpb.hidden_sectors;

    uint8_t* fsi = ata_read(fs->ebpb.fs_info_sector, fs->start, 1);

    //kmalloc(575);

    fs->fs_info.free_clusters = *(uint32_t*)(fsi + 488);
    fs->fs_info.free_clusters = *(uint32_t*)(fsi + 492);

    fs->sector_fat = fs->bpb.reserved_sectors;
    fs->sector_data = fs->sector_fat + fs->ebpb.fat_size * fs->bpb.fat_count;

    kfree(fsi);
    kfree(br);
}

void* fat32_read(FAT32FS* fs, char* path, uint8_t path_size)
{
    FAT32DirEntry dir_entry = fat32_get_file_info(fs, path, path_size);

    if (dir_entry.attributes & 0x10 || dir_entry.size == 0)
        return;

    uint32_t size_in_clusters = (dir_entry.size - 1);
    size_in_clusters += (fs->bpb.sector_size * fs->bpb.cluster_size) - size_in_clusters % (fs->bpb.sector_size * fs->bpb.cluster_size);
    size_in_clusters /= (fs->bpb.sector_size * fs->bpb.cluster_size);

    void* buffer = kmalloc(size_in_clusters * fs->bpb.sector_size);
    uint32_t current_cluster = (dir_entry.cluster_number_high << 16) + dir_entry.cluster_number_low;
    for (uint32_t i = 0; i < size_in_clusters; i++)
    {
        // assumes cluster size of 1 sector
        ata_read_to_dest(buffer + i * fs->bpb.sector_size, fat32_get_sector_from_cluster(fs, current_cluster), fs->true_start, fs->bpb.cluster_size);
    }

    return buffer;
}
void fat32_read_to_dest(FAT32FS* fs, uint8_t* buffer, char* path, uint8_t path_size)
{
    FAT32DirEntry dir_entry = fat32_get_file_info(fs, path, path_size);

    if (dir_entry.attributes & 0x10 || dir_entry.size == 0)
        return;

    uint32_t size_in_clusters = (dir_entry.size - 1);
    size_in_clusters += (fs->bpb.sector_size * fs->bpb.cluster_size) - size_in_clusters % (fs->bpb.sector_size * fs->bpb.cluster_size);
    size_in_clusters /= (fs->bpb.sector_size * fs->bpb.cluster_size);

    uint32_t current_cluster = (dir_entry.cluster_number_high << 16) + dir_entry.cluster_number_low;
    for (uint32_t i = 0; i < size_in_clusters; i++)
    {
        // assumes cluster size of 1 sector
        ata_read_to_dest(buffer + i * fs->bpb.sector_size, fat32_get_sector_from_cluster(fs, current_cluster), fs->true_start, fs->bpb.cluster_size);
    }
}

uint32_t fat32_get_next_cluster(FAT32FS* fs, uint32_t cc)
{
    uint32_t offset = cc * 4;
    uint32_t sector = fs->bpb.reserved_sectors + (offset / fs->bpb.sector_size);
    uint32_t index = offset % fs->bpb.sector_size;

    uint32_t* fat = ata_read(sector, fs->start, 1);

    uint32_t value = fat[index] & 0x0FFFFFFF;

    kfree(fat);

    return value;
}
uint32_t fat32_get_sector_from_cluster(FAT32FS* fs, uint32_t c)
{
    return fs->sector_data + (c - 2) * fs->bpb.cluster_size;
}

FAT32DirEntry fat32_get_file_info(FAT32FS* fs, char* path, uint8_t path_size)
{
    // calculate depth of path and allocate
    uint8_t depth = 1;
    for (uint8_t i = 0; i < path_size - 1; i++)
    {
        if (path[i] == 0x2F)
            depth++;
    }

    if (path[path_size - 1] == 0x2F)
        path[path_size - 1] = 0x00;

    // array of indexes of paths
    uint8_t dirs_start[depth];
    uint8_t dirs_size[depth];

    dirs_start[0] = 0;

    uint8_t current_depth = 1;
    uint8_t current_path_offset = 0;
    bool final_dir = false;
    while (current_path_offset < path_size && !final_dir)
    {
        char* current_path = path + current_path_offset;
        uint8_t current_path_size = 0;
        for (uint8_t i = 0; i < path_size; i++)
        {
            current_path_size++;
            if (current_path[i] == 0)
            {
                final_dir = true;
                i = path_size;
            }
            if (current_path[i] == 0x2F)
            {
                current_path_offset += current_path_size;
                i = path_size;
            }
        }

        dirs_start[current_depth] = current_path_offset;
        dirs_size[current_depth - 1] = current_path_size - 1;
        current_depth++;
    }

    FAT32DirEntry current_dir_entry;
    uint32_t current_cluster = fs->ebpb.root_cluster_number;
    for (uint8_t i = 0; i < depth; i++)
    {
        char name[12];
        bzero(name, 12);
        memcpy(name, path + dirs_start[i], dirs_size[i]);

        current_dir_entry = fat32_get_dir_entry(fs, current_cluster, name);
        current_cluster = (current_dir_entry.cluster_number_high << 16) + current_dir_entry.cluster_number_low;
        //terminal_write(name, 12);
    }

    return current_dir_entry;
}

FAT32DirEntry fat32_get_dir_entry(FAT32FS* fs, uint32_t c, char* n)
{
    FAT32DirEntry current_dir_entry;
    FAT32DirEntry* directory_data;
    uint32_t dir_size = 0;
    uint32_t current_cluster = c;
    while (current_cluster < 0x0FFFFFF8)
    {
        directory_data = ata_read(fat32_get_sector_from_cluster(fs, current_cluster), fs->true_start, 1);

        for (uint32_t i = 0; i < fs->bpb.sector_size / sizeof(FAT32DirEntry); i++)
        {
            // for now only 1 LFN per entry is supported
            FAT32DirLFNEntry* lfns = &(directory_data[i]);
            bool lfn_present = false;
            uint8_t lfn_c = 0;
            while (lfns[lfn_c].attribute == 0x0F)
            {
                lfn_present = true;
                i++;
                lfn_c++;
            }

            char name[12];
            current_dir_entry = directory_data[i];

            if (*(uint8_t*)&current_dir_entry == 0x00)
            {
                i = fs->bpb.sector_size / sizeof(FAT32DirEntry);
                continue;
            }
            if (*(uint8_t*)&current_dir_entry == 0xE5)
                continue;

            if (lfn_present == true)
            {
                for (uint8_t j = 0; j < 5; j++)
                {
                    if (lfns[0].first_chars[j] == 0xFFFF)
                        name[j] = 0;
                    else
                        name[j] = lfns[0].first_chars[j] & 0x00FF;
                }
                for (uint8_t j = 0; j < 6; j++)
                {
                    if (lfns[0].late_chars[j] == 0xFFFF)
                        name[5 + j] = 0;
                    else
                        name[5 + j] = lfns[0].late_chars[j] & 0x00FF;
                }
                for (uint8_t j = 0; j < 2; j++)
                {
                    if (lfns[0].last_chars[j] == 0xFFFF)
                        name[11 + j] = 0;
                    else
                        name[11 + j] = lfns[0].last_chars[j] & 0x00FF;
                }
            }
            else
            {
                for (uint8_t j = 0; j < 11; j++)
                {
                    // convert to lower case (ASCII)
                    if (current_dir_entry.name[j] >= 0x41 && current_dir_entry.name[j] <= 0x5A)
                        name[j] = current_dir_entry.name[j] + 0x20;
                    else
                        name[j] = current_dir_entry.name[j];
                }
            }

            if (memcmp(name, n, 12))
            {
                kfree(directory_data);
                return current_dir_entry;
            }
        }

        kfree(directory_data);
        current_cluster = fat32_get_next_cluster(fs, current_cluster);
        dir_size++;
    }

    terminal_writestring("File not found\n");
}
