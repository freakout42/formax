include ../cflags.make
LIBS=-lcurses
CFLAGS+=-Wno-missing-braces
SRC=buffer.c cursor.c display.c files.c line.c random.c search.c term.c window.c word.c help.c main.c
OBJ=$(patsubst %.c,%.o,$(SRC))
OBE=$(patsubst %.c,%.e,$(SRC))
OBO=$(patsubst %.c,%.e.o,$(SRC))

all: libmex.a
small: libmex.a

test: libmex.a mex

mex: main.o $(OBJ)
	$(CC) -o $@ $(LDFLAGS) $(OBJ) $(LIBS)

libmex.a: $(OBE)
	for O in $(OBE); do mv $$O $$O.o; done
	$(AR) rcs $@ $(OBO)

$(SRC):	ed.h curkeys.h

%.e: %.c
	$(CC) $(CFLAGS) -DEMBEDDED -c $< -o $@

term.o: term.c
	$(CC) $(CFLAGS) -I/usr/include/ncurses -c term.c

curkeys.h: curkeys.c
	$(CC) $(CFLAGS) curkeys.c -o curkey2
	./curkey2 >$@
	rm -f curkey2*

help.c: help_c.eng
	cp help_c.eng $@

clean:
	rm -f *.o *.e core* mex me curkeys.h help.c curkey2* libmex.a
