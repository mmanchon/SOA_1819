//
// Created by sergi on 11/4/18.
//

#include "FileUtilities.h"

int exisitsFile(char *path){
    int fd;
    if((fd = open(path,O_RDONLY)) == -1){
        printf("Error en obrir el fitxer %s\n",path);
        exit(1);

    }

    return fd;

}


int detectFileSystemType(int fd){
    int ok;
    char buffer;

    ok = lseek(fd,OFFSET_SUPERBLOCK+OFFSET_MAGICNUMBER,SEEK_SET);

    if (ok < 0){
        printf("ERROR\n");
        return 0;
    }else{
        if(read(fd, &buffer, sizeof(char)) <= 0){
            printf("ERROR\n");
            return 0;
        }else{
            printf("BUFFER: %c\n",buffer);
            return 1;
        }
    }

}