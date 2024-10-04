include ../cflags.make
LIBS=-lcurses
SRC=buffer.c cursor.c display.c files.c line.c random.c search.c term.c window.c word.c main.c
OBJ=buffer.o cursor.o display.o files.o line.o random.o search.o term.o window.o word.o help.o

all: libmex.a

test: mex

mex: main.o libmex.a
	$(CC) -o $@ $(LDFLAGS) -L. main.o -lmex $(LIBS)

libmex.a: mainloop.o $(OBJ)
	$(AR) rcs $@ mainloop.o $(OBJ)

$(SRC):	ed.h

main.o: main.c curkeys.h
	$(CC) $(CFLAGS) -Wno-missing-braces -c main.c

mainloop.o: main.c curkeys.h
	$(CC) $(CFLAGS) -DEMBEDDED -Wno-missing-braces -c main.c -o $@

term.o: term.c ed.h
	$(CC) $(CFLAGS) -I/usr/include/ncurses -c term.c

curkeys.h: curkeys.c
	$(CC) -I/usr/include/ncurses curkeys.c -o curkey2
	./curkey2 >$@
	rm curkey2

help.c: help_c.eng
	cp help_c.eng $@

clean:
	rm -f *.o core* mex me curkeys.h help.c curkey2 libmex.a
