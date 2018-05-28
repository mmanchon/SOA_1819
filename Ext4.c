//
// Created by sergi on 14/4/18.
//

#include "Ext4.h"

/**
 * Función para mostrar la información de la Fase1
 * @param ext4 Tipo que contiene la información
 */
void showInfoExt4(VolumenExt4 ext4) {

    printf(FS_INFO);
    printf(TYPE_FS, "EXT4");
    printf("\n");

    //Mostramos por pantalla la información de los inodos
    printf(INODE_INFO);
    printf(INODE_SIZE, ext4.inodeSize);
    printf(NUMBER_INODES, ext4.inodesCount);
    printf(FIRST_INODE, ext4.firstInode);
    printf(INODES_GROUP, ext4.inodesGroup);
    printf(FREE_INODES, ext4.freeInodesCount);

    //mostramos por pantalla la información de los bloques
    printf(BLOCK_INFO);
    printf(BLOCK_SIZE, pow(2, (10 + ext4.blockSize)));
    printf(RESERVED_BLOCK, ext4.reservedBlocksCount);
    printf(FREE_BLOCKS, ext4.freeBlockCount);
    printf(TOTAL_BLOCKS, ext4.blockCount);
    printf(FIRST_BLOCK, ext4.firstDataBlock);
    printf(BLOCK_GROUP, ext4.blocksGroup);
    printf(FRAGS_GROUP, ext4.fragsGroup);

    //mostramos por pantalla la información de los volumenes
    printf(VOLUME_INFO);
    printf(VOLUME_NAME, ext4.volumeName);
    printf(LAST_CHECK_VOL, asctime(getTime(ext4.lastCheck)));
    printf(LAST_MOUNT_VOL, asctime(getTime(ext4.lastMounted)));
    printf(LAST_WRITTEN, asctime(getTime(ext4.lastWritten)));

}

struct tm* getTime(uint32_t time) {
    time_t rawTime;
    struct tm *timeInfo;

    rawTime = (time_t) time;
    timeInfo = localtime(&rawTime);

    return timeInfo;
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
        //en caso de haber algun problema en el momento de leer salimos
        //Segun que cuantos bytes haya que leer leemos un sizeof() diferente

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
    uint32_t feature_compat, feature_incompat;
    int has_extent, has_journal;

    //variable global
    fd = file;

    //vamos a las posiciones necesarias para leer feature_compat y feature_incompat
    //estas variables nos diran lo necesario para saber si es
    moveThroughExt4(SEEK_SET, PADDING_EXT4 + OFF_FEATURE_COMPAT, BYTES_4, 1, &feature_compat);
    moveThroughExt4(SEEK_SET, PADDING_EXT4 + OFF_FEATURE_INCOMPAT, BYTES_4, 1, &feature_incompat);

    //hacemos AND logicas para comprovar si ese bit esta puesto a 1 o a 0
    has_extent = feature_incompat & 0x40;
    has_journal = feature_compat & 0x4;

    //en caso de que tenga extent sabemos que es ext4
    if (has_extent > 0) {
        return 1;
        // Sabemos que si no tiene extent pero tiene jounral entonces es ext3
    } else if (has_journal) {
        printf(NOT_RECOGNIZED, "EXT3");
        exit(1);
    } else {
        //en caso contrario a todos los demas es ext2
        printf(NOT_RECOGNIZED, "EXT2");
        exit(1);
    }
    return 0;
}


uint64_t searchFileExt4(char *file, DeepSearchExt4 *ext4) {
    uint32_t lowerInodeTable = 0, upperInodeTable = 0, felx_bg = 0;
    uint64_t inodeTable = 0, fileInode = 0;

    //guardamos el nombre del fichero en el typedef
    ext4->file = file;

    //Con las siguientes funciones rellenamos la variable DeepSearchExt4 para
    //tener la información necesaria y hacer recursividad

    moveThroughExt4(SEEK_SET, PADDING_EXT4 + 0x18, BYTES_4, MAX_NUM_LIST, &(ext4->blockSize));
    moveThroughExt4(SEEK_SET, PADDING_EXT4 + 0x28, BYTES_4, MAX_NUM_LIST, &(ext4->inodesPerGroup));
    moveThroughExt4(SEEK_SET, PADDING_EXT4 + 0x60, BYTES_4, MAX_NUM_LIST, &felx_bg);
    moveThroughExt4(SEEK_SET, PADDING_EXT4 + 0x58, BYTES_2, MAX_NUM_LIST, &(ext4->inodeSize));
    moveThroughExt4(SEEK_SET, PADDING_EXT4 + 0x60, BYTES_4, MAX_NUM_LIST, &(ext4->incompat));
    moveThroughExt4(SEEK_SET, PADDING_EXT4 + 0xFE, BYTES_2, MAX_NUM_LIST, &(ext4->desc_size));
    ext4->blockSize = pow(2, (10 + ext4->blockSize));
    moveThroughExt4(SEEK_SET, PADDING_EXT4 + OFF_BLOCKGROUP_SIZE, BYTES_2, 1, &(ext4->blockGroupSize));

    /*
        leemos la posicion de la tabla de inodos
        distinguimos entre 1024 y el resto porque
        el superbloque esta despues de un padding de 1024 bytes
        en el caso de un blockSize de 1024 eso sera el 2n bloque
        mientras que para el resto seguira siendo el primero
     */

    if (ext4->blockSize == 1024) {
        moveThroughExt4(SEEK_SET, PADDING_BLOCKGROUP_DESCRIPTORS + 0x28, BYTES_4, 1, &upperInodeTable);
        moveThroughExt4(SEEK_SET, PADDING_BLOCKGROUP_DESCRIPTORS + 0x8, BYTES_4, 1, &lowerInodeTable);

    } else {
        moveThroughExt4(SEEK_SET, ext4->blockSize + 0x28, BYTES_4, 1, &upperInodeTable);
        moveThroughExt4(SEEK_SET, ext4->blockSize + 0x8, BYTES_4, 1, &lowerInodeTable);

    }


    //shiftamos 32 bits la tabla y hacemos una OR logica para añadir los bits de menos peso
    if((ext4->incompat & 0x80) > 0 && ext4->desc_size > 32){
        inodeTable = upperInodeTable;
        inodeTable = inodeTable << 32;
        inodeTable = inodeTable | lowerInodeTable;
    }else{
        inodeTable = lowerInodeTable;
    }

    //El inicio de la tabla de inodos se encuentra en la posicion leida por el tamaño de bloque
    ext4->initInodeTable = ext4->blockSize * inodeTable;

    //Buscamos el inicio del extent tree
    fileInode = searchExtentTree(ext4);

    return fileInode;
}

uint64_t searchExtentTree(DeepSearchExt4 *ext4) {
    int index = 0, offset = 0;
    uint16_t magicNumber = 0;
    uint64_t fileInode = 0;

    //formula para saber donde se encuentra el inodo correspondiente (empezamos por el raiz)
    // blockGroup = (inode_num -1) / sb->s_inodes_per_group
    // index = (inode_num -1) % sb->s_inodes_per_group
    // offset = index * sb->s_inode_size

    index = 1 % ext4->inodesPerGroup;
    offset = index * ext4->inodeSize;

    //leemos el magic number para comprobar que hay un extent tree
    lseek(fd, ext4->initInodeTable + offset + OFF_EXTENT_TREE, SEEK_SET);
    read(fd, &magicNumber, sizeof(magicNumber));

    //leemos el flag necesario para la fase 4
    //Al final no ha sido necesario, este flag nos indica que se pueden
    //almacenar ficheros muy grandes
    //Magic number para saber si es extent tree
    if (magicNumber == 0xF30A) {

        //vamos a ver la información del extent tree
        fileInode = searchInfoExtent(ext4->initInodeTable + offset + OFF_EXTENT_TREE, ext4);

    }

    return fileInode;
}

uint64_t searchInfoExtent(uint64_t initExtentTree, DeepSearchExt4 *ext4) {
    uint16_t numEntries = 0, depthExtentTree = 0;
    uint64_t fileInode = 0;
    int i = 0;

    //leemos la informacion del numero de entradas validas y el depth del extent tree
    moveThroughExt4(SEEK_SET, initExtentTree + 0x2, BYTES_2, 1, &numEntries);
    moveThroughExt4(SEEK_SET, initExtentTree + 0x6, BYTES_2, 1, &depthExtentTree);

    if (depthExtentTree == 0) {

        //en caso que el depth sea igual a 0 entonces es hoja, se mira por cada una de las hojas
        for (i = 0; i < numEntries; i++) {

            //avanzamos de 12 bytes en 12 bytes para leer los diferente bloques
            if ((fileInode = infoLeaf(initExtentTree + 0xC * (i + 1), ext4)) > 0) {
                return fileInode;
            }

        }

    } else {

        //en caso de que sea diferente a 0 tenemos un extent tree
        for (i = 0; i < numEntries; i++) {

            //avanzamos de 12 bytes en 12 bytes para leer los diferentes bloques
            if ((fileInode = internalNodesExtentTree(initExtentTree + 0xC * (i + 1), ext4)) > 0) {
                return fileInode;
            }
        }
    }

    return fileInode;
}

uint64_t internalNodesExtentTree(uint64_t initNode, DeepSearchExt4 *ext4) {
    uint16_t upperExtentTree = 0;
    uint32_t lowerExtentTree = 0;
    uint64_t extentTree = 0;

    moveThroughExt4(SEEK_SET, initNode + 0x4, BYTES_4, 1, &lowerExtentTree);
    moveThroughExt4(SEEK_CUR, 0, BYTES_2, 1, &upperExtentTree);

    extentTree = upperExtentTree;
    extentTree = extentTree << 32;
    extentTree = extentTree | lowerExtentTree;

    return searchInfoExtent(extentTree * ext4->blockSize, ext4);

}

uint64_t infoLeaf(uint64_t initLeaf, DeepSearchExt4 *ext4) {
    uint16_t upperBlockNumber = 0;
    uint32_t lowerBlockNumber = 0;
    uint16_t ee_len = 0;
    uint64_t fileInode = 0;

    //leemos los bloques que ocupa el directory y la direccion
    moveThroughExt4(SEEK_SET, initLeaf + 0x4, BYTES_2, 1, &ee_len);
    moveThroughExt4(SEEK_CUR, 0, BYTES_2, 1, &upperBlockNumber);
    moveThroughExt4(SEEK_CUR, 0, BYTES_4, 1, &lowerBlockNumber);

    ext4->blockAddress = upperBlockNumber;
    ext4->blockAddress = ext4->blockAddress << 32;
    ext4->blockAddress = ext4->blockAddress | lowerBlockNumber;

    fileInode = readDirectoryInfo(ext4->blockAddress * ext4->blockSize, 0, ee_len, ext4);

    return fileInode;
}

uint64_t readDirectoryInfo(uint64_t adress, int index, uint16_t ee_len, DeepSearchExt4 *ext4) {
    char *name;
    ext4_dir_entry_2 dir;
    uint64_t fileInode = 0;

    lseek(fd, adress, SEEK_SET);
    read(fd, &dir, sizeof(dir));

    if (dir.inode != 0 && ((ee_len * ext4->blockSize) + (ext4->blockSize * ext4->blockAddress)) > (adress)) {

        name = calloc(dir.name_len, sizeof(char));
        read(fd, name, sizeof(char) * dir.name_len);
          if ((dir.file_type & 0x2) > 0 && strcmp(name, ".") != 0 &&
            strcmp(name, "..") != 0) {

            if ((fileInode = searchInfoExtent(
                    ext4->initInodeTable + ((dir.inode - 1) % ext4->inodesPerGroup) * ext4->inodeSize + OFF_EXTENT_TREE,
                    ext4)) == 0) {

                if ((fileInode = checkFile(dir, ext4, name)) == 0) {
                    fileInode = readDirectoryInfo(adress + dir.rec_len, index + 1, ee_len, ext4);
                }

            }
        } else {

            if ((fileInode = checkFile(dir, ext4, name)) == 0) {
                fileInode = readDirectoryInfo(adress + dir.rec_len, index + 1, ee_len, ext4);
            }

        }

        free(name);
    }
    return fileInode;

}

uint64_t checkFile(ext4_dir_entry_2 dir, DeepSearchExt4 *ext4, char *name) {
    uint32_t date, lowerFileSize, upperFileSize;
    uint64_t fileSize;

    if ((dir.file_type & 0x1) > 0) {
        if (strcmp(ext4->file, name) == 0) {

            moveThroughExt4(SEEK_SET,
                            ext4->initInodeTable + ((dir.inode - 1) % ext4->inodesPerGroup) * ext4->inodeSize +
                            OFF_FILE_DATE,
                            BYTES_4,
                            1,
                            &date);

            moveThroughExt4(SEEK_SET,
                            ext4->initInodeTable + ((dir.inode - 1) % ext4->inodesPerGroup) * ext4->inodeSize + 0x4,
                            BYTES_4,
                            1,
                            &lowerFileSize);

            moveThroughExt4(SEEK_SET,
                            ext4->initInodeTable + ((dir.inode - 1) % ext4->inodesPerGroup) * ext4->inodeSize + 0x6c,
                            BYTES_4,
                            1,
                            &upperFileSize);

            fileSize = upperFileSize;
            fileSize = fileSize << 32;
            fileSize = fileSize | lowerFileSize;

            ext4->fileSize = fileSize;
            ext4->dateFile = date;

            return ext4->initInodeTable + ((dir.inode - 1) % ext4->inodesPerGroup) * ext4->inodeSize;
        }
    }

    return 0;
}


void findFileInfo(uint64_t fileInode, DeepSearchExt4 ext4) {
    uint32_t lowerFileFize, upperFileSize;

    moveThroughExt4(SEEK_SET, fileInode + 0x4, BYTES_4, 1, &lowerFileFize);
    moveThroughExt4(SEEK_SET, fileInode + 0x6C, BYTES_4, 1, &upperFileSize);

    ext4.fileSize = upperFileSize;
    ext4.fileSize = ext4.fileSize << 32;
    ext4.fileSize = ext4.fileSize | lowerFileFize;
    findExtentTreeInfo(fileInode + 0x28, ext4, 0);
}

uint64_t findExtentTreeInfo(uint64_t offset, DeepSearchExt4 ext4, uint64_t numRead) {
    uint16_t numEntries, depthExtentTree;
    int i = 0;

    //leemos la informacion del numero de entradas validas y el depth del extent tree
    moveThroughExt4(SEEK_SET, offset + 0x2, BYTES_2, 1, &numEntries);
    moveThroughExt4(SEEK_SET, offset + 0x6, BYTES_2, 1, &depthExtentTree);
    if (depthExtentTree == 0) {
        //en caso que el depth sea igual a 0 entonces es hoja, se mira por cada una de las hojas

        for (i = 0; i < numEntries; i++) {
            numRead = fileLeaf(offset + 0xC * (i + 1), ext4, numRead);
        }
    } else {
        //en caso de que sea diferente a 0 tenemos un extent tree
        for (i = 0; i < numEntries; i++) {
            numRead = internalFileNodes(offset + 0xC * (i + 1), ext4, numRead);

        }
    }
    return numRead;
}

uint64_t fileLeaf(uint64_t initLeaf, DeepSearchExt4 ext4, uint64_t numRead) {
    uint16_t upperBlockNumber, ee_len;
    uint32_t lowerBlockNumber;

    moveThroughExt4(SEEK_SET, initLeaf + 0x4, BYTES_2, 1, &ee_len);
    moveThroughExt4(SEEK_CUR, 0, BYTES_2, 1, &upperBlockNumber);
    moveThroughExt4(SEEK_CUR, 0, BYTES_4, 1, &lowerBlockNumber);
    ext4.blockAddress = upperBlockNumber;
    ext4.blockAddress = ext4.blockAddress << 32;
    ext4.blockAddress = ext4.blockAddress | lowerBlockNumber;

    return showInfoFile(ext4.blockAddress * ext4.blockSize, ee_len, ext4, numRead);
}

uint64_t internalFileNodes(uint64_t initNode, DeepSearchExt4 ext4, uint64_t numRead) {
    uint16_t upperExtentTree;
    uint32_t lowerExtentTree;
    uint64_t extentTree;

    moveThroughExt4(SEEK_SET, initNode + 0x4, BYTES_4, 1, &lowerExtentTree);
    moveThroughExt4(SEEK_CUR, 0, BYTES_2, 1, &upperExtentTree);

    extentTree = upperExtentTree;
    extentTree = extentTree << 32;
    extentTree = extentTree | lowerExtentTree;

    return findExtentTreeInfo(extentTree * ext4.blockSize, ext4, numRead);
}


uint64_t showInfoFile(uint64_t offset, uint16_t ee_len, DeepSearchExt4 ext4, uint64_t numRead) {
    uint64_t i = 0;
    char c;


    lseek(fd, offset, SEEK_SET);
    while ((i < (ee_len * ext4.blockSize)) && (numRead < ext4.fileSize)) {
        read(fd, &c, sizeof(char));
        printf("%c", c);
        i++;
        numRead++;
    }
    return numRead;
}

void activateReadMode(uint64_t offset) {
    uint16_t mode;

    moveThroughExt4(SEEK_SET, offset, BYTES_2, 1, &mode);

    lseek(fd, offset, SEEK_SET);

    mode = mode & 0xFF24; // 111 1111 1111 0010 0100
    mode = mode | 0x124; //0000 0001 0010 0100

    write(fd, &mode, sizeof(mode));

}

void deactivateReadMode(uint64_t offset) {
    uint16_t mode;

    moveThroughExt4(SEEK_SET, offset, BYTES_2, 1, &mode);

    lseek(fd, offset, SEEK_SET);

    mode = mode & 0xFFB6; // 111 1111 1111 1011 0110

    mode = mode | 0x1B6; //0000 0001 1011 0110

    write(fd, &mode, sizeof(mode));

}

void changeDateFile(uint64_t offset, char* date) {
    struct tm time;
    char *token;
    int dateTime[3], i = 0;
    time_t tepoch;
    uint32_t finalDate;

    token = strtok(date,"/");

    while(token != NULL){
        dateTime[i] = atoi(token);
        token = strtok(NULL, "/");
        i++;
    }

    time.tm_year = dateTime[2] - 1900;
    time.tm_mon = dateTime[1];
    time.tm_mday = dateTime[0];

    tepoch = mktime(&time);
    finalDate = (uint32_t) tepoch;

    lseek(fd, offset + 0x90, SEEK_SET);
    write(fd, &finalDate, sizeof(finalDate));
}