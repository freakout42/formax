create table depts (id Numeric(4,0), dname Char(12), loc Char(30));
insert into depts values (10, 'ACCOUNTING', 'NEW YORK');
insert into depts values (20, 'RESEARCH',   'DALLAS');
insert into depts values (30, 'SALES',      'CHICAGO');
insert into depts values (40, 'OPERATIONS', 'BOSTON');
create table emps (id Numeric(4,0), ENAME Char(15), JOB Char(12), MGR Numeric(4,0), HIREDATE Date, SAL Numeric(10,2), COMM Numeric(10,2), DEPT_ID Numeric(4,0));
insert into emps values (7369, 'MC''SMITH',  'CLERK',     9702, '1980-12-17', NULL, 0, 20);
insert into emps values (7499, 'ALLEN',      'SALESMAN',  7698, '1981-02-20', 1600, 0, 30);
insert into emps values (7521, 'WARD',       'SALESMAN',  7698, '1981-02-22', 1250, 1, 30);
insert into emps values (7566, 'JONES',      'MANAGER',   7839, '1981-04-02', 2975, 0, 20);
insert into emps values (7654, 'MARTIN',     'SALESMAN',  7698, '1981-09-28', 1250, 0, 30);
insert into emps values (7698, 'BLAKE',      'MANAGER',   7839, '1981-05-01', 2850, 1, 30);
insert into emps values (7782, 'CLARK',      'MANAGER',   7839, '1981-06-09', 2450, 0, 10);
insert into emps values (7788, 'SCOTT',      'ANALYST',   7566, '1987-04-19', 3000, 1, 20);
insert into emps values (7839, 'KING',       'PRESIDENT', NULL, '1981-11-17', 5000, 1, 10);
insert into emps values (7844, 'TURNER',     'SALESMAN',  7698, '1981-09-08', 1500, 0, 30);
insert into emps values (7876, 'ADAMS',      'CLERK',     7788, '1987-05-23', 1100, 1, 20);
insert into emps values (7900, 'JAMES',      'CLERK',     7698, '1981-12-03', 950,  0, 30);
insert into emps values (7902, 'FORD',       'ANALYST',   7566, '1981-12-03', 3000, 0, 20);
insert into emps values (7934, 'MILLER',     'CLERK',     7782, '1982-01-23', 1300, 0, 10);