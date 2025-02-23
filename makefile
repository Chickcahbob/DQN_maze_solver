CC=gcc
CFLAGS=-I. -lpthread
DEPS=board.h movement.h keyboard.h neural_network.h
OBJ=board.o movement.o main.o keyboard.o neural_network.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

main: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)
