formax: enterprise-applications on terminals
============================================
**formax** is a tool for developing and executing
user-friedly character-based database applications with
extreme efficiency. The concept of Oracle Forms is combined
with curses as the terminal interface, ODBC for universal
database access and JavaScript for the trigger language. The
result is the most efficient solution possible for the
end-user. The applications can also be build with minimal
effort using a nocode approach. Click on the image for a
non-technical description:

[![formax](https://formax.freakout.de/assets/logo.jpg "formax")](https://formax.freakout.de/ "formax")

Introduction
============
**formax** accesses the database and generate a screen that
presents the data. The source form (.inp in practice a
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

**formax** encorporates the concept of Oracle Forms with
modern standards and current database best practices and
technologies including all required enterprise-features:
 - single-column autoincrement primary key field
 - database integrity enforced through database
 - JavaScript for client scripting
 - regular expressions for validation
 - freedom to choose the database provider
 - multiple differnt databases simultaneously accessible
 - full logging of all queries in a single place
 - no free SQL for the end-user
 - no operating system exits in the form-engine
 - 3FA security can be setup easily

Status
======
Stable 1.5.0 is useable for single-block single-page forms.
Don't hesitate to create any issues when in trouble.

Feature roadmap
---------------
 - 2.0 triggers
 - 2.2 multiple databases
 - 2.5 in-field editing
 - 3.0 multi-record fields
 - 3.1 multiple blocks and master-detail
 - 3.5 list of values - subforms
 - 4.0 menue multiple forms
 - 4.5 transaction processing
 - cqy.y: 5 shift/reduce conflicts
 - sap-like batchinput
 - key macros

Playground
----------
You can play with an example form simply by connecting ssh
to *formax.toarx.de* with user *scott* and password *tiger*.
If you are using PuTTY you must set for the session:

    Window->Translation->Remote character set = ISO-8859-15
    Connection->Data->Terminal-type string = putty

Install
=======
Fork in github and create codespace or download and extract
https://github.com/freakout42/formax/archive/refs/heads/main.zip  

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

Then do the following to build and install **formax** into
`/opt/arx/`. Nothing outside `/opt/arx/` will be touched in
any way by the installation with the exception of
`/etc/profile.d/arx.sh` which only adds `/opt/arx/bin` to
the PATH.

~~~
./configure
make
sudo make install
. /etc/profile.d/arx.sh
~~~

Getting started
===============
The following will guide you to creation, execution and
maintenance of a form by using the components of **formax**.
`makeform` (create), `runform` (execute) and `editform`
(maintain). First change into a new directory where your
development will live. Create a new database including the
table `depts` with:

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
a default form for the table `depts` and execute it:

~~~
makeform deptform Departments depts id dname loc | sqlite3 myform.frm
runform myform.frm mydb.sq3
~~~

You will be in `Insert-Mode` and can enter data for new
records. Press Enter without new data will get you into
`Query-Mode`. Quit from the form after playing around with
Ctrl-Y. Read the user guide from the docs section for more
information how to use the form.

Change the form layout and the default behaviour of the form
with the `editform` utility. This will bring up your editor
with the layout of the form and a form to maintain the field
behaviour and at least the editor again with the full
`.inp-file` for your form. You can test your changes by
running runform again:

~~~
editform myform
runform myform.frm mydb.sq3
~~~

Read the developer guide from the docs section for more
information how to modify the form. Have fun and profit.

Guides
======

[User Guide](https://formax.freakout.de/UserGuide.html "User Guide")

License
=======
GNU General Public License _Version 3, 29 June 2007_  
Copyright (c) 2024, Axel K. Reinhold. All rights reserved.
