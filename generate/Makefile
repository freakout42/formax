all: editform

test: all scotty.sq3 scotty.frm formax.frm formax.sq3

editform: adjfield formax.frm

%.frm: %.inp
	rm -f $@
	sqlite3 $@ <$<

%.sq3: %.sql
	rm -f $@
	sqlite3 $@ <$<

#scotty.inp: scotty.in2
#	cp scotty.in2 $@

scotty.inp: testform makeform
	./testform >$@

formax.sql: formax.frm
	sqlite3 formax.frm .dump >$@

clean:
	rm -f *.frm* scotty.inp scotty.frm* scotty.sq3 ads_* adjfield runform formax.sq?

