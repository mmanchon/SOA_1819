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
#include <linux/kernel.h>
#include <inttypes.h>
//includes propios
#include "types.h"

/**
 * FAT32 MESSAGES
 * */
#define SYSTEM_NAME "System Name: %s\n"
#define SECTOR_SIZE "Sector Size: %d\n"
#define SECTOR_CLUSTER "Sectors per Cluster: %d\n"
#define RESERVED_SECTORS "Reserved Sectors: %d\n"
#define NUMBER_FATS "Number of FATs: %d\n"
#define ROOT_ENTRIES "Maximum Root Entries: %d\n"
#define SECTORS_FAT "Sectors per FAT: %d\n"
#define LABEL "Label: %s\n"

#define BYTES_1 1
#define BYTES_2 2
#define BYTES_8 8
#define BYTES_11 11

#define OFF_NAME_SYS 3
#define NO_OFF 0
#define OFF_LABEL 43
int fd;

void showInfoFat32(VolumenFat32 fat32);

FileSystem initSearchInfoFat32(FileSystem fileSystem);

void moveThroughFat32(int whence,off_t offset,int bytes, char *var);

void checkIfFat32(int file);

#endif //RAGNAROK_FAT32_H
