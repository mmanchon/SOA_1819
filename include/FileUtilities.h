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

//includes propios
#include "Types.h"
#include "Ext4.h"
#include "Fat32.h"


/**
 * Funcion para mirar si existe el fichero y el FS es valido
 * @param volume nombre del volumnen
 * @return segun si existe/ es ext4 / es fat32
 */

int checkFileSystem(char *volume);

/**
 * Funcion para comprovar si el fichero existe
 * Si el fichero existe retorna el file descriptor
 * En caso contrario muestra un mensahe y finaliza ejecución
 * @param path nombre fichero
 * @return file descriptor
 */
 
int exisitsFile(char *path);

/**
 * Funcion para detectar el tipo de FS
 * Se mueve a la posicion donde deberia estar el magic number de EXT
 * en caso de estar va a comprovar si es ext4/ext3/ext2
 * de lo contrario va a var si es fat32/fat16/fat12
 * si no es ninguno de los anteriores indica fichero no encontrado
 * @param fd file descriptor del fichero
 * @return valor según lo encuentre o no
 */
 
int detectFileSystemType(int fd);

/**
 * Funcion para mostrar la informacion de un FS.
 * Comprueva si el fichero existe
 * Detecta que FS es y seguidamente, si es un FS correcto
 * llama a las funciones para mostrar la informacion del FS
 * @param volume nombre del volumen
 */
 
void infoFS(char *volume);

/**
 * Funcion para buscar un fichero.
 * Comprueva si el fichero existe y si hay FS valido
 * En caso de pasar las pruebas realiza una busqueda
 * "deepseach" a lo largo de todos los nodos de FS
 * @param volume nombre del volumen
 * @param file fichero a entcontrar
 */
 
void searchFileFS(char *volume, char *file);

/**
 * Funcion para mostrar el contenido del fichero.
 * Comprueva si el fichero existe y si hay FS valido.
 * Una vez acabado (realizando otra vez el deepsearch para
 * encontrar el fichero), llama a las funciones para mostrar
 * el contenido de dicho fichero dependiendo del FS
 * @param volume nombre del volumen
 * @param file nombre del fichero
 */
 
void showFileFS(char *volume, char *file);

/**
 * Funcion para cambiar los permisos a solo lectura.
 * Comprueva si existe y el FS es correcto.
 * Llama a las funciones para modificar los permisos
 * @param volume  nombre del volumen
 * @param file nombre del fichero
 */
 
void onlyReadMode(char *volume, char *file);

/**
 * Funcion para desactivar el modo de solo lecture.
 * Comprueva si existe el fichero y si es un FS valido
 * Llama a las funciones para modificar los permisos.
 * @param volume nombre del volumen
 * @param file nombre del fichero
 */

void notOnlyReadMode(char *volume, char *file);

/**
 * Funcion para cambiar el modo de un fichero a oculto.
 * Esta funcion solo esta habilitada para fat32.
 * Realiza las comprovaciones pertinentes y llama a las
 * funciones encargadas de cambiar el modo.
 * @param volume nombre del volumen
 * @param file nombre del fichero
 */

void hiddenFile(char *volume, char*file);

/**
 * Funcion para cambiar el modo a NO oculto.
 * Esta funcion solo esta habilitada para fat32.
 * Realiza las comprovaciones pertinentes(exista FS).
 * Llama a las funciones para cambiar de modo
 * @param volume nombre del volumen
 * @param file nombre del fichero
 */

void unHiddenFile(char *volume, char *file);

/**
 * Funcion para cambiar la fecha del fichero
 * Comprueva que exista y el FS sea valido
 * Llama a las funciones para cambia la fecha de
 * creacion del fichero.
 * @param volume nombre del volumen
 * @param file nombre del fichero
 * @param date fecha a cambiar por
 */

void changeFileDate(char *volume, char *file, char *date);

#endif //RAGNAROK_FILEUTILITIES_H
