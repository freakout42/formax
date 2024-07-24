all:  CXXFLAGS=-O3 -DNDEBUG
test: CXXFLAGS=-g -O0 -Wall -Werror
SRCS=$(wildcard *.cpp)
OBJS=$(patsubst %.cpp,%.o,$(SRCS))
OBJA=cqerror.o colquery/colquery.o regex/re.o elk/elk.o
EXAMPLE=../generate/scotty.frm ../generate/scotty.sq3
VALGRINDSUP=--gen-suppressions=no #all

all: runform

runform: curkeys.h $(OBJS) $(OBJA)
	$(CXX) $(OBJS) $(OBJA) -lcurses -lsqlite3 -lodbc -o $@

test: runform $(EXAMPLE)
	valgrind --quiet $(VALGRINDSUP) --leak-check=yes --track-origins=yes --suppressions=valgrind.sup \
    ./runform -g/tmp/formax.log -nuk $(EXAMPLE) 2>$@
	test -f $@ -a ! -s $@

curkeys.h: curkeys
	./curkeys >$@

colquery/colquery.o:
	cd colquery; make colquery.o

regex/re.o:
	cd regex; make re.o

elk/elk.o:
	cd elk; make elk.o

$(EXAMPLE):
	cd ../generate; make

clean:
	rm -f *.o a.out *core.* runform curkeys curkeys.h test afiedt.buf /tmp/odbc.log ads_*

