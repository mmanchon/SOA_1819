/**
 *
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/FileUtilities.h"

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
        }else{

            if(argc != 4){
                printf(ARGUMENTS_NUMBER);

            }else{
                if(strcmp(argv[1], "-search") == 0){

                }else if(strcmp(argv[1], "-show") == 0){

                }

            }
        }
    }
    return 0;
}