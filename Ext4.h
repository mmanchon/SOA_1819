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
#include <inttypes.h>
#include <math.h>
#include <stdarg.h>
#include <time.h>
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
#define BLOCK_SIZE "Block Size: %.2f\n"
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
#define LAST_CHECK_VOL "Last check: %s"
#define LAST_MOUNT_VOL "Last mount: %s"
#define LAST_WRITTEN "Last written: %s"


/**
 * TESTING INFO
 */
#define HAS_JOURNAL "Has journal %d\n"

#define OFF_FIRST_INODE 0x54
#define OFF_VOLUME_NAME 0x78
#define OFF_FEATURE_COMPAT 0x5C
#define OFF_FEATURE_INCOMPAT 0x60
#define OFF_LAST_CHECK 0x40
#define OFF_LAST_MOUNT 0x2C
#define OFF_BLOCKGROUP_SIZE 0xFE
#define OFF_EXTENT_TREE 0x28

#define PADDING_BLOCKGROUP_DESCRIPTORS 2048
#define PADDING_GROUP_DESCRIPTORS 64

int fd;
uint32_t blockSize;
/**
 * Función para mostrar la información de la Fase1
 * @param ext4 Tipo que contiene la información
 */
void showInfoExt4(VolumenExt4 ext4);

FileSystem initSearchInfoExt4(FileSystem fileSystem);

void moveThroughExt4(int whence,off_t offset,int bytes, int numArg, ...);

int checkIfExt4(int file);

void searchFileExt4();

int searchExtentTree(uint64_t initInodeTable, uint32_t inodesPerGroup, uint16_t inodeSize, int i);

int searchLeafs(uint64_t initExtentTree);

void infoLeaf(uint64_t initLeaf);

void readDirectoryInfo(uint64_t adress);
#endif //RAGNAROK_EXT4_H
