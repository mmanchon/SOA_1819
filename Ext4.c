//
// Created by sergi on 14/4/18.
//

#include "Ext4.h"

/**
 * Función para mostrar la información de la Fase1
 * @param ext4 Tipo que contiene la información
 */
void showInfoExt4(VolumenExt4 ext4){
    printf(FS_INFO);
    printf(TYPE_FS,"EXT4");
    printf("\n");
    printf(INODE_INFO);
    printf(INODE_SIZE,ext4.inodeSize);
    printf(NUMBER_INODES,ext4.inodesCount);
    printf(FIRST_INODE,ext4.firstInode);
    printf(INODES_GROUP,ext4.inodesGroup);
    printf(FREE_INODES,ext4.freeInodesCount);
    printf(BLOCK_INFO);
    //Falta hacer 2^(10+blockSize)
    printf(BLOCK_SIZE,pow(2,(10+ext4.blockSize)));
    printf(RESERVED_BLOCK,ext4.reservedBlocksCount);
    printf(FREE_BLOCKS,ext4.freeBlockCount);
    printf(TOTAL_BLOCKS,ext4.blockCount);
    printf(FIRST_BLOCK,ext4.firstDataBlock);
    printf(BLOCK_GROUP,ext4.blocksGroup);
    printf(FRAGS_GROUP,ext4.fragsGroup);
    printf(VOLUME_INFO);
    printf(VOLUME_NAME,ext4.volumeName);
    /*printf();
    printf();
    printf();
    */
}


FileSystem initSearchInfoExt4(){
    FileSystem fileSystem;

    moveThroughExt4(SEEK_SET,PADDING_EXT4,BYTES_4,1,&fileSystem.ext4.inodesCount);
    moveThroughExt4(SEEK_CUR,0,BYTES_4,1,&fileSystem.ext4.blockCount);
    moveThroughExt4(SEEK_CUR,0,BYTES_4,1,&fileSystem.ext4.reservedBlocksCount);
    moveThroughExt4(SEEK_CUR,0,BYTES_4,1,&fileSystem.ext4.freeBlockCount);
    moveThroughExt4(SEEK_CUR,0,BYTES_4,1,&fileSystem.ext4.freeInodesCount);
    moveThroughExt4(SEEK_CUR,0,BYTES_4,1,&fileSystem.ext4.firstDataBlock);
    moveThroughExt4(SEEK_CUR,0,BYTES_4,1,&fileSystem.ext4.blockSize);
    moveThroughExt4(SEEK_CUR,BYTES_4,BYTES_4,1,&fileSystem.ext4.blocksGroup);
    moveThroughExt4(SEEK_CUR,0,BYTES_4,1,&fileSystem.ext4.fragsGroup);
    moveThroughExt4(SEEK_CUR,0,BYTES_4,1,&fileSystem.ext4.inodesGroup);
    moveThroughExt4(SEEK_SET,PADDING_EXT4+OFF_FIRST_INODE,BYTES_2,1,&fileSystem.ext4.firstInode);
    moveThroughExt4(SEEK_CUR,0,BYTES_4,1,&fileSystem.ext4.inodeSize);
    moveThroughExt4(SEEK_SET,PADDING_EXT4+OFF_VOLUME_NAME,BYTES_2,1,&fileSystem.ext4.volumeName);
    showInfoExt4(fileSystem.ext4);

    return fileSystem;
}

void moveThroughExt4(int whence,off_t offset,int bytes,int numArg, ...){
    //Creamos una lista para los argumentos no definidos '...'
    va_list valist;
    //Constante MAX_NUM_LIST se encuentra en EXT4.h
    //Inicializamos la lista a 1 posicion
    va_start(valist,numArg);

    //Comprovamos que nos hemos podido desplazar el offset correspondiente
    if(lseek(fd,offset,whence) != -1) {
        //Segun la variable bytes leeremos un tipo de variables o otro
        if(bytes == BYTES_4){
            uint32_t *aux = va_arg(valist, uint32_t *);
            if(read(fd,aux, sizeof(uint32_t)) <= 0){
                printf("Erro. La lectura de 32byte na ha sido posible\n");
                exit(1);
            }
        }else if(bytes == BYTES_2){
            uint16_t *aux = va_arg(valist, uint16_t *);
            if(read(fd,aux, sizeof(uint16_t)) <= 0){
                printf("Erro. La lectura de 16byte na ha sido posible\n");
                exit(1);
            }
        }else if(bytes == BYTES_1){
            char *aux = va_arg(valist, char *);
            if(read(fd,aux, sizeof(char)) <= 0){
                printf("Erro. La lectura de 8byte na ha sido posible\n");
                exit(1);
            }
        }
    }else{
        printf("Error. El lseek no ha funcionado\n");
        exit(1);
    }

    va_end(valist);
}

void checkIfExt4(int file){
    fd = file;
    uint32_t aux;
    //https://unix.stackexchange.com/questions/123009/reliable-way-to-detect-ext2-or-ext3-or-ext4
    moveThroughExt4(SEEK_SET,PADDING_EXT4+OFF_FEATURE_COMPAT,BYTES_4,1,&aux);
    printf("EL CHEQUEO DA %"PRIu32 "\n\n",aux);
    moveThroughExt4(SEEK_CUR,0,BYTES_4,1,&aux);
    printf("EL CHEQUEO2 DA %"PRIu32 "\n\n",aux);

}