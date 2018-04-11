//
// Created by sergi on 11/4/18.
//
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <linux/kernel.h>

#define OFFSET_SUPERBLOCK 1024
#define OFFSET_MAGICNUMBER 56

#ifndef RAGNAROK_FILEUTILITIES_H
#define RAGNAROK_FILEUTILITIES_H

int exisitsFile(char *path);

int detectFileSystemType(int fd);
#endif //RAGNAROK_FILEUTILITIES_H
