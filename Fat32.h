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


//includes propios
#include "types.h"

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

#define OFF_NAME_SYS 3
#define OFF_SECTOR_FAT 0x24
#define OFF_LABEL 0x47
#define OFF_FAT_TYPE 0x52
#define OFF_FAT_TYPE_2 0x36

#define OFF_ROOT_DIR 0x2C

#define LOW_ADDR_FC 0x1A //FIRST CLUSTER LOW
#define HIGH_ADDR_FC 0x14 //FIRST CLUSTER HIGH

int fd;

typedef struct{
    int year;
    int month;
    int day;
    uint16_t  hex_date;
}Date;

typedef struct{
    char name[12];
    uint8_t attributes;
    int isDir;
    int isLfn;
    char file_size[4];
    char longname1[11];
    char longname2[13];
    char longname3[5];
    char hola;
    uint32_t size;
    Date date;
}Dir_info;

typedef struct{
    uint64_t cluster_begin_lba;
    uint64_t lba_adrr;
    uint64_t max_lba_adrr;
    uint32_t i_cluster;
    uint32_t init_cluster;
    char name[255];
    char old[255];
    char new_name[255];
    Dir_info dir;
}Lba_info;


/**
 * Funcio unicament destinada a mostrar el FS.
 * @param fat32
 */
void showInfoFat32(VolumenFat32 fat32);
/**
 * Funcio destinada a moure's per el FS obtenint la info necessaria.
 * @param fileSystem
 * @return
 */
FileSystem getInfoFat32(FileSystem fileSystem);

/**
 * Funció relacionada amb la fase 1 encarregada d'obtenir la informació deL FS.
 * @param fileSystem
 * @return
 */
FileSystem initSearchInfoFat32(FileSystem fileSystem);

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

/**
 * Funció destinada a obtenir la informació del volume_id del sector #0
 * del volum indicat.
 * @param fileSystem Tipus destinat a contenir la informació base del volume.
 * @return returnem el fs amb la informació obtinguda.
 */
//FileSystem goTroughFS(FileSystem fileSystem);
void goTroughFS(Lba_info info,char *argv, FileSystem fileSystem, Lba_info *trace, int *nTraces, int mode);

/**
 * Funció destinada a la cerca del fitxer introduit per l'usuari
 * @param fileSystem Li passem el tipus propi de FS on guardarem la info del FS.
 * @param argv Nom del fitxer.
 * @return
 */
FileSystem searchFileFat32(FileSystem fileSystem, char *argv);

/**
 * Funció que va al first cluster del segment de 32B del segment llegit del DS.
 * @param addr_base @ del DS on comença el segment de 32B del DS.
 * @return Una adreça vàlida del cluster on apunta el segment de 32B.
 */
uint32_t getNextClusterAddr(uint32_t addr_base);

/**
 * Conjunt de moviments necessaris per obtenir el nom del fitxer/directori.
 * @param info
 */
void getLongName(Lba_info *info);

/**
 * Actualitzem el recorregut realitzat, introduint info al array.
 * @param trace
 * @param nTraces Numero d'entrades de l'array
 * @return
 */
Lba_info * updateTrace(Lba_info *trace,int *nTraces,Lba_info info);

/**
 * Obtenim el ultim info guardat en el array de trace
 * @param trace
 * @param nTraces
 * @param info
 * @return
 */
Lba_info recoveryLastTrace(Lba_info *trace,int *nTraces, Lba_info info);

/**
 * Funció encarregada de gestionar la obtenció de tres tipus @'s diferents
 * relaionades amb la LBA.
 * @param mode
 * @param info
 * @param fileSystem
 */
void getAddr(int mode,Lba_info *info,FileSystem fileSystem);

/**
 * Funció encarregada de recorre l'arbre del sistema de fitxers i
 * mostrar el contingut del fitxer.
 * @param fileSystem
 * @param argv
 * @return
 */
FileSystem showContentFileFat32(FileSystem fileSystem, char *argv);

FileSystem activeReadModeFat32(FileSystem fileSystem,char *argv);
FileSystem activeWriteModeFat32(FileSystem fileSystem,char *argv);
FileSystem activeHideModeFat32(FileSystem fileSystem,char *argv);
FileSystem desactivateHideModeFat32(FileSystem fileSystem,char *argv);
#endif
