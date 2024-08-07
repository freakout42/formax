formax: enterprise-applications on terminals
============================================

**formax** is a tool for developing and executing
user-friedly character-based database applications with
extreme efficiency. The basic concepts of Oracle SQL-Forms
3.0 are combined with curses as the terminal interface, ODBC
for universal database access and JavaScript as trigger
language. The result is the most efficient solution possible
for the end-user. The applications can also be build with
minimal effort using a nocode approach.  
Click on the image for a non-technical description:

[![formax](https://formax.freakout.de/assets/logo.jpg "formax")](https://formax.freakout.de/ "formax")

Introduction
============
**formax** forms access the database and generate a screen
that presents the data. The source form (.inp in practice a
sql-script to create a sqlite3 database) is compiled into a
binary (.frm in practice a sqlite3 database), that is run by
the runtime component. The form is used to view and edit
data in a database-driven manner. The environment supplies
built-in record creation, query, delete and update modes,
each with its own default data manipulations. This minimizes
the need for program code. **formax** can be seen as a
"No-code RAD" (Rapid Application Development).

The running form has an event driven interface and
implements event-handling functions called triggers which
are automatically invoked at critical steps in the
processing of records and the receipt of keyboard strokes.
Different triggers may be called before, during, and after
each critical step. Each event function is initially a stub,
containing a default action or nothing. Programming
therefore generally consists of modifying the contents of
these triggers in order to alter the default behavior if
necessary.

**formax** encorporates the concept of Oracle SQL-Forms with
modern standards and current database best practices and
technologies:
 - single-column autoincrement primary key field
 - database integrity enforced through database
 - JavaScript for client scripting
 - regular expressions for validation
 - no free SQL for the end-user
 - freedom to choose the database provider
 - full logging of all queries in single place

Status
======
Current 1.0.0 is BETA - the generator can only create a
single-block single-page form. You are able to create,
retrieve, update and delete (CRUD) records in this table.

Feature roadmap
---------------
 - multi-record fields
 - TOTP 3FA
 - transaction processing
 - in-field editing
 - editor
 - checksumming form with password
 - multiple blocks and master-detail
 - list of values - subforms
 - menue multiple forms
 - cqy.y: 5 shift/reduce conflicts
 - sap-like batchinput
 - field help
 - key macros
 - multiple databases
 - full SQL-Forms-3 functionality

Playground
----------
You can play with an example form simply by connecting ssh
to *formax.toarx.de* with user *scott* and password *tiger*.
If you are using PuTTY you must set for the session:

    Window->Translation->Remote character set = ISO-8859-15
    Connection->Data->Terminal-type string = putty

For maximum convenience you can add the following to your
registry. Then you will find a loadable session which you
can open in PuTTY or start with "putty -load formax" from
the command line.

~~~
REGEDIT4

[HKEY_CURRENT_USER\SOFTWARE\SimonTatham\PuTTY\Sessions\formax]
"Present"=dword:00000001
"HostName"="formax.toarx.de"
"TerminalType"="putty"
"UserName"="scott"
"LineCodePage"="ISO-8859-15:1999 (Latin-9, \"euro\")"

~~~

Install
=======
System preparation is needed to install the dependencies:

On codespace/debian/Ubuntu do the following:
~~~
sudo apt-get -y update
sudo apt-get -y install build-essential
sudo apt-get -y install python
sudo apt-get -y install unixodbc
sudo apt-get -y install libsqliteodbc
sudo apt-get -y install libncurses5-dev
sudo apt-get -y install flex
~~~

On RHEL/CentOS/Oracle/Alma/Rocky do the following:
~~~
sudo yum -y update
sudo yum -y group install "Development Tools"
sudo yum -y install python3
sudo yum -y install unixODBC
sudo yum -y install unixODBC-devel
sudo yum -y install ncurses-devel
sudo yum -y install sqlite
rpm -i https://github.com/freakout42/formax/releases/download/v0.9.9/sqliteodbc-0.9998-1.x86_64.rpm
~~~

Fork in github and create codespace or download and extract
https://github.com/freakout42/formax/archive/refs/heads/main.zip  
Then do the following to build and install into `/opt/arx/`.
Nothing outside `/opt/arx/` will be touched in any way by
the installation with the exception of /etc/profile.d/arx.sh
which only adds `/opt/arx/bin` to the PATH.

~~~
./configure
make
sudo make install
. /etc/profile.d/arx.sh
~~~

Getting started
===============

The following wil guide you to creation, execution and
maintenance of a form by using `makeform` (create),
`runform` (execute) and `editform` (maintain). First change
into a new directory where your development will live.
Create a new database including the table `depts` with:

~~~
sqlite3 mydb.sq3
create table depts (id integer primary key not null, dname text, loc text);
insert into depts values (10, 'ACCOUNTING', 'NEW YORK');
insert into depts values (20, 'RESEARCH',   'DALLAS');
insert into depts values (30, 'SALES',      'CHICAGO');
insert into depts values (40, 'OPERATIONS', 'BOSTON');
.quit
~~~

You should now have a sqlite-database in `mydb.sq3`. Create
a default form for the table `depts` and execute it.

~~~
makeform deptform Departments depts id dname loc | sqlite3 myform.frm
runform -i myform.frm mydb.sq3
~~~

Use the following procedures for CRUD-actions on the table:

~~~
Edit:     Edit with "Left/Right" and any characters
Commit:   Press "Enter" to save the changes
Retrieve: Edit conditions in each field and "Enter"
Create:   Press "Insert" or Ctrl-o
Update:   Move to the record and field with
          "Up/Down" and "Tab" and edit the field
Delete:   Move to the record
          Press "Delete" or Ctrl-k
~~~

License
=======
GNU General Public License _Version 3, 29 June 2007_  
Copyright (c) 2024, Axel K. Reinhold. All rights reserved.
