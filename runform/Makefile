include cflags.make
SRCS=$(wildcard *.cpp)
OBJS=$(patsubst %.cpp,%.o,$(SRCS))
OBJA=cqerror.o colquery/colquery.o crypt/libmd5.a membed/libmex.a elk/elk.o regex/re.o
EXAMPLE=../generate/scotty.frm ../generate/scotty.sq3
FORMAXE=../generate/formax.frm ../generate/formax.sq3
VALGRINDSUP=--gen-suppressions=no #all

all: runform
	strip runform

$(OBJS): version.h
version.h: runform.h
	git log -1 | sed -n 's/commit \(.*\)/#define GITCOMMIT "\1"/p' >$@
runform.h: company.h curkeys.h
company.h: arx.h
	if [ -f $@ ]; then touch $@; else cp arx.h $@; fi

runform: $(OBJS) $(OBJA)
	$(CXX) $(OBJS) $(CLDFLAGS) $(OBJA) -lcurses -lsqlite3 -lodbc -o $@

run: runform $(EXAMPLE)
	strip runform
		./runform -x -g/tmp/formax.log $(EXAMPLE)
#	./runform -x -g/tmp/formax.log ../generate/formax.frm ../generate/scotty.frm
#	sqlite3 ../generate/scotty.frm .dump >../generate/scotty.in2
#	cp ../generate/formax.frm .
#	./runform -x -g/tmp/formax.log ../generate/formax.frm formax.frm
#	sqlite3 formax.frm .dump >../generate/formax.inp

test: clean runform $(EXAMPLE) $(FORMAXE)
	valgrind --quiet $(VALGRINDSUP) --leak-check=yes --track-origins=yes --suppressions=valgrind.sup \
		./runform -wx -g/tmp/formax.log $(EXAMPLE) 2>$@
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
	for D in colquery crypt membed elk regex ../generate; do ( cd $$D; make clean >/dev/null; ); done
	rm -f *.o a.out *core.* runform curkeys curkeys.h test afiedt.buf \
		/tmp/odbc.log /tmp/formax.log ads_* $(EXAMPLE) testelk formax.frm
