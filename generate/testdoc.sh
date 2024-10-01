#!/bin/sh
rm -f mydb.sq3 myform.frm
sqlite3 mydb.sq3 <<EOF
create table depts (id integer primary key not null, dname text, loc text);
insert into depts values (10, 'ACCOUNTING', 'NEW YORK');
insert into depts values (20, 'RESEARCH',   'DALLAS');     
insert into depts values (30, 'SALES',      'CHICAGO');
insert into depts values (40, 'OPERATIONS', 'BOSTON');
EOF
ARX=.. ./makeform deptform Departments depts id dname loc | sqlite3 myform.frm
../runform/runform myform.frm mydb.sq3
ARX=.. ./editform myform
../runform/runform myform.frm mydb.sq3
exit
