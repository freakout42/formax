include cflags.make
SRCS=$(wildcard *.cpp)
OBJS=$(patsubst %.cpp,%.o,$(SRCS))
OBJB=cqerror.o colquery/colquery.o elk/elk.o regex/re.o
OBJA=$(OBJB) crypt/libmd5.a membed/libmex.a
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

runform0: clean $(OBJS) $(OBJA)
	$(CXX) $(OBJS) $(CLDFLAGS) $(OBJB) -lcurses -lodbc -o $@
	ls -l $@

runform: $(OBJS) $(OBJA)
	$(CXX) $(OBJS) $(CLDFLAGS) $(OBJA) -lcurses -lsqlite3 -lodbc -o $@

run: runform $(EXAMPLE)
	./runform -zx $(EXAMPLE)
#	./runform -x ../generate/formax.frm ../generate/scotty.frm
#	sqlite3 ../generate/scotty.frm .dump >../generate/scotty.in2
#	cp ../generate/formax.frm .
#	./runform -x ../generate/formax.frm formax.frm
#	sqlite3 formax.frm .dump >../generate/formax.inp

#RUNMODE=-w
RUNMODE=-r/dev/null
test: clean runform $(EXAMPLE) $(FORMAXE)
	@rm -f /tmp/formax.log
	valgrind --quiet $(VALGRINDSUP) --leak-check=yes --track-origins=yes --suppressions=valgrind.sup \
		./runform $(RUNMODE) -x -g/tmp/formax.log $(EXAMPLE) 2>$@
	@test -f $@ -a ! -s $@
	@sqlite3 /tmp/formax.log 'select logtext from logs order by id;' | diff ../generate/scotty.log -
	@echo -e "\033[0;32mOk\\033[0m"

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

$(EXAMPLE): ../generate/scotty.in2
	cd ../generate; make clean test
$(FORMAXE): ../generate/formax.inp
	cd ../generate; make clean test

clean:
	for D in colquery crypt membed elk regex ../generate; do ( cd $$D; make clean >/dev/null; ); done
	rm -f *.o a.out *core.* runform runform0 curkeys curkeys.h test afiedt.buf \
		/tmp/odbc.log ads_* $(EXAMPLE) testelk formax.frm company.h
