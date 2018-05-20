
#include "FileUtilities.h"


int checkFileSystem(char *volume){
    int fd = 0;

    fd = exisitsFile(volume);

    return detectFileSystemType(fd);
}


int exisitsFile(char *path) {
    int fd;

    if ((fd = open(path, O_RDWR)) == -1) {
        printf("Error en obrir el fitxer %s\n", path);
        exit(1);

    }

    return fd;

}


int detectFileSystemType(int fd) {

    uint16_t buffer;

    //Comprovamos si nos podemos mover hasta la posicion deseada
    if (!(lseek(fd, PADDING_EXT4 + OFFSET_MAGICNUMBER, SEEK_SET) < 0)) {

        //leemos el file magic number
        if (!(read(fd, &buffer, sizeof(uint16_t)) <= 0)) {

            //en caso de haber magic numbre estamos en un FS de tipo EXT
            if (buffer == MAGIC_NUMBER_EXT4) {
                //miramos que ext es
                checkIfExt4(fd);
                return 1;
                //miramos si es FAT32
            } else if (checkIfFat32(fd)) {
                return 2;
            }
        }
    }

    return 0;

}

void infoFS(char *volume){
    FileSystem fileSystem;

    switch(checkFileSystem(volume)){
        case 1:
            fileSystem = initSearchInfoExt4(fileSystem);

            break;
        case 2:
            fileSystem = initSearchInfoFat32(fileSystem);

            break;
        default:
            printf(NOT_FOUND);
            break;
    }

}

void searchFileFS(char *volume, char *file){
    FileSystem fileSystem;
    uint64_t fileInode;
    DeepSearchExt4 ext4;
    struct tm *time;

    switch(checkFileSystem(volume)){
        case 1:

            fileInode = searchFileExt4(file, &ext4);

            if(!fileInode){
                printf(FILE_NOT_FOUND);
            }else{
                time = getTime(ext4.dateFile);
                printf(FILE_FOUND, ext4.fileSize, time->tm_mday,time->tm_mon,(time->tm_year +1900));
            }

            break;
        case 2:
            fileSystem = searchFileFat32(fileSystem, file);

            break;
        default:
            printf(NOT_FOUND);
            break;
    }

}

void showFileFS(char *volume, char *file){
    FileSystem fileSystem;
    uint64_t fileInode;
    DeepSearchExt4 ext4;

    switch(checkFileSystem(volume)){
        case 1:
            fileInode = searchFileExt4(file, &ext4);

            if(fileInode != 0){
                printf(SHOWING_CONTENT);
                findFileInfo(fileInode,ext4);

            }else{
                printf(FILE_NOT_FOUND);

            }

            break;
        case 2:
            showContentFileFat32(fileSystem,file);

            break;
        default:
            printf(NOT_FOUND);
            break;
    }

}

void onlyReadMode(char *volume, char *file){
    FileSystem fileSystem;
    uint64_t fileInode;
    DeepSearchExt4 ext4;

    switch(checkFileSystem(volume)){
        case 1:
            fileInode = searchFileExt4(file, &ext4);

            if(fileInode != 0){
                activateReadMode(fileInode);
                printf(CHANGE_RWX,file);
            }else{
                printf(FILE_NOT_FOUND);

            }
            break;
        case 2:
            activeReadModeFat32(fileSystem,file);

            break;
        default:
            printf(NOT_FOUND);
            break;
    }

}

void notOnlyReadMode(char *volume, char *file){
    FileSystem fileSystem;
    uint64_t fileInode;
    DeepSearchExt4 ext4;

    switch(checkFileSystem(volume)){
        case 1:
            fileInode = searchFileExt4(file, &ext4);

            if(fileInode != 0){
                deactivateReadMode(fileInode);
                printf(CHANGE_RWX,file);
            }else{
                printf(FILE_NOT_FOUND);

            }
            break;
        case 2:
            activeWriteModeFat32(fileSystem,file);

            break;
        default:
            printf(NOT_FOUND);
            break;
    }


}

void hiddenFile(char *volume, char *file){
    FileSystem fileSystem;

    switch(checkFileSystem(volume)){
        case 1:
            printf(NOT_ALLOWED);

            break;
        case 2:
            activeHideModeFat32(fileSystem,file);

            break;
        default:
            printf(NOT_FOUND);
            break;
    }

}

void unHiddenFile(char *volume, char *file){
    FileSystem fileSystem;

    switch(checkFileSystem(volume)){
        case 1:
            printf(NOT_ALLOWED);

            break;
        case 2:
            desactivateHideModeFat32(fileSystem,file);

            break;
        default:
            printf(NOT_FOUND);
            break;
    }

}

void changeFileDate(char *volume, char *file, char* date){
    FileSystem fileSystem;
    uint64_t fileInode;
    DeepSearchExt4 ext4;

    switch(checkFileSystem(volume)){
        case 1:
            fileInode = searchFileExt4(file, &ext4);

            if(fileInode != 0){
                changeDateFile(fileInode, date);
                printf(CHANGE_DATE,file);

            }else{
                printf(FILE_NOT_FOUND);

            }
            break;
        case 2:
            newDateFat32(fileSystem,file,date);
            break;
        default:
            printf(NOT_FOUND);
            break;
    }

}