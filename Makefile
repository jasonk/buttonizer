CC=gcc 
CFLAGS= -Wall -O2

all: buttonizer

buttonizer: buttonizer.o 
	$(CC) -o $@ buttonizer.o 

buttonizer.o: buttonizer.c 
	$(CC) $(CFLAGS) -c buttonizer.c 

clean:
	rm -f buttonizer *.o core
