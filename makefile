all: flip clean

flip: img.o flip.o main.o
	gcc source/main.o source/flip.o source/img.o -o binary/flip

main.o: main.c
	gcc -Wall -std=c99 -c source/main.c

flip.o: flip.c flip.h
	gcc -Wall -std=c99 -c source/flip.c

img.o: img.c img.h
	gcc -Wall -std=c99 -c source/img.c

clean:
	rm -f source/*.o