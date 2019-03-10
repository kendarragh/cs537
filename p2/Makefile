CC=gcc
CFLAGS=-lpthread -Wall -Wextra 
DEPS= queue.h
OBJ = main.o queue.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

prodcomm: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -f prodcomm main.o queue.o
