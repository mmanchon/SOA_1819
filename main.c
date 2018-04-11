/**
 *
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "FileUtilities.h"

void main(int argc, char **argv){
    int fd = 0;

    if(!argc || argc > 3){
        printf("Error\n");
    } else{

        if (memcmp(argv[1], "-info", sizeof(char) * strlen(argv[1])) == 0){
            fd = exisitsFile(argv[2]);
            detectFileSystemType(fd);
        }else{
            printf("Error\n");
        }
    }
}