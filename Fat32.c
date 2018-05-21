#include "Fat32.h"

void showInfoFat32(VolumenFat32 fat32) {
    printf(FS_INFO);
    printf(TYPE_FS, "FAT32");
    printf("\n");
    printf(SYSTEM_NAME, fat32.systemName);
    printf(SECTOR_SIZE, fat32.sectorSize);
    printf(SECTOR_CLUSTER, (fat32.sectorsPerCluster));
    printf(RESERVED_SECTORS, (fat32.reservedSectors));
    printf(NUMBER_FATS, (fat32.numberFat));
    printf(ROOT_ENTRIES, (fat32.numberEntries));
    printf(SECTORS_FAT, (fat32.sectorsPerFat));
    printf(LABEL, fat32.label);

}


FileSystem getInfoFat32(FileSystem fileSystem) {
    moveThroughFat32(SEEK_SET, OFF_NAME_SYS, BYTES_8, MAX_NUM_LIST, fileSystem.fat32.systemName);
    moveThroughFat32(SEEK_CUR, NO_OFF, BYTES_2, MAX_NUM_LIST, &fileSystem.fat32.sectorSize);
    moveThroughFat32(SEEK_CUR, NO_OFF, BYTES_1, MAX_NUM_LIST, &fileSystem.fat32.sectorsPerCluster);
    moveThroughFat32(SEEK_CUR, NO_OFF, BYTES_2, MAX_NUM_LIST, &fileSystem.fat32.reservedSectors);
    moveThroughFat32(SEEK_CUR, NO_OFF, BYTES_1, MAX_NUM_LIST, &fileSystem.fat32.numberFat);
    moveThroughFat32(SEEK_CUR, NO_OFF, BYTES_2, MAX_NUM_LIST, &fileSystem.fat32.numberEntries);
    moveThroughFat32(SEEK_SET, OFF_SECTOR_FAT, BYTES_4, MAX_NUM_LIST, &fileSystem.fat32.sectorsPerFat);
    moveThroughFat32(SEEK_SET, OFF_LABEL, BYTES_11, MAX_NUM_LIST, fileSystem.fat32.label);

    moveThroughFat32(SEEK_SET, OFF_ROOT_DIR, BYTES_4, MAX_NUM_LIST, &fileSystem.fat32.rootCluster);
    return fileSystem;
}


void getAddr(int mode, Lba_info *info, FileSystem fileSystem) {
    switch (mode) {
        case 0:
            info->cluster_begin_lba =
                    fileSystem.fat32.reservedSectors + (fileSystem.fat32.numberFat * fileSystem.fat32.sectorsPerFat);
            break;

        case 1://Moure per DS
            info->lba_adrr = (info->cluster_begin_lba + (info->i_cluster - 2) * fileSystem.fat32.sectorsPerCluster) *
                             fileSystem.fat32.sectorSize;
            break;

        case 2://Moure sobre FAT
            info->lba_adrr = (fileSystem.fat32.sectorSize * fileSystem.fat32.reservedSectors) + (info->i_cluster * 4);
            break;
    }
}

void getLongName(Lba_info *info) {
    char aux_name[255], type;
    int i, j;
    moveThroughFat32(SEEK_SET, info->lba_adrr, BYTES_1, MAX_NUM_LIST, &type);
    //printf("LN--> %x\n", type & 0x40);
    memset(aux_name, 0, 255);
    //while((type&0x40) <= 40){
    moveThroughFat32(SEEK_SET, info->lba_adrr + 1, BYTES_10, MAX_NUM_LIST, info->dir.longname1);
    j = 0;
    for (i = 0; i < 10; i++) {
        if ((i % 2) == 0) {
            aux_name[j] = info->dir.longname1[i];
            j++;
        }
    }

    moveThroughFat32(SEEK_SET, info->lba_adrr + 0xE, BYTES_12, MAX_NUM_LIST, info->dir.longname2);
    for (i = 0; i < 12; i++) {
        if ((i % 2) == 0) {
            aux_name[j] = info->dir.longname2[i];
            j++;
        }
    }

    moveThroughFat32(SEEK_SET, info->lba_adrr + 0x1C, BYTES_4, MAX_NUM_LIST, info->dir.longname3);
    for (i = 0; i < 4; i++) {
        if ((i % 2) == 0) {
            aux_name[j] = info->dir.longname3[i];
            j++;
        }
    }
    aux_name[j] = 0;
    strcpy(info->name, aux_name);
}

Lba_info *updateTrace(Lba_info *trace, int *nTraces, Lba_info info) {
    (*nTraces)++;
    trace = (Lba_info *) realloc(trace, sizeof(Lba_info) * (*nTraces));

    trace[(*nTraces) - 1] = info;
    return trace;
}

Lba_info recoveryLastTrace(Lba_info *trace, int *nTraces, Lba_info info) {
    if ((*nTraces) <= 0) {
        free(trace);
        printf("Error: File not found.\n");
        exit(1);
    } else {
        info = trace[(*nTraces) - 1];
        info.lba_adrr += 0x20;
        (*nTraces)--;
    }
    return info;
}

char *getClusterContent(Lba_info info, uint32_t *size) {
    char aux;
    char *content_file = NULL;
    moveThroughFat32(SEEK_SET, info.lba_adrr, BYTES_1_V2, MAX_NUM_LIST, &aux);
    content_file = (char *) realloc(content_file, sizeof(char) + (*size));
    content_file[(*size)] = aux;
    (*size)++;

    while ((*size) < info.dir.size || aux != 0) {
        moveThroughFat32(SEEK_CUR, NO_OFF, BYTES_1_V2, MAX_NUM_LIST, &aux);
        content_file = (char *) realloc(content_file, sizeof(char) + (*size));
        content_file[(*size)] = aux;
        (*size)++;
    }
    return content_file;
}

void showContent(Lba_info info, FileSystem fileSystem) {
    uint32_t size = 0;
    char *content_file = NULL;

    info.i_cluster = getNextClusterAddr(info.lba_adrr);
    getAddr(1, &info, fileSystem);

    content_file = getClusterContent(info, &size);
    getAddr(2, &info, fileSystem);


    while (size < info.dir.size && info.lba_adrr <= 0xfff8) {
        strcat(content_file, getClusterContent(info, &size));
    }

    printf("Content:\n\n%s\n", content_file);
    free(content_file);

    exit(1);
}


void changeDateFileFat(uint64_t offset, char *date) {
    struct tm time;
    char *token;
    int dateTime[3];
    int i = 0;
    uint16_t finalDate = 0;

    token = strtok(date, "/");

    while (token != NULL) {
        dateTime[i] = atoi(token);
        token = strtok(NULL, "/");
        i++;
    }

    time.tm_year = (dateTime[2] - 1980);
    time.tm_mon = dateTime[1];
    time.tm_mday = dateTime[0];

    finalDate = (uint16_t) ((time.tm_mday & 0b11111) |
                            (((time.tm_mon) & 0b1111) << 5) |
                            (((time.tm_year) & 0b1111111) << 9));

    lseek(fd, offset + 16, SEEK_SET);
    write(fd, &finalDate, sizeof(finalDate));
}

void handlerMode(FileSystem fileSystem, Lba_info info, char *argv, char *date, int mode) {

    switch (mode) {
        case 0://Fase2-3 --> -search
            printf("File Found! \t Size: %d Bytes\t Date: %.2d/%.2d/%.4d\n\n", info.dir.size,
                   info.dir.date.day,
                   info.dir.date.month,
                   info.dir.date.year);
            break;
        case 1://FASE 4 --> -show
            showContent(info, fileSystem);
            break;
        case 2://FASE 5 --> -r
            if (!(info.dir.attributes & 0x01)) {
                info.dir.attributes = info.dir.attributes | 0x01;
            }

            lseek(fd, info.lba_adrr + 0x0B, SEEK_SET);
            write(fd, &info.dir.attributes, sizeof(info.dir.attributes));
            break;
        case 3://FASE 5 --> -w
            if ((info.dir.attributes & 0x01)) {
                info.dir.attributes = info.dir.attributes & 0xFE;
            }
            lseek(fd, info.lba_adrr + 0x0B, SEEK_SET);
            write(fd, &info.dir.attributes, sizeof(info.dir.attributes));
            break;
        case 4://FASE 5 --> -h
            if ((info.dir.attributes & 0x02)) {
                info.dir.attributes = info.dir.attributes & 0xFD;
            }
            lseek(fd, info.lba_adrr + 0x0B, SEEK_SET);
            write(fd, &info.dir.attributes, sizeof(info.dir.attributes));
            break;
        case 5://FASE 5 --> -s
            if (!(info.dir.attributes & 0x02)) {
                info.dir.attributes = info.dir.attributes | 0x02;
            }
            lseek(fd, info.lba_adrr + 0x0B, SEEK_SET);
            write(fd, &info.dir.attributes, sizeof(info.dir.attributes));
            break;
        case 6://FASE 5 --> -d
            changeDateFileFat(info.lba_adrr, date);
            break;
    }
    if(mode>1)printf("Se han editado los permisos de %s\n", argv);
    exit(1);
}


void goTroughFS(Lba_info info, char *argv, char *date, FileSystem fileSystem, Lba_info *trace, int *nTraces, int mode) {
    char type;

    moveThroughFat32(SEEK_SET, info.lba_adrr, BYTES_11, MAX_NUM_LIST, info.dir.name);

    moveThroughFat32(SEEK_SET, info.lba_adrr + 16, BYTES_2, MAX_NUM_LIST, &info.dir.date.hex_date);
    moveThroughFat32(SEEK_SET, info.lba_adrr + 28, BYTES_4, MAX_NUM_LIST, &info.dir.size);
    info.dir.name[11] = 0;
    if (info.dir.name[0] == 0 || info.lba_adrr == info.max_lba_adrr) {
        //Anem a la fat per obtenir la direcció del següent cluster.
        getAddr(2, &info, fileSystem);
        moveThroughFat32(SEEK_SET, info.lba_adrr, BYTES_4, MAX_NUM_LIST, &info.lba_adrr);

        //Diferenciem si la fat diu que no hi han mes clusters.
        if (info.lba_adrr >= 0xffffff8) {

            if (info.i_cluster != info.init_cluster) {
                //Tornem al directori anterior
                info = recoveryLastTrace(trace, nTraces, info);
                goTroughFS(info, argv, date, fileSystem, trace, nTraces, mode);
            } else {
                printf("Error: File not found.\n");
                exit(1);
            }
        } else {
            info.i_cluster = info.lba_adrr;
            getAddr(1, &info, fileSystem);
            info.max_lba_adrr = info.lba_adrr + 512;
            goTroughFS(info, argv, date, fileSystem, trace, nTraces, mode);

        }
    } else {
        //Obtenim els atributs i apliquem dos mascares per saber si el segment de 32B es fitxer/directori o LFN
        moveThroughFat32(SEEK_SET, info.lba_adrr + 0x0B, BYTES_1, MAX_NUM_LIST, &info.dir.attributes);
        info.dir.isDir = info.dir.attributes & 0x10;
        if (info.dir.isDir > 0)info.dir.isDir = 1;
        info.dir.isLfn = info.dir.attributes & 0x0f;

        if (info.dir.isLfn == 0x0f) {
            info.dir.isLfn = 1;
            moveThroughFat32(SEEK_SET, info.lba_adrr, BYTES_1, MAX_NUM_LIST, &type);
            if ((type & 0x4F) == 0x41) {
                getLongName(&info);
                //printf("\n\nDir name: %s\n", info.name);

            } else if ((type & 0x4F) > 41) {
                getLongName(&info);
            } else if ((type & 0x4f) >= 1 && (type & 0x4f) < 41) {
                strcpy(info.old, info.name);
                getLongName(&info);
                strcat(info.name, info.old);
                if ((type & 0x4f) == 1) {
                    //printf("\n\nDir name: %s\n", info.name);
                }
            }
        } else {
            info.dir.isLfn = 0;
        }

        //Entrem en un nou directori
        if (info.dir.isDir && strcmp(info.dir.name, ".          ") && strcmp(info.dir.name, "..         ")) {
            //Actualitzem el recorregut
            trace = updateTrace(trace, nTraces, info);

            info.i_cluster = getNextClusterAddr(info.lba_adrr);

            //Entrem en un nou directori
            getAddr(1, &info, fileSystem);
            info.max_lba_adrr = info.max_lba_adrr + 512;
            goTroughFS(info, argv, date, fileSystem, trace, nTraces, mode);

        } else if (!strcmp(info.dir.name, ".          ") || !strcmp(info.dir.name, "..         ")) {
            //Mostrem el nom de fitxer, quan no es ni directori ni shortname.
            //printf("\n\nDir name: %s\n", info.dir.name);
        }

        if (!info.dir.isDir && !info.dir.isLfn && !strcmp(argv, info.name)) {
            info.dir.date.year = ((info.dir.date.hex_date & 0xFE00) >> 9) + 1980;
            info.dir.date.month = ((info.dir.date.hex_date & 0xF0) >> 5);
            info.dir.date.day = ((info.dir.date.hex_date & 0x1F));
            //Cridem a un handler que en funció del mode de busqueda realitzara la tasca corresponent
            handlerMode(fileSystem, info, argv, date, mode);
        } else {
            info.lba_adrr += 0x20;
            goTroughFS(info, argv, date, fileSystem, trace, nTraces, mode);
        }
    }

}

void searchHandler(FileSystem fileSystem, char *argv, char *date, int mode) {
    Lba_info info;
    Lba_info *trace;
    int nTraces;

    fileSystem = getInfoFat32(fileSystem);

    //Inicialitzem les @
    info.i_cluster = fileSystem.fat32.rootCluster;
    info.init_cluster = info.i_cluster;

    nTraces = 0;
    trace = NULL;

    getAddr(0, &info, fileSystem);
    getAddr(1, &info, fileSystem);
    info.max_lba_adrr = info.lba_adrr + 512;
    goTroughFS(info, argv, date, fileSystem, trace, &nTraces, mode);
    printf("Error: File not found.\n");
}


uint32_t getNextClusterAddr(uint32_t addr_base) {
    uint16_t low, high;

    moveThroughFat32(SEEK_SET, addr_base + LOW_ADDR_FC, BYTES_2, MAX_NUM_LIST, &low);
    moveThroughFat32(SEEK_SET, addr_base + HIGH_ADDR_FC, BYTES_2, MAX_NUM_LIST, &high);

    uint32_t aux = low + (high << 8);
    return aux;
}

FileSystem initSearchInfoFat32(FileSystem fileSystem) {
    fileSystem = getInfoFat32(fileSystem);
    showInfoFat32(fileSystem.fat32);
    return fileSystem;
}

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
    //printf("BUFFER: -%s-\n", aux);
    if (strcmp(aux, "FAT32   ") == 0) {
        return 1;
    } else {
        moveThroughFat32(SEEK_SET, OFF_FAT_TYPE_2, BYTES_8, 1, aux);
        if (strcmp(aux, "FAT12   ") == 0) {
            printf(NOT_RECOGNIZED, "FAT12");
            exit(1);
        } else if (strcmp(aux, "FAT16   ") == 0) {
            printf(NOT_RECOGNIZED, "FAT16");
            exit(1);
        } else {
            printf(NOT_FOUND);
            return 0;

        }
    }
}