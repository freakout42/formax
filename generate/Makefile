all: adjfield formax.frm trgr.frm

test: all scotty.sq3 scotty.frm

editform: adjfield formax.frm trgr.frm

%.frm: %.inp
	rm -f $@
	sqlite3 $@ <$<

scotty.sq3: scotty.sql
	rm -f $@
	sqlite3 $@ <scotty.sql

scotty.inp: testform makeform
	./testform >$@

clean:
	rm -f formax.frm* trgr.frm* scotty.inp scotty.frm* scotty.sq3 ads_* adjfield runform

