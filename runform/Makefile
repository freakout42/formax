SRCS=$(wildcard *.cpp)
OBJS=$(patsubst %.cpp,%.o,$(SRCS))
CXXFLAGS=-g	-O0 -Wall -Werror
RUNFORM=./runform ../generate/scotty.frm ../generate/scotty.sq3

runform: $(OBJS)
	$(CXX) $(OBJS) -lcurses -lodbc -o $@

test: runform
	/opt/valgrind/bin/valgrind --quiet --leak-check=yes --suppressions=valgrind.sup $(RUNFORM) 2>$@
	test -f $@
	test -s $@ || true
#	/opt/valgrind/bin/valgrind --leak-check=yes --gen-suppressions=all $(RUNFORM)

screen.cpp: curkeys.h

curkeys.h: curkeys
	./curkeys >$@

clean:
	rm -f *.o a.out core.* vgcore.* runform curkeys curkeys.h test

