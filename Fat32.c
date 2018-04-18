//
// Created by sergi on 15/4/18.
//

#include "Fat32.h"

//http://www.file-recovery.com/recovery-understanding-file-system-fat.htm
//https://en.wikipedia.org/wiki/Design_of_the_FAT_file_system
//http://www.c-jump.com/CIS24/Slides/FAT/lecture.html
void showInfoFat32(VolumenFat32 fat32) {
    printf(FS_INFO);
    printf(TYPE_FS, "FAT32");
    printf("\n");
    printf(SYSTEM_NAME, fat32.systemName);
    printf(SECTOR_SIZE, fat32.sectorSize);
    printf(SECTOR_CLUSTER, (fat32.sectorsPerCluster));
    printf(RESERVED_SECTORS, (fat32.reservedSectors));
    printf(NUMBER_FATS, (fat32.numberFat));
    printf(ROOT_ENTRIES, (fat32.numberEntries));
    printf(SECTORS_FAT, (fat32.sectorsPerFat));
    printf(LABEL, fat32.label);

}

FileSystem initSearchInfoFat32(FileSystem fileSystem) {

    moveThroughFat32(SEEK_SET, OFF_NAME_SYS, BYTES_8, MAX_NUM_LIST, fileSystem.fat32.systemName);
    moveThroughFat32(SEEK_CUR, NO_OFF, BYTES_2, MAX_NUM_LIST, &fileSystem.fat32.sectorSize);
    moveThroughFat32(SEEK_CUR, NO_OFF, BYTES_1, MAX_NUM_LIST, &fileSystem.fat32.sectorsPerCluster);
    moveThroughFat32(SEEK_CUR, NO_OFF, BYTES_2, MAX_NUM_LIST, &fileSystem.fat32.reservedSectors);
    moveThroughFat32(SEEK_CUR, NO_OFF, BYTES_1, MAX_NUM_LIST, &fileSystem.fat32.numberFat);
    moveThroughFat32(SEEK_CUR, NO_OFF, BYTES_2, MAX_NUM_LIST, &fileSystem.fat32.numberEntries);
    moveThroughFat32(SEEK_SET, OFF_SECTOR_FAT, BYTES_2, MAX_NUM_LIST, &fileSystem.fat32.sectorsPerFat);
    moveThroughFat32(SEEK_SET, OFF_LABEL, BYTES_11, MAX_NUM_LIST, fileSystem.fat32.label);

    showInfoFat32(fileSystem.fat32);

    return fileSystem;
}

void moveThroughFat32(int whence, off_t offset, int bytes, int numArg, ...) {
    //Creamos una lista para los argumentos no definidos '...'
    va_list valist;
    //Constante MAX_NUM_LIST se encuentra en EXT4.h
    //Inicializamos la lista a 1 posicion
    va_start(valist, numArg);

    //Comprovamos que nos hemos podido desplazar el offset correspondiente
    if (lseek(fd, offset, whence) != -1) {
        //Segun la variable bytes leeremos un tipo de variables o otro
        if (bytes == BYTES_4) {
            uint32_t *aux = va_arg(valist, uint32_t * );
            if (read(fd, aux, sizeof(uint32_t)) <= 0) {
                printf("Erro. La lectura de 4byte na ha sido posible\n");
                exit(1);
            }
        } else if (bytes == BYTES_2) {
            uint16_t *aux = va_arg(valist, uint16_t * );
            if (read(fd, aux, sizeof(uint16_t)) <= 0) {
                printf("Erro. La lectura de 2 byte na ha sido posible\n");
                exit(1);
            }
        } else if (bytes == BYTES_1) {
            char *aux = va_arg(valist, uint8_t * );
            if (read(fd, aux, sizeof(uint8_t)) <= 0) {
                printf("Erro. La lectura de 1byte na ha sido posible\n");
                exit(1);
            }
        } else if (bytes == BYTES_8) {
            char *aux = va_arg(valist, char * );
            if (read(fd, aux, sizeof(char) * 8) <= 0) {
                printf("Erro. La lectura de 8byte na ha sido posible\n");
                exit(1);
            }

        } else if (bytes == BYTES_11) {
            char *aux = va_arg(valist, char * );
            if (read(fd, aux, sizeof(char) * 11) <= 0) {
                printf("Erro. La lectura de 8byte na ha sido posible\n");
                exit(1);
            }
        }

    } else {
        printf("Error. El lseek no ha funcionado\n");

        exit(1);
    }

    va_end(valist);
}

int checkIfFat32(int file) {
    fd = file;
    char aux[8];
    moveThroughFat32(SEEK_SET, OFF_FAT_TYPE, BYTES_8, 1, aux);
    printf("BUFFER: -%s-\n", aux);
    if (strcmp(aux, "FAT32   ") == 0) { return 1; }
    else if (strcmp(aux, "FAT12   ") == 0){
        printf(NOT_RECOGNIZED,"FAT12");
        return 3;
    } else if(strcmp(aux, "FAT16   ") == 0) {
        printf(NOT_RECOGNIZED,"FAT16");
        return 3;
    }else{
        return 2;

    }

}