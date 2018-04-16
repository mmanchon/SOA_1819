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

}VolumenExt4;

typedef struct {

}VolumenFat32;

typedef struct {
    VolumenExt4 ext4;
    VolumenFat32 fat32;
}FileSystem;
#endif //RAGNAROK_TYPES_H
