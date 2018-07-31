#include "../include/Fat32.h"


/************************************ FASE1 ********************************/

void showInfoFat32(VolumenFat32 fat32) {
    printf(FS_INFO);
    printf(TYPE_FS, "FAT32");
    printf("\n");
    printf(SYSTEM_NAME, fat32.subFAT32.systemName);
    printf(SECTOR_SIZE, fat32.subFAT32.sectorSize);
    printf(SECTOR_CLUSTER, (fat32.subFAT32.sectorsPerCluster));
    printf(RESERVED_SECTORS, (fat32.subFAT32.reservedSectors));
    printf(NUMBER_FATS, (fat32.subFAT32.numberFat));
    printf(ROOT_ENTRIES, (fat32.subFAT32.numberEntries));
    printf(SECTORS_FAT, (fat32.sectorsPerFat));
    printf(LABEL, fat32.label);
}

VolumenFat32 getInfoFat32(VolumenFat32 fat32) {

    lseek(fd,OFF_NAME_SYS,SEEK_SET);
    read(fd,&fat32.subFAT32, sizeof(SubFAT32));

    moveThroughFat32(SEEK_SET, OFF_SECTORS_PER_FAT, BYTES_4, MAX_NUM_LIST, &fat32.sectorsPerFat);
    moveThroughFat32(SEEK_SET, OFF_LABEL, BYTES_11, MAX_NUM_LIST, fat32.label);

    fat32.label[11] = 0;

    moveThroughFat32(SEEK_SET, OFF_ROOT_DIR, BYTES_4, MAX_NUM_LIST, &fat32.rootCluster);

    return fat32;
}

void initSearchInfoFat32() {
    VolumenFat32 fat32;
    fat32 = getInfoFat32(fat32);
    showInfoFat32(fat32);
}


/************************************ FASE2 ********************************/


VolumeIdInfo getBasicInfoVolumeId(){
    VolumeIdInfo volumeIdInfo;

    lseek(fd,OFF_BYTES_PER_SEC,SEEK_SET);
    read(fd,&volumeIdInfo.BPB_BytsPerSec, sizeof(volumeIdInfo.BPB_BytsPerSec));

    //lseek(fd,NO_OFF,SEEK_CUR);
    read(fd,&volumeIdInfo.BPB_SecPerClus, sizeof(volumeIdInfo.BPB_SecPerClus));

    //lseek(fd,NO_OFF,SEEK_CUR);
    read(fd,&volumeIdInfo.BPB_RsvdSecCnt, sizeof(volumeIdInfo.BPB_RsvdSecCnt));

    //lseek(fd,NO_OFF,SEEK_CUR);
    read(fd,&volumeIdInfo.BPB_NumFATs, sizeof(volumeIdInfo.BPB_NumFATs));

    lseek(fd,OFF_SECTORS_PER_FAT,SEEK_SET);
    read(fd,&volumeIdInfo.BPB_FATSz32, sizeof(volumeIdInfo.BPB_FATSz32));

    lseek(fd,OFF_ROOT_DIR, SEEK_SET);
    read(fd,&volumeIdInfo.BPB_RootClus, sizeof(volumeIdInfo.BPB_RootClus));

    lseek(fd,OFF_SIGNATURE,SEEK_SET);
    read(fd,&volumeIdInfo.signature, sizeof(volumeIdInfo.signature));


#ifdef DEBUG
    DEBUG_PRINT(("GET BASIC INFO LINE 71\n"));
    DEBUG_PRINT((PRINT16,"BYTES PER SECTOR",volumeIdInfo.BPB_BytsPerSec));
    DEBUG_PRINT((PRINT8,"SECTOR PER CLUSTER",volumeIdInfo.BPB_SecPerClus));
    DEBUG_PRINT((PRINT16,"BPB_RsvdSecCnt",volumeIdInfo.BPB_RsvdSecCnt));
    DEBUG_PRINT((PRINT8,"BPB_NumFATs",volumeIdInfo.BPB_NumFATs));
    DEBUG_PRINT((PRINT32,"BPB_FATSz32",volumeIdInfo.BPB_FATSz32));
    DEBUG_PRINT((PRINT32,"BPB_RootClus",volumeIdInfo.BPB_RootClus));
    DEBUG_PRINT((PRINT16,"signature",volumeIdInfo.signature));
#endif
    return volumeIdInfo;
}


FATBasic calculateBasicFormulas(VolumeIdInfo volumeIdInfo){
    FATBasic fatBasic;

    fatBasic.fat_begin_lba = volumeIdInfo.BPB_RsvdSecCnt;
    fatBasic.cluster_begin_lba = volumeIdInfo.BPB_RsvdSecCnt + (volumeIdInfo.BPB_NumFATs * volumeIdInfo.BPB_FATSz32);
    fatBasic.sectors_per_cluster = volumeIdInfo.BPB_SecPerClus;
    fatBasic.root_dir_first_cluster = volumeIdInfo.BPB_RootClus;

#ifdef DEBUG
    DEBUG_PRINT(("CALCULATE BASIC FORMULAS LINE 93\n"));
    DEBUG_PRINT((PRINT32,"fat_begin_lba",fatBasic.fat_begin_lba));
    DEBUG_PRINT((PRINT32,"cluster_begin_lba",fatBasic.cluster_begin_lba));
    DEBUG_PRINT((PRINT8,"sectors_per_cluster",fatBasic.sectors_per_cluster));
    DEBUG_PRINT((PRINT32,"root_dir_first_cluster",fatBasic.root_dir_first_cluster));
#endif


    return fatBasic;

}


int searchFileInFAT32(FATBasic fatBasic, UINT32 lba_adrr){
    FAT32Dir fat32Dir;
    char *firstByte;
    lseek(fd,(long)lba_adrr*FAT32_SIZE,SEEK_SET);
    read(fd, &fat32Dir, sizeof(fat32Dir));

    firstByte = fat32Dir.DIR_Name;

#ifdef DEBUG
    DEBUG_PRINT(("SEARCH FILE IN FAT 32\n"));
    DEBUG_PRINT((PRINT32,"LBA_ADDR",lba_adrr));
    DEBUG_PRINT(("DIR_NAME: -%s-\n",fat32Dir.DIR_Name));
    DEBUG_PRINT((PRINT8,"DIR_Name",*firstByte));
    DEBUG_PRINT(("DIR_EXTENSION: -%s-\n",fat32Dir.DIR_Extension));
    DEBUG_PRINT((PRINT8,"DIR_Attr",fat32Dir.DIR_Attr));
    DEBUG_PRINT((PRINT16,"DIR_FstClusHI",fat32Dir.DIR_FstClusHI));
    DEBUG_PRINT((PRINT16,"DIR_FstClusLO",fat32Dir.DIR_FstClusLO));
    DEBUG_PRINT((PRINT32,"DIR_FileSize",fat32Dir.DIR_FileSize));
#endif

    switch (*firstByte){
        case 0x05:
        case 0xE5:
            searchFileInFAT32(fatBasic,lba_adrr+32);
            break;
        case 0x00:
            return 0;
            break;
        case 0x2e:
            break;
        default:
            break;
    }

    return 1;

}




/**********************************FAT32 UTILITIES **************************/

void moveThroughFat32(int whence, off_t offset, int bytes, int numArg, ...) {
    //Creamos una lista para los argumentos no definidos '...'
    va_list valist;

    //Constante MAX_NUM_LIST se encuentra en EXT4.h
    //Inicializamos la lista a 1 posicion
    va_start(valist, numArg);

    //Comprovamos que nos hemos podido desplazar el offset correspondiente
    if (lseek(fd, offset, whence) != -1) {
        //Segun la variable bytes leeremos un tipo de variables o otro
        if (bytes == BYTES_4) {
            uint32_t *aux = va_arg(valist, uint32_t *);
            if (read(fd, aux, sizeof(uint32_t)) <= 0) {
                printf("Erro. La lectura de 4byte na ha sido posible\n");
                exit(1);
            }
        } else if (bytes == BYTES_2) {
            char *aux = va_arg(valist, char *);
            if (read(fd, aux, sizeof(char) * 2) <= 0) {
                printf("Erro. La lectura de 2byte na ha sido posible\n");
                exit(1);
            }
        } else if (bytes == BYTES_1) {
            char *aux = va_arg(valist, uint8_t *);
            if (read(fd, aux, sizeof(uint8_t)) <= 0) {
                printf("Erro. La lectura de 1 byte na ha sido posible\n");
                exit(1);
            }
        }else if (bytes == BYTES_1_V2) {
            char *aux = va_arg(valist, uint8_t *);
            if (read(fd, aux, sizeof(uint8_t)) <= 0) {
                printf("File is empty.\n");
                exit(1);
            }
        }else if (bytes == BYTES_8) {
            char *aux = va_arg(valist, char *);
            if (read(fd, aux, sizeof(char) * 8) <= 0) {
                printf("Erro. La lectura de 8byte na ha sido posible\n");
                exit(1);
            }

        } else if (bytes == BYTES_11) {
            char *aux = va_arg(valist, char *);
            if (read(fd, aux, sizeof(char) * 11) <= 0) {
                printf("Erro. La lectura de 11byte na ha sido posible\n");
                exit(1);
            }
        } else if (bytes == BYTES_12) {
            char *aux = va_arg(valist, char *);
            if (read(fd, aux, sizeof(char) * 12) <= 0) {
                printf("Erro. La lectura de 12byte na ha sido posible\n");
                exit(1);
            }
        } else if (bytes == BYTES_10) {
            char *aux = va_arg(valist, char *);
            if (read(fd, aux, sizeof(char) * 10) <= 0) {
                printf("Erro. La lectura de 10byte na ha sido posible\n");
                exit(1);
            }
        }
    } else {
        printf("Error. El lseek no ha funcionado\n");
        exit(1);
    }
    va_end(valist);
}

int checkIfFat32(int file) {
    fd = file;
    char aux[8];
    moveThroughFat32(SEEK_SET, OFF_FAT_TYPE, BYTES_8, 1, aux);


    if (memcmp(aux, "FAT32", sizeof(char) * 5) == 0) {
        return 1;

    } else {

        moveThroughFat32(SEEK_SET, OFF_FAT_TYPE_2, BYTES_8, 1, aux);

        if (memcmp(aux, "FAT12", sizeof(char) * 5) == 0) {
            printf(NOT_RECOGNIZED, "FAT12");
            exit(1);
        } else if (memcmp(aux, "FAT16", sizeof(char) * 5) == 0) {
            printf(NOT_RECOGNIZED, "FAT16");
            exit(1);
        } else {
            printf(NOT_FOUND);
            exit(1);
        }
    }
}