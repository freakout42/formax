CFLAGS := -O0 -g -std=c99

all: elk.o

elk.o: test elk.c
	$(CC) $(CFLAGS) -c elk.c

test: unittest.c
	$(CC) $(CFLAGS) unittest.c -o unittest
	./unittest
	
testelk: testelk.c
	$(CC) $(CFLAGS) testelk.c -o testelk
	./testelk

clean:
	rm -f *.o unittest

