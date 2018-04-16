//
// Created by sergi on 15/4/18.
//

#include "Fat32.h"

//http://www.file-recovery.com/recovery-understanding-file-system-fat.htm
//https://en.wikipedia.org/wiki/Design_of_the_FAT_file_system
//http://www.c-jump.com/CIS24/Slides/FAT/lecture.html
void showInfoFat32(VolumenFat32 fat32){
    printf(FS_INFO);
    printf(TYPE_FS,"FAT32");
    printf("\n");
    printf(SYSTEM_NAME,fat32.systemName);
    printf(SECTOR_SIZE,atoi(fat32.sectorSize));
    printf(SECTOR_CLUSTER,atoi(&fat32.sectorsPerCluster));
    printf(RESERVED_SECTORS,atoi(fat32.reservedSectors));
    printf(NUMBER_FATS,atoi(&fat32.numberFat));
    printf(ROOT_ENTRIES,atoi(fat32.numberEntries));
    printf(SECTORS_FAT,atoi(fat32.sectorsPerFat));
    printf(LABEL,fat32.label);

}

FileSystem initSearchInfoFat32(FileSystem fileSystem){
    moveThroughFat32(SEEK_SET,OFF_NAME_SYS,8,fileSystem.fat32.systemName);
    uint32_t aux;
    read(fd,&aux, sizeof(uint32_t));
    printf("Esto es una prueba %"PRIu32"\n",aux);
    moveThroughFat32(SEEK_SET,0x0B,32,fileSystem.fat32.sectorSize);
    moveThroughFat32(SEEK_CUR,NO_OFF,BYTES_1,&fileSystem.fat32.sectorsPerCluster);
    moveThroughFat32(SEEK_CUR,NO_OFF,BYTES_2,fileSystem.fat32.reservedSectors);
    moveThroughFat32(SEEK_CUR,NO_OFF,BYTES_1,&fileSystem.fat32.numberFat);
    moveThroughFat32(SEEK_CUR,NO_OFF,BYTES_2,fileSystem.fat32.numberEntries);
    moveThroughFat32(SEEK_CUR,NO_OFF,BYTES_2,fileSystem.fat32.sectorsPerFat);
    moveThroughFat32(SEEK_SET,OFF_LABEL,BYTES_11,fileSystem.fat32.label);

    showInfoFat32(fileSystem.fat32);

    return fileSystem;
}

void moveThroughFat32(int whence,off_t offset,int bytes, char *var){

    //Comprovamos que nos hemos podido desplazar el offset correspondiente
    if(lseek(fd,offset,whence) != -1) {
        //Segun la variable bytes leeremos un tipo de variables o otro
        if(read(fd,var, sizeof(char)*bytes) <= 0){
            printf("Error. La lectura de %d bytes no ha sido posible\n",bytes);
            exit(1);
        }
    }else{
        printf("Error. El lseek no ha funcionado\n");
        exit(1);
    }
}

void checkIfFat32(int file){
    fd = file;
}