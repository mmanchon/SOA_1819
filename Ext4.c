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

    if (has_extent > 0) {
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
    uint32_t lowerInodeTable, upperInodeTable, felx_bg;
    uint64_t /*inodeBitmap,*/ inodeTable ;
    DeepSearchExt4 ext4;
    // moveThroughExt4(SEEK_SET, PADDING_EXT4 + OFF_FEATURE_COMPAT, BYTES_4, 1, &feature_compat);
    // feature_compat = feature_compat & 0x10;
    // printf("FEATURE COMPAT HAS GDT BLOCKS -%"PRIu32"-\n", feature_compat);
    // moveThroughExt4(SEEK_SET, PADDING_EXT4 + 0xCE, BYTES_2, 1, &reservedGdtBlocks);
    //printf("RESERVED GDT BLOCKS -%"PRIu16"-\n", reservedGdtBlocks);
    moveThroughExt4(SEEK_SET, PADDING_EXT4 + 0x18, BYTES_4, MAX_NUM_LIST, &ext4.blockSize);
    printf("--------------------- SEARCH FILE EXT4 ---------------------- \n");
    printf("BLOCKSIZE: -%"PRIu32"-\n", ext4.blockSize);

    ext4.blockSize = pow(2, (10 + ext4.blockSize));
    printf("BLOCKSIZE: -%"PRIu32"-\n", ext4.blockSize);

    //leemos el tamaño del block group descriptors indicado en el superblock con offset de 0xFE
    moveThroughExt4(SEEK_SET, PADDING_EXT4 + OFF_BLOCKGROUP_SIZE, BYTES_2, 1, &ext4.blockGroupSize);
   // printf("SIZE OF BLOCK GROUP DESCRIPTOR -%"PRIu16"-\n", blockGroupSize);
    //Nos movemos hasta el block group descriptor y leemos los 32 bits
    //high del bitmap inode y seguidamente los de menos peso
    //moveThroughExt4(SEEK_SET, PADDING_BLOCKGROUP_DESCRIPTORS + 0x24, BYTES_4, 1, &upperInodeBitmap);
    moveThroughExt4(SEEK_SET, PADDING_BLOCKGROUP_DESCRIPTORS + 0x28, BYTES_4, 1, &upperInodeTable);
    // moveThroughExt4(SEEK_SET, PADDING_BLOCKGROUP_DESCRIPTORS + 0x4, BYTES_4, 1, &lowerInodeBitmap);
    moveThroughExt4(SEEK_SET, PADDING_BLOCKGROUP_DESCRIPTORS + 0x8, BYTES_4, 1, &lowerInodeTable);

    //pasamos lo leido a la variable de 64 bits
    /* printf("DEBUG: UpperBitmap -%"PRIu32"-\n", upperInodeBitmap);
     printf("DEBUG: LowerBitmap -%"PRIu32"-\n", lowerInodeBitmap);
     inodeBitmap = upperInodeBitmap;
     printf("DEBUG: InodeBitmap -%"PRIu64"-\n", inodeBitmap);
     inodeBitmap = inodeBitmap << 32;
     printf("DEBUG: InodeBitmap -%"PRIu64"-\n", inodeBitmap);
     inodeBitmap = inodeBitmap | lowerInodeBitmap;
     printf("DEBUG: InodeBitmap -%"PRIu64"-\n", inodeBitmap);
 */

    //printf("DEBUG: UpperInodeTable -%"PRIu32"-\n", upperInodeTable);
   //printf("DEBUG: LowerIndoeTable -%"PRIu32"-\n", lowerInodeTable);
    inodeTable = upperInodeTable;
    inodeTable = inodeTable << 32;
    inodeTable = inodeTable | lowerInodeTable;
    printf("DEBUG: InodeTable -%"PRIu64"-\n", inodeTable);
    printf("BLOCKSIZE: -%"PRIu32"-\n", ext4.blockSize);

    //leemos el inodes per group y inodes size
    moveThroughExt4(SEEK_SET, PADDING_EXT4 + 0x28, BYTES_4, MAX_NUM_LIST, &ext4.inodesPerGroup);
    moveThroughExt4(SEEK_SET, PADDING_EXT4 + 0x60, BYTES_4, MAX_NUM_LIST, &felx_bg);
    moveThroughExt4(SEEK_SET, PADDING_EXT4 + 0x58, BYTES_2, MAX_NUM_LIST, &ext4.inodeSize);
    printf("INODES PER GROUP: -%"PRIu32"-\n", ext4.inodesPerGroup);
    printf("INODE SIZE: -%"PRIu16"-\n", ext4.inodeSize);
    printf("BLOCKSIZE: -%"PRIu32"-\n", ext4.blockSize);
    printf("FLEX BG: -%"PRIu32"-\n", (felx_bg & 0x200));

    ext4.initInodeTable = ext4.blockSize * inodeTable;
    searchExtentTree(ext4);


}

int searchExtentTree(DeepSearchExt4 ext4) {
    int index = 0, offset = 0;
    uint16_t magicNumber;
    uint32_t aux;
        index = 1 % ext4.inodesPerGroup;
        offset = index * ext4.inodeSize;

        lseek(fd, ext4.initInodeTable + offset + OFF_EXTENT_TREE, SEEK_SET);
        read(fd, &magicNumber, sizeof(magicNumber));
        printf("------------ SEARCH EXTENT TREE ------------\n");
        lseek(fd, ext4.initInodeTable +0x20 +offset,SEEK_SET);
        read(fd, &aux, sizeof(aux));
        printf("FLAG: %"PRIu32"\n",(aux&0x80000));
        printf("MAGIC NUMBER %x\n", magicNumber);
        //Magic number para saber si es extent tree
        if (magicNumber == 0xF30A) {
            searchInfoExtent(ext4.initInodeTable + offset + OFF_EXTENT_TREE, ext4);

        }

    return 0;
}

int searchInfoExtent(uint64_t initExtentTree, DeepSearchExt4 ext4) {
    uint16_t numEntries;
    uint16_t depthExtentTree;

    printf("---------------SEARCH INFO EXTENT ------------\n");
    int i = 0;
    printf("INIT EXTENT TREE: %"PRIu64"\n", initExtentTree);
    moveThroughExt4(SEEK_SET, initExtentTree + 0x2, BYTES_2, 1, &numEntries);
    printf("NUM ENTRIES: %"PRIu16"\n",numEntries);
    moveThroughExt4(SEEK_SET, initExtentTree + 0x6, BYTES_2, 1, &depthExtentTree);
    printf("DEPTH: %"PRIu16"\n",depthExtentTree);

    if (depthExtentTree == 0) {
        for(i=0; i < numEntries; i++){
            infoLeaf(initExtentTree + 0xC * (i + 1), ext4);
        }
    }else{

        for(i=0; i < numEntries; i++){
            internalNodesExtentTree(initExtentTree + 0xC * (i+1), ext4);
        }
    }
    return 0;
}

void internalNodesExtentTree(uint64_t initNode, DeepSearchExt4 ext4){
    uint16_t upperExtentTree;
    uint32_t lowerExtentTree;
    uint64_t extentTree;
    moveThroughExt4(SEEK_SET, initNode + 0x4,BYTES_4,1,&lowerExtentTree);
    moveThroughExt4(SEEK_CUR,0,BYTES_2,1,&upperExtentTree);

    extentTree = upperExtentTree;
    extentTree = extentTree << 32;
    extentTree = extentTree | lowerExtentTree;

    searchInfoExtent(extentTree * ext4.blockSize,ext4);

}

void infoLeaf(uint64_t initLeaf, DeepSearchExt4 ext4) {
    uint16_t upperBlockNumber;
    uint32_t lowerBlockNumber;
    uint64_t blockAddress;
    uint16_t ee_len;
    moveThroughExt4(SEEK_SET,initLeaf + 0x4 , BYTES_2, 1, &ee_len);
    moveThroughExt4(SEEK_CUR, 0, BYTES_2, 1, &upperBlockNumber);
    moveThroughExt4(SEEK_CUR, 0, BYTES_4, 1, &lowerBlockNumber);

    blockAddress = upperBlockNumber;
    blockAddress = blockAddress << 32;
    blockAddress = blockAddress | lowerBlockNumber;
    printf("------------INFO LEAF ---------------------\n");
    printf("INIT LEAF: %"PRIu64"\n", initLeaf);
    printf("BLOCK ADRESS: %"PRIu64"\n", blockAddress);
    printf("BLOCK SIZE: -%"PRIu32"-\n", ext4.blockSize);
    readDirectoryInfo(blockAddress*ext4.blockSize,0,ee_len,ext4);
}

void readDirectoryInfo(uint64_t adress, int index, uint16_t ee_len, DeepSearchExt4 ext4) {
    char *name;
    ext4_dir_entry_2 dir;

    lseek(fd,adress,SEEK_SET);
    read(fd,&dir, sizeof(dir));
    printf("------------READ DIRECTORY ENTRY ---------------------\n");
    printf("ADRESS: %"PRIu64"\n", adress);
    printf("INODE: %"PRIu32"\n",dir.inode);
    printf("DIR LENGTH %"PRIu16"\n",dir.rec_len);
    printf("LENGTH: %"PRIu8"\n",dir.name_len);
    printf("TYPE: %"PRIu8"\n",dir.file_type);
    printf("EELEN: %"PRIu16"\n",ee_len);
    name = malloc(sizeof(dir.name_len));
    read(fd,name,sizeof(char)*dir.name_len);
    //name[strlen(name)] = '\0';
    printf("NAME %s\n", name);
    //
    if(dir.inode != 0) {

        if((dir.file_type & 0x2) > 0 && memcmp(name,".",sizeof(char)) != 0 && memcmp(name,"..",sizeof(char)*2) != 0){
            printf("ENTRO\n");
            searchInfoExtent(ext4.initInodeTable + ((dir.inode-1)%ext4.inodesPerGroup)*ext4.inodeSize + OFF_EXTENT_TREE, ext4);
        }
        readDirectoryInfo(adress + dir.rec_len,index+1,ee_len,ext4);
        free(name);

    }

}