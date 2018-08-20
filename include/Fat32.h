#ifndef _FAT32_H_
#define _FAT32_H_

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
#include <time.h>


//includes propios
#include "Types.h"

#include <conio.h>
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
#define ROOT_CLUSTER "\nREMOVE!!\nRootCluster: %hhu\n\n"

#define BYTES_10 10
#define BYTES_12 12
#define BYTES_1_V2 100

#define OFF_NAME_SYS 3
#define OFF_SECTORS_PER_FAT 0x24
#define OFF_LABEL 0x47
#define OFF_FAT_TYPE 0x52
#define OFF_FAT_TYPE_2 0x36
#define OFF_BYTES_PER_SEC 0x0B
#define OFF_ROOT_DIR 0x2C
#define OFF_SIGNATURE 0x1fe

#define FAT32_SIZE 512
#define LOW_ADDR_FC 0x1A //FIRST CLUSTER LOW
#define HIGH_ADDR_FC 0x14 //FIRST CLUSTER HIGH

int fd;

/********************************* FASE 1 *******************************/

/**
 * Funcio unicament destinada a mostrar informació del FS.
 * @param fat32
 */
void showInfoFat32(VolumenFat32 fat32);
/**
 * Funcio destinada a moure's per el FS obtenint la info necessaria.
 * @param fileSystem
 * @return
 */
VolumenFat32 getInfoFat32(VolumenFat32 fat32);

/**
 * Funció relacionada amb la fase 1 encarregada d'obtenir la informació deL FS.
 * @param fileSystem
 * @return
 */
void initSearchInfoFat32();

/********************************* FASE 2 *******************************/


/**
 * Function in charge of looking for the basic information of the volume ID.
 * This basic information is the bytes per sector, sectors per cluster,
 * the number of reserved sectors, number of FATs, Sectors per FAT, root
 * direcorty first cluster and the signature.
 * This attributes will help us moving through all the file system.
 * It's called when we detect the filesystem is FAT32 and we want to move throgh.
 * Also used in other phases
 * @return VolumeIdInfo is filled with all this attributes.
 */
VolumeIdInfo getBasicInfoVolumeId();


FATBasic calculateBasicFormulas(VolumeIdInfo volumeIdInfo);


int searchFileInFAT32(FATBasic fatBasic, UINT32 lba_adrr);




/********************************* FAT32 UTILITIES *******************************/

/**
 * Funció destinada a gestionar el moviment per el sistema de fitxers.
 * @param whence Tipus de moviment.
 * @param offset Offset que desitgem realitzar.
 * @param bytes Número de bytes a llegir.
 * @param numArg Numero d'arguments li passarem a la funció
 * @param ... Arguments que necessitarà la funció, com la variable
 * on guardar la informació llegida.
 */
void moveThroughFat32(int whence,off_t offset,int bytes,int numArg, ...);

/**
 * Encarregada de trobar si el fs es FAT32.
 * @param file File descriptor del volum.
 * @return File descriptor actualitzat.
 */
int checkIfFat32(int file);

UINT32 FAT32Table(UINT32 actualCluster, FATBasic fatBasic);

#endif
