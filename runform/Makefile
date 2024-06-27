SRCS=$(wildcard *.cpp)
OBJS=$(patsubst %.cpp,%.o,$(SRCS))
CXXFLAGS=-g	-O0 -Wall -Werror

runform: $(OBJS)
	$(CXX) $(OBJS) -lcurses -lodbc -o $@

valg: runform
	/opt/valgrind/bin/valgrind --leak-check=yes ./runform ../generate/scotty.frm ../generate/scotty.sq3 2>lll

test: runform
	./runform ../generate/scotty.frm ../generate/scotty.sq3 2>lll

screen.cpp: curkeys.h

curkeys.h: curkeys
	./curkeys >$@

clean:
	rm -f *.o a.out core.* runform curkeys curkeys.h

