all: adjfield formax.frm

test: all scotty.sq3 scotty.frm

editform: adjfield formax.frm

formax.frm: formax.inp
	rm -f $@
	sqlite3 $@ <$<

scotty.sq3: scotty.sql
	rm -f $@
	sqlite3 $@ <scotty.sql

scotty.frm: scotty.inp
	rm -f $@
	sqlite3 $@ <$<

scotty.inp: testform makeform
	./testform >$@

clean:
	rm -f formax.frm* scotty.inp scotty.frm* scotty.sq3 ads_* adjfield runform

