CC = gcc
CFLAGS = -Wall

all : latest_version

latest_version : libpower.h libpower.c main.c
	$(CC) -c libpower.c -o libpower.o $(CFLAGS)
	$(CC) -c main.c -o main.o $(CFLAGS)
	$(CC) main.o libpower.o -o latest_version $(CFLAGS)
	rm *.o

clean :
	rm -f *.o latest_version *.c~
