//
// Created by sergi on 13/4/18.
//

#ifndef RAGNAROK_TYPES_H
#define RAGNAROK_TYPES_H

typedef struct {
    //Inodes Info
    unsigned long inodesCount;
    unsigned long freeInodesCount;
    unsigned long inodesGroup;

    //Block info
    unsigned long blockCount;
    unsigned long freeBlockCount;
    unsigned long reservedBlocksCount;
    unsigned long firstDataBlock;
    unsigned long blocksGroup;

}VolumenExt4;

typedef struct {

}VolumenFat32;
#endif //RAGNAROK_TYPES_H
