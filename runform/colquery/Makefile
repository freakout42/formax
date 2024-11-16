include ../cflags.make
CFLAGS+=-DYY_NO_UNPUT -DYY_NO_INPUT -DYY_SKIP_YYWRAP
LDFLAGS=-r
YACC=bison
YFLAGS=-d --name-prefix=cq
# -Wno-conflicts-sr
LEXFLAGS=--prefix=cq

all: colquery.o
small: colquery.o

test: colq
	valgrind --quiet --leak-check=yes ./colq "2 a" 3-7 2>$@
	test -f $@
	test -s $@ || true

colq: colq.c colquery.o colquery.h yyerror.o
	$(CC) $(CFLAGS) -DYYDEBUG colq.c colquery.o yyerror.o -o $@

colquery.o: colquery0.o colquery.h parse.o cql.o cqy.o
	$(LD) $(LDFLAGS) colquery0.o parse.o cqy.o cql.o -o $@

colquery0.o: colquery.c colquery.h colq.h
	$(CC) $(CFLAGS) -c colquery.c -o $@

cql.o: cql.l cqy.o
	$(LEX) $(LEXFLAGS) cql.l
	sed 's/YYcqwrap/yywrap/g' <lex.cq.c >cql.c
	: patch -p0 <cql.c.patch
	$(CC) $(CFLAGS) -c cql.c

cqy.c colq.h: cqy.y
	$(YACC) $(YFLAGS) cqy.y
	@if [ -f y.tab.c ];   then mv y.tab.c   cqy.c;  fi
	@if [ -f y.tab.h ];   then mv y.tab.h   colq.h; fi
	@if [ -f cqy.tab.c ]; then mv cqy.tab.c cqy.c;  fi
	@if [ -f cqy.tab.h ]; then mv cqy.tab.h colq.h; fi

cqy.o: cqy.c colq.h
	cp cqy.c  y.tab.c
	cp colq.h cqy.tab.h
	$(CC) $(CFLAGS) -c y.tab.c
	mv y.tab.o cqy.o

colquery.h: cqy.o

clean:
	rm -f *.o y.tab.[ch] colquery.map colq lex.cq.c y.tab.c *.output cqy.tab.* test *core.* cql.y colq.h cqy.c cql.c

