//
// Created by sergi on 11/4/18.
//


#ifndef RAGNAROK_FILEUTILITIES_H
#define RAGNAROK_FILEUTILITIES_H

//includes del systema
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <linux/kernel.h>
//includes propios
#include "types.h"
#include "Ext4.h"
#include "Fat32.h"

int exisitsFile(char *path);

int detectFileSystemType(int fd);


#endif //RAGNAROK_FILEUTILITIES_H
