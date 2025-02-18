CC=gcc
CFLAGS=-I.
DEPS=board.h movement.h keyboard.h
OBJ=board.o movement.o main.o keyboard.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

main: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)
