//
// Created by sergi on 13/4/18.
//

#ifndef RAGNAROK_TYPES_H
#define RAGNAROK_TYPES_H

/**
 * Constantes de Ext4
 * */
#define PADDING_EXT4 1024 //No siempre, hay que mirarlo IMPORTANTE

//OFFSET Y MAGIC NUMBER PARA EXT4
#define OFFSET_MAGICNUMBER 0x38
#define MAGIC_NUMBER_EXT4 0xEF53


/**
 * Constantes strings para mensajes
 * */
#define FS_INFO "--- FileSystem Information ---\n"
#define TYPE_FS "FileSystem: %s\n"
/**
 * ERROR MESSAGES
 * */
#define NOT_FOUND "Error.FileSystem not found\n"
#define NOT_RECOGNIZED "File System not recognized (%s)\n"


#define BYTES_1 1
#define BYTES_2 2
#define BYTES_4 4
#define BYTES_8 8
#define BYTES_11 11
#define BYTES_16 16
#define MAX_NUM_LIST 1


#define NO_OFF 0

typedef struct {
    //Inodes Info
    uint32_t inodesCount;
    uint32_t freeInodesCount;
    uint32_t inodesGroup;
    uint32_t inodeSize;
    uint32_t firstInode;

    //Block info
    uint32_t blockCount;
    uint32_t freeBlockCount;
    uint32_t reservedBlocksCount;
    uint32_t firstDataBlock;
    uint32_t blocksGroup;
    uint32_t blockSize;
    uint32_t fragsGroup;

    //Volume info
    char volumeName[16];
    uint32_t lastCheck;
    uint32_t lastMounted;
    uint32_t lastWritten;

}VolumenExt4;

typedef struct {
    char systemName[8];
    uint16_t sectorSize;
    uint8_t sectorsPerCluster;
    uint16_t reservedSectors;
    uint8_t numberFat;
    uint16_t numberEntries;
    uint32_t sectorsPerFat;
    uint32_t rootCluster;//4Bytes
    char label[11];
}VolumenFat32;

typedef struct {
    VolumenExt4 ext4;
    VolumenFat32 fat32;
}FileSystem;

typedef struct __attribute__((packed)){
    uint32_t inode;
    uint16_t rec_len;
    uint8_t name_len;
    uint8_t file_type;
}ext4_dir_entry_2;

typedef struct{
    uint16_t blockGroupSize;
    uint16_t inodeSize;
    uint32_t blockSize;
    uint32_t inodesPerGroup;
    uint64_t initInodeTable;
}DeepSearchExt4;
#endif //RAGNAROK_TYPES_H
