include ../cflags.make
CFLAGS+=-std=c99 -I.

all: re.o
small: re.o

test: testre

re.o: re.c re.h

testre: testre.c re.o
	$(CC) $(CFLAGS) testre.c re.o -o $@
	./$@
	
clean:
	rm -f *.o testre

