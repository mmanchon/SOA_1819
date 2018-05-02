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

    uint16_t buffer;

    if (lseek(fd, PADDING_EXT4 + OFFSET_MAGICNUMBER, SEEK_SET) < 0) {
       // printf("ERROR\n");
        return 0;
    } else {

        if (read(fd, &buffer, sizeof(uint16_t)) <= 0) {
            //printf("ERROR\n");
            return 0;

        } else {

            if (buffer == MAGIC_NUMBER_EXT4) {
                checkIfExt4(fd);
                return 1;
            } else if (checkIfFat32(fd)) {
                return 2;
            }
            return 0;
        }
    }

}