include ../cflags.make
LIBS=-lcurses
CFLAGS+=-Wno-missing-braces
SRC=buffer.c cursor.c display.c files.c line.c random.c search.c term.c window.c word.c main.c
OBC=buffer.o cursor.o line.o random.o search.o term.o window.o word.o
OBJ=$(OBC) display.o help.o files.o
OBJL=$(OBC) display-h.o help-h.o files-h.o

all: libmex.a

test: libmex.a mex

mex: main.o $(OBJ)
	$(CC) -o $@ $(LDFLAGS) main.o $(OBJ) $(LIBS)

libmex.a: mainloop.o $(OBJL)
	$(AR) rcs $@ mainloop.o $(OBJL)

$(SRC):	ed.h curkeys.h

mainloop.o: main.c
	$(CC) $(CFLAGS) -DEMBEDDED -c main.c -o $@

display-h.o: display.c
	$(CC) $(CFLAGS) -DEMBEDDED -c display.c -o $@
help-h.o: help.c
	$(CC) $(CFLAGS) -DEMBEDDED -c help.c -o $@
files-h.o: files.c
	$(CC) $(CFLAGS) -DEMBEDDED -c files.c -o $@

term.o: term.c
	$(CC) $(CFLAGS) -I/usr/include/ncurses -c term.c

curkeys.h: curkeys.c
	$(CC) -I/usr/include/ncurses curkeys.c -o curkey2
	./curkey2 >$@
	rm curkey2

help.c: help_c.eng
	cp help_c.eng $@

clean:
	rm -f *.o core* mex me curkeys.h help.c curkey2 libmex.a
