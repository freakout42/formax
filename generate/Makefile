all: scotty.sq3 scotty.frm

scotty.sq3: scotty.sql
	rm -f $@
	sqlite3 $@ <scotty.sql

scotty.frm: scotty.inp
	rm -f $@
	sqlite3 $@ <scotty.inp

scotty.inp: testform makeform
	./testform >$@

clean:
	rm -f scotty.inp scotty.frm scotty.sq3 ads_*

