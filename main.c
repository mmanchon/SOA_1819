/**
 *
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "FileUtilities.h"

int main(int argc, char **argv){
    int fd = 0;
    int fd_file = 0;
    int systemType = 0;
    FileSystem fileSystem;

    if(!argc){
        printf("Error\n");
    } else{

        if (memcmp(argv[1], "-info", sizeof(char) * strlen(argv[1])) == 0){
            fd = exisitsFile(argv[2]);
            systemType = detectFileSystemType(fd);
            if(systemType == 1){
                fileSystem = initSearchInfoExt4(fileSystem);
            }else if(systemType == 2){
                fileSystem = initSearchInfoFat32(fileSystem);
            }
        }else if(memcmp(argv[1], "-search", sizeof(char) * strlen(argv[1])) == 0){
            fd = exisitsFile(argv[2]);
            systemType = detectFileSystemType(fd);
            if(systemType == 1){
                if(argc == 4) {
                    fd_file = exisitsFile(argv[3]);
                    printf("hola tete\n");
                    //...
                }
            }else if(systemType == 2){
                if(argc == 4) {
                    fd_file = exisitsFile(argv[3]);
                    //...
                }
            }
            printf("End without errors!\n");
        }
    }

    return 0;
}