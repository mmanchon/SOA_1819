//
// Created by sergi on 15/4/18.
//

#include "Fat32.h"

//http://www.file-recovery.com/recovery-understanding-file-system-fat.htm
//https://en.wikipedia.org/wiki/Design_of_the_FAT_file_system
//http://www.c-jump.com/CIS24/Slides/FAT/lecture.html
void showInfoFat32(VolumenExt4 ext4){
    printf(FS_INFO);
    printf(TYPE_FS,"FAT32");

}

FileSystem initSearchInfoFat32(){

}

void moveThroughFat32(int whence,off_t offset,int bytes, int numArg, ...){

}

void checkIfFat32(){

}