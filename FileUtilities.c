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
    FileSystem fileSystem;
    //char buffer[2];
    //long buffer = 0;
    uint16_t buffer;
    ok = lseek(fd,PADDING_EXT4 + OFFSET_MAGICNUMBER,SEEK_SET);
    if (ok < 0){
        printf("ERROR\n");
        return 0;
    }else{
        if(read(fd, &buffer, sizeof(uint16_t)) <= 0){
            printf("ERROR\n");
            return 0;
        }else{
            printf("BUFFER: %X\n\n",buffer);
            if(buffer == MAGIC_NUMBER_EXT4){
                printf("Estamos leyendo un FS ext4\n");
                fileSystem = initSearchInfoExt4(fd);
            }
            return 1;
        }
    }

}