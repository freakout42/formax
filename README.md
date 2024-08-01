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

Install
=======
System preparation:

On codespace/debian/Ubuntu do the following:
~~~
sudo apt-get -y install build-essential
sudo apt-get -y install python
sudo apt-get -y install unixodbc
sudo apt-get -y install libsqliteodbc
sudo apt-get -y install libncurses5-dev
sudo apt-get -y install flex
~~~

On RHEL/CentOS/Oracle/Alma/Rocky do the following:
~~~
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
Then do the following to build and run the example:
~~~
./configure
make
cd dist/formax
bin/runform lib/scotty.frm lib/scotty.sq3
~~~

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

Components
==========
**formax** consists of the following programs, or
components, that you can execute independently from the
command line.

generate
--------
makeform is a shell script which can produce the sql script
(.inp) which in turn can create the form database (.frm) by
using the sqlite3 utility. The produced sql script
represents in practice the source code of the form
application and can be edited to change the default
behaviour or to add objects to the form.

editform
--------
can edit the form layout by an editor and the fields table
database with a form. The editor is called with the .inp
file to make other adjustments before the .frm is recreated.

runform
-------
Execute forms (.frm) interactively on a terminal by the
end-user.

colquery
--------
Parser for a natural query language. Translats human
properties to SQL where clauses.

regex
-----
Regular expression engine for validating user entered data.\
See: github.com/kokke/tiny-regex-c

elk
---
Javascript engine for 3GL triggers.\
See: github.com/cesanta/elk

mex
---
Microemacs editor for editing.\
See: github.com/freakout42/microemacs

Objects
=======
A **formax** application is made up of objects. These
objects contain all the information that is needed for an
application. They have a 1:1 relationship to the tables in
the form-database and to the classes in the C++ source. In
the source-code there is a separate class for every object 
for reading the form-database and for processing.

Forms
-----
The primary object of a **formax** application is the form.
A form is made up of additional objects. These objects link
the form to database elements, such as columns and tables,
and provide control over the flow of execution.

Blocks
------
Describe each section or subsection of the form, and serve
as the basis of default database interaction.

Fields
------
Represent columns or data entry areas and describe how the
data should be displayed and validated and how an operator
should interact with the data while it is entered.

Pages
-----
Are collections of display information, such as constant
text. All fields are displayed on some page.

Triggers
--------
Are sets of processing commands associated with event
points, such as when a particular function key is pressed by
the operator.

Processing
==========
When a form is executed **formax** follows a pre-defined set
of rules for how actions should occur. These actions include
navigation whithin the application and the validation of
data. Within the processing rules, you can customize the
default behaviour to meet the needs of your application.

Events and Functions
--------------------
All processing centers around events. Put simply, events are
things that occur when a form is exeecuted. **formax** knows
about events and handles them by executing functions. Note
that during processing, events are usually nested. That is,
the occurence of one event usually invokes functions that
invoke other events. Inherent in the functions that events
invoke - and that are invoked by your tiggers - are the
processes of navigation and validation.

Trigger Points
--------------
Every function that an event calls might have on or more
trigger point associated with it. A trigger point is
temporal place in an event with a specific trigger type is
associated. Trigger points and triggers are your primary
tools for modifying the way that **formax** handles, or
processes, a particular event.

Navigation
----------

Navigation is an internal function that is invoked by
specific events. **formax** perfoms navigation primarily to
move the cursor from one location to another. The main
concepts of navigation are the navigation unit and the
cursor. The navigation unit is always the field.

Validation
----------
Validation is an internal function that is invoked by
specific events. Validation is the process by which
**formax** determines whether the data in an object is valid
or correct.

Trigger Processing
------------------
Events invoke functions, which have trigger points. When
**formax** processes a trigger point, it executes, or fires,
the associated trigger. Every trigger pont has a specific
type of trigger associated with it.

Development
===========
**formax** is implemented in pure C++. Interface to the user
is solely done with curses-lib. Interface to the database is
solely through ODBC. Both libraries are not used directly
but wrapped by the Screen and Record classes. Record is a
simple ORM inspired by rails ActiceRecord. The form-database
is a sqlite-database. The form generator builds a SQL script
that creates the default form database and can be edited to
change the default behaviour.

Security
--------
There are no calls to the OS (except malloc/free).
Everything is compiled with -Wall -Werror and memchecked
with valgrind. Only int and char types are used and where
possible static variables are used. Everything that comes
from outside is passed to ODBC with bind variables. There is
no free SQL enterable as is was possible with original
SQL-Forms. The password for the database is encrypted on the
command line. Every executed SQL-statement can be logged in
a central database.

License
=======
GNU General Public License _Version 3, 29 June 2007_  
Copyright (c) 2024, Axel K. Reinhold. All rights reserved.
