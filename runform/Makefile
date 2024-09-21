# MacOsX: brew install ncurses unixodbc sqliteodbc
LINTING=-Wall -Werror -Wno-write-strings
all:  CXXFLAGS=-O3 -DNDEBUG $(LINTING)
test: CXXFLAGS=-g -O0 $(LINTING)
SRCS=$(wildcard *.cpp)
OBJS=$(patsubst %.cpp,%.o,$(SRCS))
OBJA=cqerror.o colquery/colquery.o regex/re.o elk/elk.o crypt/libmd5.a membed/libmex.a
EXAMPLE=../generate/scotty.frm ../generate/scotty.sq3
VALGRINDSUP=--gen-suppressions=no #all

all: runform

runform: curkeys.h $(OBJS) $(OBJA)
	$(CXX) $(OBJS) $(OBJA) -lcurses -lsqlite3 -lodbc -o $@

run: runform $(EXAMPLE)
	./runform -g/tmp/formax.log $(EXAMPLE)
# ./runform ../generate/formax.frm ../generate/formax.frm 2>$@

test: runform $(EXAMPLE)
	/opt/valgrind/bin/valgrind --quiet $(VALGRINDSUP) --leak-check=yes --track-origins=yes --suppressions=valgrind.sup \
    ./runform -g/tmp/formax.log $(EXAMPLE) 2>$@
	test -f $@ -a ! -s $@

curkeys.h: curkeys
	./curkeys >$@

colquery/colquery.o:
	cd colquery; make colquery.o

regex/re.o:
	cd regex; make re.o

elk/elk.o:
	cd elk; make elk.o

crypt/libmd5.a:
	cd crypt; make

membed/libmex.a:
	cd membed; make

$(EXAMPLE):
	cd ../generate; make clean test

clean:
	for D in colquery regex elk crypt membed; do ( cd $$D; make clean; ); done
	rm -f *.o a.out *core.* runform curkeys curkeys.h test afiedt.buf /tmp/odbc.log ads_* $(EXAMPLE)
