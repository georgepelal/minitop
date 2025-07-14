CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c99

all: minitop clean

main.o: src/main.c src/proc_reader.h
	$(CC) $(CFLAGS) -c src/main.c

proc_reader.o: src/proc_reader.c src/proc_reader.h
	$(CC) $(CFLAGS) -c src/proc_reader.c

minitop: main.o proc_reader.o
	$(CC) -o minitop main.o proc_reader.o

clean:
	rm -f *.o
