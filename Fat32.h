//
// Created by sergi on 15/4/18.
//

#ifndef RAGNAROK_FAT32_H
#define RAGNAROK_FAT32_H

//includes del systema
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <inttypes.h>
#include <stdarg.h>

//includes propios
#include "types.h"

/**
 * FAT32 MESSAGES
 * */
#define SYSTEM_NAME "System Name: %s\n"
#define SECTOR_SIZE "Sector Size: %d\n"
#define SECTOR_CLUSTER "Sectors per Cluster: %hhu\n"
#define RESERVED_SECTORS "Reserved Sectors: %d\n"
#define NUMBER_FATS "Number of FATs: %d\n"
#define ROOT_ENTRIES "Maximum Root Entries: %d\n"
#define SECTORS_FAT "Sectors per FAT: %d\n"
#define LABEL "Label: %s\n"


#define OFF_NAME_SYS 3
#define OFF_SECTOR_FAT 0x24
#define OFF_LABEL 0x47
#define OFF_FAT_TYPE 0x52

int fd;

void showInfoFat32(VolumenFat32 fat32);

FileSystem initSearchInfoFat32(FileSystem fileSystem);

void moveThroughFat32(int whence,off_t offset,int bytes,int numArg, ...);

int checkIfFat32(int file);

#endif //RAGNAROK_FAT32_H
