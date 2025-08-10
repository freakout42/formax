NCURSESVER=6.3
include cflags.make
SRCS=$(wildcard *.cpp)
OBJS=$(patsubst %.cpp,%.o,$(SRCS))
OBJB=cqerror.o colquery/colquery.o elk/elk.o regex/re.o membed/libmex.a
OBJA=$(OBJB) crypt/libmd5.a
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
	$(CXX) $(OBJS) $(CLDFLAGS) $(OBJB) $(CURSESLIB) -lodbc -o $@
	ls -l $@

runform: $(OBJS) $(OBJA)
	$(CXX) $(OBJS) $(CLDFLAGS) $(OBJA) $(CURSESLIB) -lsqlite3 -lodbc -o $@

run: runform $(EXAMPLE) $(FORMAXE)
	./runform -zx $(EXAMPLE)

test: clean runform $(EXAMPLE)
	@rm -f /tmp/formax.log
	@cp ../generate/scotty.sq3 .
	valgrind --quiet $(VALGRINDSUP) --leak-check=yes --track-origins=yes --suppressions=valgrind.sup \
		./runform -r -x -g/tmp/formax.log $(EXAMPLE) 2>$@ | \
			grep -v -e'formax.freakout.de' -e'compile[dr]:' -ecurses.inc: -ecurses.run: -esqlite -edver: -echarset: >out
	test -f $@ -a ! -s $@
	sqlite3 /tmp/formax.log 'select logtext from logs order by id;' | diff ../generate/scotty.log -
	diff ../generate/scotty.out out
	@rm -f out
	@rm -f /tmp/formax.log
	@cp scotty.sq3 ../generate
	valgrind --quiet $(VALGRINDSUP) --leak-check=yes --track-origins=yes --suppressions=valgrind.sup \
		./runform -w -x -g/tmp/formax.log $(EXAMPLE) 2>$@
	@test -f $@ -a ! -s $@
	@sqlite3 /tmp/formax.log 'select logtext from logs order by id;' | diff ../generate/scotty.log -
	@mv scotty.sq3 ../generate
	./runform -zx -g/tmp/formax.log $(EXAMPLE)
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
$(CURSESVARANT)/include/curses.h: ../bin/ncurses.tar.gz
	tar xzf ../bin/ncurses.tar.gz
../bin/ncurses.tar.gz: ../bin/ncurses-$(NCURSESVER).tar.gz
	@rm -rf ncurses*
	tar xzf ../bin/ncurses-$(NCURSESVER).tar.gz
	mv ncurses-$(NCURSESVER) ncurses
	cd ncurses; ./configure --prefix=/opt/ncurses --with-terminfo-dirs=/usr/lib/terminfo; make
	tar czf $@ ncurses/include ncurses/lib/libncurses.a
# wget -O../bin/ncurses-win32.zip            https://invisible-island.net/datafiles/release/mingw32.zip
# wget -O../bin/ncurses-$(NCURSESVER).tar.gz https://invisible-island.net/datafiles/release/ncurses.tar.gz

$(EXAMPLE): ../generate/scotty.in2
	cd ../generate; make clean test
$(FORMAXE): ../generate/formax.inp
	cd ../generate; make clean test

prepw32: scotty.sq3
scotty.sq3:
	sqlite3 scotty.frm <../generate/scotty.in2
	sqlite3 scotty.sq3 <../generate/scotty.sql

libncursesw6.dll: ../bin/ncurses-win32.zip scotty.sq3
	unzip ../bin/ncurses-win32.zip
	cp ../bin/*.dll ../bin/*.exe .

clean:
	for D in colquery crypt membed elk regex ../generate; do ( cd $$D; make clean >/dev/null; ); done
	rm -f *.o a.out *core.* runform runform0 curkeys curkeys.h test afiedt.buf \
		/tmp/odbc.log ads_* $(EXAMPLE) testelk formax.frm company.h out *.exe *.dll scotty.*
	rm -rf bin include lib ncurses
