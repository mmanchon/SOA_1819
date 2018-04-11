all: Ragnarok

main.o: main.c FileUtilities.h
	gcc main.c -c -Wall -Wextra

FileUtilities.o: FileUtilities.c FileUtilities.h
	gcc FileUtilities.c -c -Wall -Wextra


Ragnarok: main.o FileUtilities.o
	gcc main.o FileUtilities.o -o Ragnarok -Wall -Wextra

run: ./Ragnarok "-info" ../Volumenes/Volume_1_1024_Block_Size.ext4

clean:
	rm *.o Ragnarok

tar:
	tar -cvzf Ragnarok.tar.gz *.c *.h makefile
