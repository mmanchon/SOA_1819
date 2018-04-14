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
    /*printf();
    printf();
    printf();
    printf();
    */
}


FileSystem initSearchInfoExt4(int file){
    FileSystem fileSystem;
    fd = file;

    moveThroughExt4(SEEK_SET,PADDING_EXT4,&fileSystem.ext4.inodesCount,NULL);
    moveThroughExt4(SEEK_CUR,0,&fileSystem.ext4.blockCount,NULL);
    moveThroughExt4(SEEK_CUR,0,&fileSystem.ext4.reservedBlocksCount,NULL);
    moveThroughExt4(SEEK_CUR,0,&fileSystem.ext4.freeBlockCount,NULL);
    moveThroughExt4(SEEK_CUR,0,&fileSystem.ext4.freeInodesCount,NULL);
    moveThroughExt4(SEEK_CUR,0,&fileSystem.ext4.firstDataBlock,NULL);
    moveThroughExt4(SEEK_CUR,0,&fileSystem.ext4.blockSize,NULL);
    moveThroughExt4(SEEK_CUR,BITS_32,&fileSystem.ext4.blocksGroup,NULL);
    moveThroughExt4(SEEK_CUR,0,&fileSystem.ext4.fragsGroup,NULL);
    moveThroughExt4(SEEK_CUR,0,&fileSystem.ext4.inodesGroup,NULL);
    moveThroughExt4(SEEK_SET,PADDING_EXT4+OFF_FIRST_INODE,&fileSystem.ext4.firstInode,NULL);
    moveThroughExt4(SEEK_CUR,0,&fileSystem.ext4.inodeSize,NULL);

    showInfoExt4(fileSystem.ext4);

    return fileSystem;
}

void moveThroughExt4(int whence,off_t offset,uint32_t *field32, uint16_t *field16){
    if(lseek(fd,offset,whence) != -1) {

        if ((field32 == NULL && field16 == NULL) || (field32 != NULL && field16 != NULL)) {
            printf("Error.No hay ningún parametro a leer o hay uno que no es NULL\n");
            exit(1);
        } else if (field32 != NULL && field16 == NULL) {
            if (read(fd,field32, sizeof(uint32_t)) <= 0){
                printf("Erro. La lectura de 32byte na ha sido posible\n");
                exit(1);
            }
        } else if (field32 == NULL && field16 != NULL) {
            if (read(fd,field16, sizeof(uint16_t)) <= 0){
                printf("Erro. La lectura de 16byte na ha sido posible\n");
                exit(1);
            }
        }
    }else{
        printf("Error. El lseek no ha funcionado\n");
        exit(1);
    }
}