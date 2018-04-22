//
// Created by sergi on 14/4/18.
//

#include "Ext4.h"

/**
 * Función para mostrar la información de la Fase1
 * @param ext4 Tipo que contiene la información
 */
void showInfoExt4(VolumenExt4 ext4) {
    struct tm *timeinfo;
    time_t rawTime;

    printf(FS_INFO);
    printf(TYPE_FS, "EXT4");
    printf("\n");
    printf(INODE_INFO);

    printf(INODE_SIZE, ext4.inodeSize);
    printf(NUMBER_INODES, ext4.inodesCount);
    printf(FIRST_INODE, ext4.firstInode);
    printf(INODES_GROUP, ext4.inodesGroup);
    printf(FREE_INODES, ext4.freeInodesCount);
    printf(BLOCK_INFO);
    //Falta hacer 2^(10+blockSize)
    printf(BLOCK_SIZE, pow(2, (10 + ext4.blockSize)));
    printf(RESERVED_BLOCK, ext4.reservedBlocksCount);
    printf(FREE_BLOCKS, ext4.freeBlockCount);
    printf(TOTAL_BLOCKS, ext4.blockCount);
    printf(FIRST_BLOCK, ext4.firstDataBlock);
    printf(BLOCK_GROUP, ext4.blocksGroup);
    printf(FRAGS_GROUP, ext4.fragsGroup);
    printf(VOLUME_INFO);
    printf(VOLUME_NAME, ext4.volumeName);

    rawTime = (time_t) ext4.lastCheck;
    timeinfo = localtime(&rawTime);
    printf(LAST_CHECK_VOL, asctime(timeinfo));

    rawTime = (time_t) ext4.lastMounted;
    timeinfo = localtime(&rawTime);
    printf(LAST_MOUNT_VOL, asctime(timeinfo));
    rawTime = (time_t) ext4.lastWritten;
    timeinfo = localtime(&rawTime);
    printf(LAST_WRITTEN, asctime(timeinfo));

}


FileSystem initSearchInfoExt4(FileSystem fileSystem) {

    moveThroughExt4(SEEK_SET, PADDING_EXT4, BYTES_4, MAX_NUM_LIST, &fileSystem.ext4.inodesCount);
    moveThroughExt4(SEEK_CUR, NO_OFF, BYTES_4, MAX_NUM_LIST, &fileSystem.ext4.blockCount);
    moveThroughExt4(SEEK_CUR, NO_OFF, BYTES_4, MAX_NUM_LIST, &fileSystem.ext4.reservedBlocksCount);
    moveThroughExt4(SEEK_CUR, NO_OFF, BYTES_4, MAX_NUM_LIST, &fileSystem.ext4.freeBlockCount);
    moveThroughExt4(SEEK_CUR, NO_OFF, BYTES_4, MAX_NUM_LIST, &fileSystem.ext4.freeInodesCount);
    moveThroughExt4(SEEK_CUR, NO_OFF, BYTES_4, MAX_NUM_LIST, &fileSystem.ext4.firstDataBlock);
    moveThroughExt4(SEEK_CUR, NO_OFF, BYTES_4, MAX_NUM_LIST, &fileSystem.ext4.blockSize);
    moveThroughExt4(SEEK_CUR, BYTES_4, BYTES_4, MAX_NUM_LIST, &fileSystem.ext4.blocksGroup);
    moveThroughExt4(SEEK_CUR, NO_OFF, BYTES_4, MAX_NUM_LIST, &fileSystem.ext4.fragsGroup);
    moveThroughExt4(SEEK_CUR, NO_OFF, BYTES_4, MAX_NUM_LIST, &fileSystem.ext4.inodesGroup);
    moveThroughExt4(SEEK_SET, PADDING_EXT4 + OFF_FIRST_INODE, BYTES_4, MAX_NUM_LIST, &fileSystem.ext4.firstInode);
    moveThroughExt4(SEEK_CUR, NO_OFF, BYTES_4, MAX_NUM_LIST, &fileSystem.ext4.inodeSize);
    moveThroughExt4(SEEK_SET, PADDING_EXT4 + OFF_VOLUME_NAME, BYTES_16, MAX_NUM_LIST, fileSystem.ext4.volumeName);
    moveThroughExt4(SEEK_SET, PADDING_EXT4 + OFF_LAST_CHECK, BYTES_4, MAX_NUM_LIST, &fileSystem.ext4.lastCheck);
    moveThroughExt4(SEEK_SET, PADDING_EXT4 + OFF_LAST_MOUNT, BYTES_4, MAX_NUM_LIST, &fileSystem.ext4.lastMounted);
    moveThroughExt4(SEEK_CUR, NO_OFF, BYTES_4, MAX_NUM_LIST, &fileSystem.ext4.lastWritten);

    showInfoExt4(fileSystem.ext4);

    return fileSystem;
}

void moveThroughExt4(int whence, off_t offset, int bytes, int numArg, ...) {
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
                printf("Erro. La lectura de 2byte na ha sido posible\n");
                exit(1);
            }
        } else if (bytes == BYTES_1) {
            char *aux = va_arg(valist, char * );
            if (read(fd, aux, sizeof(char)) <= 0) {
                printf("Erro. La lectura de 1byte na ha sido posible\n");
                exit(1);
            }
        } else if (bytes == BYTES_16) {
            char *aux = va_arg(valist, char * );
            if (read(fd, aux, sizeof(char) * 16) <= 0) {
                printf("Erro. La lectura de 16byte na ha sido posible\n");
                exit(1);
            }
        }
    } else {
        printf("Error. El lseek no ha funcionado\n");
        exit(1);
    }

    va_end(valist);
}

int checkIfExt4(int file) {
    fd = file;

    uint32_t feature_compat;
    uint32_t feature_incompat;
    int has_extent, has_journal;

    moveThroughExt4(SEEK_SET, PADDING_EXT4 + OFF_FEATURE_COMPAT, BYTES_4, 1, &feature_compat);
    moveThroughExt4(SEEK_SET, PADDING_EXT4 + OFF_FEATURE_INCOMPAT, BYTES_4, 1, &feature_incompat);

    has_extent = feature_incompat & 0x40;
    has_journal = feature_compat & 0x4;

    if (has_extent>0) {
        return 1;
    } else if (has_journal) {
        printf(NOT_RECOGNIZED, "EXT3");
        exit(1);
    } else {
        printf(NOT_RECOGNIZED, "EXT2");
        exit(1);
    }
    return 0;
}


void searchFileExt4() {
    uint16_t blockGroupSize;
    uint32_t upperInodeBitmap;
    uint32_t lowerInodeBitmap;
    uint64_t inodeBitmap;
    uint32_t lowerInodeTable;
    uint32_t upperInodeTable;
    uint64_t inodeTable;
    uint32_t feature_compat;
    uint16_t reservedGdtBlocks;
    uint32_t blockSize;

    moveThroughExt4(SEEK_SET, PADDING_EXT4 + OFF_FEATURE_COMPAT, BYTES_4, 1, &feature_compat);
    feature_compat = feature_compat & 0x10;
    printf("FEATURE COMPAT HAS GDT BLOCKS -%"PRIu32"-\n", feature_compat);
    moveThroughExt4(SEEK_SET, PADDING_EXT4 +0xCE, BYTES_2, 1, &reservedGdtBlocks);
    printf("RESERVED GDT BLOCKS -%"PRIu16"-\n", reservedGdtBlocks);
    moveThroughExt4(SEEK_SET, PADDING_EXT4+0x18, BYTES_4, MAX_NUM_LIST, &blockSize);
    size = pow(2, (10 + blockSize));

    //leemos el tamaño del block group descriptors indicado en el superblock con offset de 0xFE
    moveThroughExt4(SEEK_SET,PADDING_EXT4+OFF_BLOCKGROUP_SIZE,BYTES_2,1,&blockGroupSize);
    printf("SIZE OF BLOCK GROUP DESCRIPTOR -%"PRIu16"-\n",blockGroupSize);
    //Nos movemos hasta el block group descriptor y leemos los 32 bits
    //high del bitmap inode y seguidamente los de menos peso

    moveThroughExt4(SEEK_SET,PADDING_BLOCKGROUP_DESCRIPTORS+0x24,BYTES_4,1,&upperInodeBitmap);
    moveThroughExt4(SEEK_SET,PADDING_BLOCKGROUP_DESCRIPTORS+0x28,BYTES_4,1,&upperInodeTable);
    moveThroughExt4(SEEK_SET,PADDING_BLOCKGROUP_DESCRIPTORS+0x4,BYTES_4,1,&lowerInodeBitmap);
    moveThroughExt4(SEEK_SET,PADDING_BLOCKGROUP_DESCRIPTORS+0x8,BYTES_4,1,&lowerInodeTable);

    //pasamos lo leido a la variable de 64 bits
    printf("DEBUG: UpperBitmap -%"PRIu32"-\n", upperInodeBitmap);
    printf("DEBUG: LowerBitmap -%"PRIu32"-\n", lowerInodeBitmap);
    inodeBitmap = upperInodeBitmap;
    printf("DEBUG: InodeBitmap -%"PRIu64"-\n", inodeBitmap);
    inodeBitmap = inodeBitmap << 32;
    printf("DEBUG: InodeBitmap -%"PRIu64"-\n", inodeBitmap);
    inodeBitmap = inodeBitmap | lowerInodeBitmap;
    printf("DEBUG: InodeBitmap -%"PRIu64"-\n", inodeBitmap);


    printf("DEBUG: UpperInodeTable -%"PRIu32"-\n", upperInodeTable);
    printf("DEBUG: LowerIndoeTable -%"PRIu32"-\n", lowerInodeTable);
    inodeTable = upperInodeTable;
    inodeTable = inodeTable << 32;
    inodeTable = inodeTable | lowerInodeTable;
    printf("DEBUG: InodeTable -%"PRIu64"-\n", inodeTable);

    uint16_t aux;
    printf("POSITIONS %"PRIu64"\n",blockSize*inodeTable);
    moveThroughExt4(SEEK_SET,blockSize*inodeTable,BYTES_2,1,&aux);
    printf("FIRST THING TO READ -%"PRIu16"-\n",aux);


}