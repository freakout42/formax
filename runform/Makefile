include cflags.make
SRCS=$(wildcard *.cpp)
OBJS=$(patsubst %.cpp,%.o,$(SRCS))
OBJA=cqerror.o colquery/colquery.o crypt/libmd5.a membed/libmex.a elk/elk.o regex/re.o
EXAMPLE=../generate/scotty.frm ../generate/scotty.sq3
FORMAXE=../generate/formax.frm ../generate/formax.sq3
VALGRINDSUP=--gen-suppressions=no #all

all: runform
	strip runform

$(OBJS): runform.h
runform.h: curkeys.h

runform: $(OBJS) $(OBJA)
	$(CXX) $(OBJS) $(OBJA) -Wl,-rpath,/opt/arx/lib -lcurses -lsqlite3 -lodbc -o $@

run: runform $(EXAMPLE)
	strip runform
	./runform -x -g/tmp/formax.log ../generate/formax.frm ../generate/scotty.frm 2>test

test: clean runform $(EXAMPLE) $(FORMAXE)
	valgrind --quiet $(VALGRINDSUP) --leak-check=yes --track-origins=yes --suppressions=valgrind.sup \
    ./runform -x -g/tmp/formax.log $(EXAMPLE) 2>$@
	test -f $@ -a ! -s $@

curkeys.h: curkeys
	./curkeys >$@

colquery/colquery.o:
	cd colquery; make $(SUBTARGET)
crypt/libmd5.a:
	cd crypt; make $(SUBTARGET)
membed/libmex.a:
	cd membed; make $(SUBTARGET)
elk/elk.o:
	cd elk; make $(SUBTARGET)
regex/re.o:
	cd regex; make $(SUBTARGET)

$(EXAMPLE):
	cd ../generate; make clean test
$(FORMAXE):
	cd ../generate; make clean test

clean:
	for D in colquery crypt membed elk regex ../generate; do ( cd $$D; make clean; ); done
	rm -f *.o a.out *core.* runform curkeys curkeys.h test afiedt.buf /tmp/odbc.log ads_* $(EXAMPLE) testelk

