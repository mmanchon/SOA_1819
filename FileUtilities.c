//
// Created by sergi on 11/4/18.
//

#include "FileUtilities.h"

int exisitsFile(char *path) {
    int fd;
    if ((fd = open(path, O_RDONLY)) == -1) {
        printf("Error en obrir el fitxer %s\n", path);
        exit(1);

    }

    return fd;

}


int detectFileSystemType(int fd) {
    int ok;
    FileSystem fileSystem;
    uint16_t buffer;
    ok = lseek(fd, PADDING_EXT4 + OFFSET_MAGICNUMBER, SEEK_SET);

    if (ok < 0) {
        printf("ERROR\n");
        return 0;
    } else {

        if (read(fd, &buffer, sizeof(uint16_t)) <= 0) {
            printf("ERROR\n");
            return 0;

        } else {

            printf("BUFFER: %X\n\n", buffer);

            if (buffer == MAGIC_NUMBER_EXT4) {

                ok = checkIfExt4(fd);

                if(ok)fileSystem = initSearchInfoExt4(fileSystem);

            }else{
                checkIfFat32(fd);
                fileSystem = initSearchInfoFat32(fileSystem);
                /*if(lseek(fd,3,SEEK_SET) < 0) {
                    printf("ERROR\n");
                }else{
                    char aux[8];
                    read(fd,aux, sizeof(char)*8);
                    printf("BUFFFEEEERR: %s\n",aux);
                }*/
            }
            return 1;
        }
    }

}