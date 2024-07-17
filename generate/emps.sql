alter session set nls_date_format = 'yyyy-mm-dd hh24:mi:ss';
DROP TABLE emps;
CREATE TABLE emps
 (id         NUMBER(11) NOT NULL,
  ename      VARCHAR2(15),
  job        VARCHAR2(12),
  mgr        NUMBER(4),
  hiredate   VARCHAR2(10)/*DATE*/,
  sal        NUMBER(10,2),
  comm       NUMBER(10,2),
  dept_id    NUMBER(4)
 );
ALTER TABLE emps ADD (CONSTRAINT emp_pk PRIMARY KEY (id));
DROP SEQUENCE emp_seq;
CREATE SEQUENCE emp_seq START WITH 8000;
CREATE OR REPLACE TRIGGER emp_bir
BEFORE INSERT ON emps
FOR EACH ROW
BEGIN
  IF :new.id IS NULL THEN
    SELECT emp_seq.NEXTVAL
    INTO   :new.id
    FROM   dual;
  END IF;
END;
/
insert into emps values (7369, 'MC''SMITH',  'CLERK',     7902, '1980-12-17', 800,  NULL, 20);
insert into emps values (7499, 'ALLEN',      'SALESMAN',  7698, '1981-02-20', 1600, 300,  30);
insert into emps values (7521, 'WARD',       'SALESMAN',  7698, '1981-02-22', 1250, 500,  30);
insert into emps values (7566, 'JONES',      'MANAGER',   7839, '1981-04-02', 2975, NULL, 20);
insert into emps values (7654, 'MARTIN',     'SALESMAN',  7698, '1981-09-28', 1250, 1400, 30);
insert into emps values (7698, 'BLAKE',      'MANAGER',   7839, '1981-05-01', 2850, NULL, 30);
insert into emps values (7782, 'CLARK',      'MANAGER',   7839, '1981-06-09', 2450, NULL, 10);
insert into emps values (7788, 'SCOTT',      'ANALYST',   7566, '1987-04-19', 3000, NULL, 20);
insert into emps values (7839, 'KING',       'PRESIDENT', NULL, '1981-11-17', 5000, NULL, 10);
insert into emps values (7844, 'TURNER',     'SALESMAN',  7698, '1981-09-08', 1500, 0,    30);
insert into emps values (7876, 'ADAMS',      'CLERK',     7788, '1987-05-23', 1100, NULL, 20);
insert into emps values (7900, 'JAMES',      'CLERK',     7698, '1981-12-03', 950,  NULL, 30);
insert into emps values (7902, 'FORD',       'ANALYST',   7566, '1981-12-03', 3000, NULL, 20);
insert into emps values (7934, 'MILLER',     'CLERK',     7782, '1982-01-23', 1300, NULL, 10);
