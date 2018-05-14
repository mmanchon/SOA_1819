/**
 *
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "FileUtilities.h"

int main(int argc, char **argv) {
    int fd = 0;
    int systemType = 0;
    FileSystem fileSystem;
    uint64_t fileInode;
    DeepSearchExt4 ext4;

    if (!argc) {
        printf(ARGUMENTS_NUMBER);

    } else {

        if (strcmp(argv[1], "-info") == 0) {
            fd = exisitsFile(argv[2]);
            systemType = detectFileSystemType(fd);

            if (systemType == 1) {
                fileSystem = initSearchInfoExt4(fileSystem);

            } else if (systemType == 2) {
                fileSystem = initSearchInfoFat32(fileSystem);

            }else{
                printf(NOT_FOUND);
            }
        } else if (strcmp(argv[1], "-search") == 0) {

            if (argc != 4) {
                printf(ARGUMENTS_NUMBER);

            } else {
                fd = exisitsFile(argv[2]);
                systemType = detectFileSystemType(fd);

                if (systemType == 1) {
                    searchFileExt4(argv[3], &ext4);

                } else if (systemType == 2) {
                    fileSystem = searchFileFat32(fileSystem, argv[3]);
                }else{
                    printf(NOT_FOUND);
                }
            }
        }else if (strcmp(argv[1], "-show") == 0){

            if(argc != 4){
                printf(ARGUMENTS_NUMBER);

            }else{
                fd = exisitsFile(argv[2]);
                systemType = detectFileSystemType(fd);

                if (systemType == 1) {
                    fileInode = searchFileExt4(argv[3], &ext4);

                    if(fileInode != 0)findFileInfo(fileInode,ext4);

                } else if (systemType == 2) {
                    showContentFileFat32(fileSystem,argv[3]);
                }else{
                    printf(NOT_FOUND);
                }
            }
        } else if (strcmp(argv[1], "-r") == 0){

            if(argc != 4){
                printf(ARGUMENTS_NUMBER);

            }else{
                fd = exisitsFile(argv[2]);
                systemType = detectFileSystemType(fd);

                if (systemType == 1) {
                    fileInode = searchFileExt4(argv[3], &ext4);

                    if(fileInode != 0)activateReadMode(fileInode,ext4);

                } else if (systemType == 2) {
                    activeReadModeFat32(fileSystem,argv[3]);
                }else{
                    printf(NOT_FOUND);
                }
            }
        }else if (strcmp(argv[1], "-w") == 0){

            if(argc != 4){
                printf(ARGUMENTS_NUMBER);

            }else{
                fd = exisitsFile(argv[2]);
                systemType = detectFileSystemType(fd);

                if (systemType == 1) {
                    fileInode = searchFileExt4(argv[3], &ext4);

                    if(fileInode != 0)activateReadMode(fileInode,ext4);

                } else if (systemType == 2) {
                    activeWriteModeFat32(fileSystem,argv[3]);
                }else{
                    printf(NOT_FOUND);
                }
            }
        }else if (strcmp(argv[1], "-h") == 0){

            if(argc != 4){
                printf(ARGUMENTS_NUMBER);

            }else{
                fd = exisitsFile(argv[2]);
                systemType = detectFileSystemType(fd);

                if (systemType == 1) {

                } else if (systemType == 2) {
                    activeHideModeFat32(fileSystem,argv[3]);
                }else{
                    printf(NOT_FOUND);
                }
            }
        }else if (strcmp(argv[1], "-s") == 0){

            if(argc != 4){
                printf(ARGUMENTS_NUMBER);

            }else{
                fd = exisitsFile(argv[2]);
                systemType = detectFileSystemType(fd);

                if (systemType == 1) {

                } else if (systemType == 2) {
                    desactivateHideModeFat32(fileSystem,argv[3]);
                }else{
                    printf(NOT_FOUND);
                }
            }
        }else if (strcmp(argv[1], "-d") == 0){

            if(argc != 4){
                printf(ARGUMENTS_NUMBER);

            }else{
                fd = exisitsFile(argv[2]);
                systemType = detectFileSystemType(fd);

                if (systemType == 1) {

                    fileInode = searchFileExt4(argv[4], &ext4);

                    uint32_t date = (uint32_t) argv[3];

                    if(fileInode != 0)changeDateFile(fileInode,ext4, date);
                } else if (systemType == 2) {
                    //activeReadModeFat32(fileSystem,argv[3]);
                }else{
                    printf(NOT_FOUND);
                }
            }
        }
    }

    return 0;
}