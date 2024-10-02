CFLAGS := -O0 -g -std=c99

all: elk.o

elk.o: test elk.c
	$(CC) $(CFLAGS) -c elk.c

test: unittest.c
	$(CC) $(CFLAGS) unittest.c -o unittest
	./unittest
	
clean:
	rm -f *.o unittest

