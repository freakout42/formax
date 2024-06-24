SRCS=$(wildcard *.cpp)
OBJS=$(patsubst %.cpp,%.o,$(SRCS))
CXXFLAGS=-g	-Wall -Werror

runform: $(OBJS)
	$(CXX) $(OBJS) -lcurses -lodbc -o $@

screen.cpp: curkeys.h

curkeys.h: curkeys
	./curkeys >$@

clean:
	rm -f *.o a.out core.* runform curkeys curkeys.h

