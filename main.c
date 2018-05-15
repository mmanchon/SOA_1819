/**
 *
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "FileUtilities.h"

int main(int argc, char **argv) {

    //En todos los casos tiene que haber 3 argumentos
    if (!argc) {
        printf(ARGUMENTS_NUMBER);

    } else {

        if (strcmp(argv[1], "-info") == 0) {

            //La comanda -info tiene 3 argumentos
            if (argc != 3) {
                printf(ARGUMENTS_NUMBER);

            } else {

                //buscamos la informacion del fs
                infoFS(argv[2]);

            }

        } else if (strcmp(argv[1], "-d") == 0) {

            //otro caso unico donde los argumentos son individuales
            if (argc != 5) {
                printf(ARGUMENTS_NUMBER);

            } else {
                //cambiamos la fecha del fs
                changeFileDate(argv[3], argv[4], argv[2]);

            }

        }else{

            //argupacion de comandos con 4 argumentos
            if(argc != 4){
                printf(ARGUMENTS_NUMBER);

            }else {

                if (strcmp(argv[1], "-search") == 0) {

                    //Buscamos el fichero dentro del fs con deepsearch
                    searchFileFS(argv[2], argv[3]);

                } else if (strcmp(argv[1], "-show") == 0) {

                    //Mostramos el contenido del fichero del fs
                    showFileFS(argv[2], argv[3]);

                } else if (strcmp(argv[1], "-r") == 0) {

                    //modificamos los permisos del fichero a solo lectura
                    onlyReadMode(argv[2], argv[3]);

                } else if (strcmp(argv[1], "-w") == 0) {

                    //modificamos los permisos del fichero a NO solo lectura
                    notOnlyReadMode(argv[2], argv[3]);

                } else if (strcmp(argv[1], "-h") == 0) {

                    //cambiamos el fichero a oculto
                    hiddenFile(argv[2],argv[3]);

                } else if (strcmp(argv[1], "-s") == 0) {

                    //cambiamos el fichero a NO oculto
                    unHiddenFile(argv[2], argv[3]);

                }
            }
        }
    }
    return 0;
}