all: adjfield formax.frm

test: all scotty.sq3 scotty.frm formax.frm formax.sq3 depts.dbf emps.dbf

%.frm: %.inp
	@rm -f $@
	sqlite3 $@ <$<

%.sq3: %.sql
	@rm -f $@
	sqlite3 $@ <$<

scotty.inp: scotty.in2
	cp scotty.in2 $@

scotty.in1: testform makeform
	./testform >$@

formax.sql: formax.frm
	sqlite3 formax.frm .dump >$@

depts.dbf emps.dbf: scotty.ads
	isql Adsf <scotty.ads >/dev/null

clean:
	@rm -f *.frm* scotty.inp scotty.frm* scotty.sq3 ads_* adjfield runform formax.sq? depts.dbf emps.dbf

