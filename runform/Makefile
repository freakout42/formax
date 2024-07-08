all:  CXXFLAGS=-O3 -DNDEBUG
test: CXXFLAGS=-g -O0 -Wall -Werror
SRCS=$(wildcard *.cpp)
OBJS=$(patsubst %.cpp,%.o,$(SRCS))
EXAMPLE=../generate/scotty.frm ../generate/scotty.sq3
VALGRINDSUP=--gen-suppressions=all

all: runform

runform: curkeys.h $(OBJS) cqerror.o colquery/colquery.o
	$(CXX) $(OBJS) cqerror.o colquery/colquery.o -lcurses -lodbc -o $@

test: runform $(EXAMPLE)
	valgrind --quiet --leak-check=yes --suppressions=valgrind.sup ./runform $(EXAMPLE) 2>$@
	test -f $@
	test -s $@ || true

curkeys.h: curkeys
	./curkeys >$@

colquery/colquery.o:
	cd colquery; make colquery.o

$(EXAMPLE):
	cd ../generate; make

clean:
	rm -f *.o a.out *core.* runform curkeys curkeys.h test

