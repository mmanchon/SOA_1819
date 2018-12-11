#ifndef RAGNAROK_TYPES_H
#define RAGNAROK_TYPES_H


#include <stdint.h>

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
#define NOT_FOUND "Error.FileSystem not found.\n"
#define NOT_RECOGNIZED "File System not recognized (%s).\n"

/**
 * FASE 3 MESSAGES
 * */
#define FILE_NOT_FOUND "Error. File not found.\n"
#define ARGUMENTS_NUMBER "Error.Incorrect parameter number.\n"
#define FILE_FOUND "File found! Size: %"PRIu64" bytes.  Created on: %d/%d/%d.\n"

/**
 * FASE 4 MESSAGES
 */

#define SHOWING_CONTENT "File found! Showing content...\n"

/**
 * FASE 5 MESSAGES
 */

#define CHANGE_RWX "Se han editado los permisos de %s\n"
#define CHANGE_DATE "Se han editado las fechas de %s\n"
#define NOT_ALLOWED "Operacion no valida para Ext4\n"

/**
 * NUMBER OF BYTES WE READ
 * */
#define BYTES_1 1
#define BYTES_2 2
#define BYTES_4 4
#define BYTES_8 8
#define BYTES_11 11
#define BYTES_16 16


#define MAX_NUM_LIST 1 /* MAXIMUM ARGUMENT NUMBER*/
#define NO_OFF 0 /* NO OFFSET FOR MOVING*/

#define UINT8 uint8_t
#define UINT16 uint16_t
#define UINT32 uint32_t
#define UINT64 uint64_t

/*** DEBUG SECTION **/
#define DEBUG

#define PRINT8 "Variable %s: -%"PRIu8"-\n"
#define PRINT16 "Variable %s: -%"PRIu16"-\n"
#define PRINT32 "Variable %s: -%"PRIu32"-\n"
#define PRINT64 "Variable %s: -%"PRIu64"-\n"

#ifdef DEBUG
#define DEBUG_PRINT(x) printf x
#else
#define DEBUG_PRINT(x) do {} while (0)
#endif

/********************* EXT4 ************************/


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

//Cuando hemos llegado al directory entrie entonces leemos directamente este typedef
typedef struct __attribute__((packed)){
    uint32_t inode;
    uint16_t rec_len;
    uint8_t name_len;
    uint8_t file_type;
}ext4_dir_entry_2;

//Typedef necesario para la fase 3.
//Vamos pasando este typedef de forma recursiva
typedef struct{
    uint16_t blockGroupSize;
    uint16_t inodeSize;
    uint32_t blockSize;
    uint32_t inodesPerGroup;
    uint64_t initInodeTable;
    char *file;
    uint64_t blockAddress;
    uint64_t initInodeFile;
    uint64_t fileSize;
    uint32_t dateFile;
    uint32_t incompat;
    uint16_t desc_size;
}DeepSearchExt4;

/********************* FAT32 ************************/


typedef struct __attribute__((packed)){
    char systemName[8];
    UINT16 sectorSize;
    UINT8 sectorsPerCluster;
    UINT16 reservedSectors;
    UINT8 numberFat;
    UINT16 numberEntries;
}SubFAT32;

typedef struct {
    SubFAT32 subFAT32;
    UINT32 sectorsPerFat;
    UINT32 rootCluster;
    char label[12];
}VolumenFat32;

typedef struct {
    UINT16 BPB_BytsPerSec;
    UINT8 BPB_SecPerClus;
    UINT16 BPB_RsvdSecCnt;
    UINT8 BPB_NumFATs;
    UINT32 BPB_FATSz32;
    UINT32 BPB_RootClus;
    UINT16 signature;
}VolumeIdInfo;

typedef struct{
    UINT32 fat_begin_lba;
    UINT32 cluster_begin_lba;
    UINT8 sectors_per_cluster;
    UINT32 root_dir_first_cluster;
}FATBasic;


typedef struct __attribute__((packed)){
    char DIR_Name[8];
    char DIR_Extension[3];
    UINT8 DIR_Attr;
    UINT16 DIR_FstClusHI;
    UINT16 DIR_FstClusLO;
    UINT32 DIR_FileSize;
}FAT32Dir;

typedef struct __attribute__((packed)){
    char seq_number;

}

#endif //RAGNAROK_TYPES_H
