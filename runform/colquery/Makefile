CFLAGS=-DYY_NO_UNPUT -DYY_SKIP_YYWRAP
test: CFLAGS=-g -O0 -Wall -Werror -DYY_NO_UNPUT -DYY_SKIP_YYWRAP $(DEBUG)
LDFLAGS=-r
YFLAGS=-Wno-conflicts-sr -d --name-prefix=cq
LEXFLAGS=--prefix=cq

all: colq

colq: colq.c colquery.o colquery.h yyerror.o
	$(CC) $(CFLAGS) -DYYDEBUG colq.c colquery.o yyerror.o -o $@

test: colq
	valgrind --quiet --leak-check=yes ./colq "2 a" 3-7 2>$@
	test -f $@
	test -s $@ || true

colquery.o: colquery0.o colquery.h parse.o cql.o cqy.o
	$(LD) $(LDFLAGS) colquery0.o parse.o cqy.o cql.o -o $@

colquery0.o: colquery.c colquery.h colq.h
	$(CC) $(CFLAGS) -c colquery.c -o $@

cql.o: cql.l cqy.o
	$(LEX) $(LEXFLAGS) cql.l
	sed 's/YYcqwrap/yywrap/g' <lex.cq.c >cql.c
	$(CC) $(CFLAGS) -c cql.c

cqy.o: cqy.y
	$(YACC) $(YFLAGS) cqy.y
	$(CC) $(CFLAGS) -c y.tab.c
	mv y.tab.o $@
	mv y.tab.h colq.h

colquery.h: cqy.o

clean:
	rm -f *.o y.tab.[ch] colq.h colquery.map colq lex.cq.c y.tab.c cql.c *.output cqy.tab.* test

