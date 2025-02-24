CC=gcc
CFLAGS=-I. -lpthread -lm
DEPS=board.h movement.h keyboard.h neural_network.h dqn.h
OBJ=board.o movement.o main.o keyboard.o neural_network.o dqn.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

main: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)
