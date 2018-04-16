all: Ragnarok

command='-info'

main.o: main.c FileUtilities.h
	gcc main.c -c -Wall -Wextra

FileUtilities.o: FileUtilities.c FileUtilities.h Ext4.h
	gcc FileUtilities.c -c -Wall -Wextra

Ext4.o: Ext4.c Ext4.h
	gcc Ext4.c -c -Wall -Wextra

Fat32.o: Fat32.c Fat32.h
	gcc Fat32.c -c -Wall -Wextra

Ragnarok: main.o FileUtilities.o Ext4.o Fat32.o
	gcc main.o FileUtilities.o Ext4.o Fat32.o -o Ragnarok -Wall -Wextra -lm

run: ./Ragnarok $(command) ../Volumenes/Volume_1_1024_Block_Size.ext4

clean:
	rm *.o Ragnarok

tar:
	tar -cvzf Ragnarok.tar.gz *.c *.h makefile
