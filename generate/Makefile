all: scotty.sq3 scotty.frm

scotty.sq3: scotty.sql
	rm -f $@
	sqlite3 $@ <scotty.sql

scotty.frm: scotty.inp
	rm -f $@
	sqlite3 $@ <scotty.inp

scotty.inp: makeform
	./makeform scotty formax1 emp id ename job mgr hiredate sal comm dept_id >$@

../runform/runform:
	( cd ../runform; make runform )

test: scotty.sq3 scotty.frm ../runform/runform
	../runform/runform scotty.frm scotty.sq3

clean:
	rm -f scotty.inp scotty.frm scotty.sq3

