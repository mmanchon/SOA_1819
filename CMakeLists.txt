cmake_minimum_required(VERSION 2.6.0)

# here we specify that the project is C language only, so the default
# C compiler on the system will be used
project(SOA C)

#incluimos las librerias propias necesarias
include_directories(include)

#incluimos las sources del programa
set(SOURCES main.c src/Ext4.c src/Fat32.c src/FileUtilities.c)

add_executable(SOA ${SOURCES})