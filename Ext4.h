//
// Created by sergi on 14/4/18.
//


#ifndef RAGNAROK_EXT4_H
#define RAGNAROK_EXT4_H


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
#include <math.h>
//includes propios
#include "types.h"


/**
 * Mensajes para EXT4
 * INODE INFO
 * */
#define INODE_INFO "INODE INFO\n"
#define INODE_SIZE "Inode Size: %d\n"
#define NUMBER_INODES "Number of Inodes: %d\n"
#define FIRST_INODE "First Inode: %d\n"
#define INODES_GROUP "Inodes Group: %d\n"
#define FREE_INODES "Free Inodes: %d\n\n"
/**
 * BLOCK INFO
 * */
#define BLOCK_INFO "BLOCK INFO\n"
#define BLOCK_SIZE "Block Size: %f\n"
#define RESERVED_BLOCK "Reserved Blocks: %d\n"
#define FREE_BLOCKS "Free Blocks: %d\n"
#define TOTAL_BLOCKS "Total Blocks: %d\n"
#define FIRST_BLOCK "First Block: %d\n"
#define BLOCK_GROUP "Block Group: %d\n"
#define FRAGS_GROUP "Frags Group: %d\n\n"
/**
 * VOLUME INFO
 * */
#define VOLUME_INFO "VOLUME INFO\n"
#define VOLUME_NAME "Volume name: %s\n"
#define LAST_CHECK_VOL "Last check: %s\n"
#define LAST_MOUNT_VOL "Last mount: %s\n"
#define LAST_WRITTEN "Last written: %s\n"

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

#define BITS_32 32
#define OFF_FIRST_INODE 0x54
int fd;

/**
 * Función para mostrar la información de la Fase1
 * @param ext4 Tipo que contiene la información
 */
void showInfoExt4(VolumenExt4 ext4);

FileSystem initSearchInfoExt4(int file);

void moveThroughExt4(int whence,off_t offset,uint32_t *field32, uint16_t *field16);

#endif //RAGNAROK_EXT4_H
