CFLAGS=-DYY_NO_UNPUT -DYY_SKIP_YYWRAP $(DEBUG)
LDFLAGS=-r
YFLAGS=-Wno-conflicts-sr -d --name-prefix=cq
LEXFLAGS=--prefix=cq

all: colq
	./colq "2 a" 3-7

colq: colq.c colquery.o yyerror.o
	$(CC) $(CFLAGS) -DYYDEBUG colq.c colquery.o yyerror.o -o $@

colquery.o: colquery0.o parse.o cql.o cqy.o
	$(LD) $(LDFLAGS) colquery0.o parse.o cqy.o cql.o -o $@

colquery0.o: colquery.c colquery.h
	$(CC) $(CFLAGS) -c colquery.c -o $@

cql.o: cql.l cqy.o
	$(LEX) $(LEXFLAGS) cql.l
	sed 's/YYcqwrap/yywrap/g' <lex.cq.c >cql.c
	$(CC) $(CFLAGS) -c cql.c

cqy.o: cqy.y
	$(YACC) $(YFLAGS) cqy.y
	$(CC) $(CFLAGS) -c y.tab.c
	mv y.tab.o $@
	mv y.tab.h colquery.h

colquery.h: cqy.o

clean:
	rm -f *.o y.tab.[ch] colquery.h colquery.map colq lex.cq.c y.tab.c cql.c *.output cqy.tab.*

