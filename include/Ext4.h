
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
#include "Types.h"


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
 * IMPORTANT OFFSETS
 */

 #define OFF_FIRST_INODE 0x54
#define OFF_VOLUME_NAME 0x78
#define OFF_FEATURE_COMPAT 0x5C
#define OFF_FEATURE_INCOMPAT 0x60
#define OFF_LAST_CHECK 0x40
#define OFF_LAST_MOUNT 0x2C
#define OFF_BLOCKGROUP_SIZE 0xFE
#define OFF_EXTENT_TREE 0x28
#define OFF_FILE_DATE 0x90

/**
 * PADDINGS
 * */

#define PADDING_BLOCKGROUP_DESCRIPTORS 2048
#define PADDING_GROUP_DESCRIPTORS 64

int fd;


//Funciones necesarias para la fase 1

/**
 * Función para mostrar la información del volumen Ext4
 * @param ext4 Tipo que contiene la información
 */

void showInfoExt4(VolumenExt4 ext4);

/**
 * Funcion necesaria para mostrar la informacion del volumen
 * Esta funcion recorre el superbloque en busqueda de la informacion deseada
 * @param fileSystem struct donde almacenamos la informacion
 * @return struct lleno
 */

void initSearchInfoExt4();

/**
 * Funcion que te mueve por el fichero. Guarda el valor leido segun
 * la variable que le pases y de que tipo sea
 * @param whence SEEK_SET / SEEK_CUR /SEEK_END
 * @param offset donde te tienes que mover
 * @param bytes numero de bytes que leemos
 * @param numArg numero de variables a leer (SIEMPRE 1)
 * @param ... variable a leer
 */

void moveThroughExt4(int whence,off_t offset,int bytes, int numArg, ...);

/**
 * Funcion que distingue entre ext4 /ext3 /ext2
 * @param file file descriptor
 * @return segunt que tipo de volumen sea devuelve un valor
 */

int checkIfExt4(int file);

/**
 * Funcion que nos devuelve el struct del tiempo
 * @param time
 * @return
 */

struct tm* getTime(uint32_t time);

//Funciones necesarias para la fase2/3

/**
 * Funcion que busca la inforamcion necesaria dentro del superbloque para
 * empezar a buscar el fichero.
 * Una vez tiene la información busca el inicio de la tabla de inodos en
 * el group descriptor.
 * Finalmente busca el inicio del extent tree.
 * @param file nombre del fichero a buscar
 * @param ext4 struct con información necesaria
 * @return el inodo, 0 altrament
 */

uint64_t searchFileExt4(char *file, DeepSearchExt4 *ext4);

/**
 * Funcion para buscar el inicio del extent tree
 * Calcula la posicion del inodo raiz, comprueba si hay un
 * extent tree con el magic number y se mueve un offset
 * @param ext4 struct con informacion necesaria
 * @return inodo del fichero, 0 altrament
 */

uint64_t searchExtentTree(DeepSearchExt4 *ext4);

/**
 * Funcion para leer el numero de entries validas y el depth
 * Segun esta informacion sabremos si es una hoja o un nodo interno
 * @param initExtentTree offset donde empieza el extent tree
 * @param ext4 struct con informacion
 * @return inodo, 0 altrament
 */

uint64_t searchInfoExtent(uint64_t initExtentTree, DeepSearchExt4 *ext4);

/**
 * Funcion necesaria para leer el numero de bloques que ocupa la eentry y
 * la direccion dende esta se encuentra. Esta funcion se llama cuando sabemos
 * que es un nodo hoja
 * @param initLeaf offset donde leemos el nodo hoja
 * @param ext4 struct con informacion
 * @return inodo, 0 altrament
 */

uint64_t infoLeaf(uint64_t initLeaf,DeepSearchExt4 *ext4);

/**
 * Funcion para leer la inforamcion del directory entry.
 * @param adress
 * @param index
 * @param ee_len
 * @param ext4
 * @return
 */

uint64_t readDirectoryInfo(uint64_t adress, int index, uint16_t ee_len, DeepSearchExt4 *ext4);

/**
 * Función que recorre los nodos internos del extent tree
 * @param initNode
 * @param ext4
 * @return
 */

uint64_t internalNodesExtentTree(uint64_t initNode, DeepSearchExt4 *ext4);

/**
 * Funcion para comprobar que el fichero es el que busca el comando
 * @param dir
 * @param ext4
 * @param name
 * @return
 */

uint64_t checkFile(ext4_dir_entry_2 dir,DeepSearchExt4 *ext4, char *name);



//Funciones necesarias para la fase 4

/**
 * Funcion inicial para mostrar el contenido del fichero
 * @param fileInode
 * @param ext4
 */
void findFileInfo(uint64_t fileInode, DeepSearchExt4 ext4);

/**
 * Funcion para leer el numero de entries validas y el depth
 * Segun esta informacion sabremos si es una hoja o un nodo interno
 * @param offset
 * @param ext4
 */
uint64_t findExtentTreeInfo(uint64_t offset, DeepSearchExt4 ext4,uint64_t numRead);

/**
 *Funcion necesaria para leer el numero de bloques que ocupa la eentry y
 * la direccion dende esta se encuentra. Esta funcion se llama cuando sabemos
 * que es un nodo hoja
 * @param initLeaf
 * @param ext4
 * @param numRead
 * @return
 */
uint64_t fileLeaf(uint64_t initLeaf,DeepSearchExt4 ext4, uint64_t numRead);

/**
 * Función que recorre los nodos internos del extent tree
 * @param initNode
 * @param ext4
 */
uint64_t internalFileNodes(uint64_t initNode, DeepSearchExt4 ext4, uint64_t numRead);

/**
 * Muestra la información del fichero
 * @param offset
 * @param ee_len
 * @param ext4
 * @param numRead
 * @return
 */
uint64_t showInfoFile(uint64_t offset, uint16_t ee_len, DeepSearchExt4 ext4, uint64_t numRead);

//Funciones fase 5

/**
 * Modifica los parametros del fichero a solo lectura
 * @param offset
 */
void activateReadMode(uint64_t offset);

/**
 * Modifica los parametros del fichero a no solo lectura
 * @param offset
 */
void deactivateReadMode(uint64_t offset);

/**
 * Modifica la fecha de creacion
 * @param offset
 * @param date
 */
void changeDateFile(uint64_t offset, char* date);

#endif //RAGNAROK_EXT4_H
