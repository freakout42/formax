include ../cflags.make
CFLAGS+=-std=c99

all: elk.o

test: unittest testelk elk.o
	./unittest
	./testelk

elk.o: elk.c elk.h

unittest.c: elk.o
	
testelk.c: elk.o

clean:
	rm -f *.o unittest testelk

