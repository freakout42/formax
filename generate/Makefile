all: editform

test: all scotty.sq3 scotty.frm formax.frm formax.sq3

editform: adjfield formax.frm field.frm trigger.frm

%.frm: %.inp
	rm -f $@
	sqlite3 $@ <$<

%.sq3: %.sql
	rm -f $@
	sqlite3 $@ <$<

scotty.inp: testform makeform
	./testform >$@

formax.sql: formax.frm
	sqlite3 formax.frm .dump >$@

clean:
	rm -f *.frm* scotty.inp scotty.frm* scotty.sq3 ads_* adjfield runform my* formax.sq?

