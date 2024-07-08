formax: enterprise-applications on terminals
============================================

# ALPHA - ALPHA - ALPHA - ALPHA - ALPHA - ALPHA

**formax** is a tool for developing and executing
user-friedly character-based database applications with
extreme efficiency of labor. The basic concepts of Oracle
SQL-Forms 3.0 are combined with curses as the terminal
interface, ODBC for universal database access and JavaScript
as trigger language. The result is the most efficient
solution possible for the end-user. Don't see **formax** as
a romantic nostalgica! The concepts of Oracle's SQL-Forms
are still used today and could be very interesting for
certain use cases on a terminal.

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

Components
==========

**fromax** consists of the following programs, or
components, that you can execute independently from the
command line.

generate
--------

    makeform is a shell script which can produce the sql
    script (.inp) which in turn can create the form database
    (.frm) by using the sqlite3 utility. The produced sql
    script represents in practice the source code of the
    form application and can be edited to change the default
    behaviour or to add objects to the form.

    editform can edit the form database with a form.
    NOT YET IMPLEMENTED.

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

    Regular expression engine for validating user entered
    data.
    See: github.com/kokke/tiny-regex-c

elk
---

    Javascript engine for 3GL triggers.
    See: github.com/cesanta/elk

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
data should be displayed and validated an how an operator
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

When a form is executed **fromax** follows a pre-defined set
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
==========

Navigation is an internal function that is invoked by
specific events. **formax** perfoms navigation primarily to
move the cursor from one location to another. The main
concepts of navigation are the navigation unit and the
cursor. The navigation unit is always defined as a specific
form, block, record or field in the form, or as outside the
form.

Validation
==========

Validation is an internal function that is invoked by
specific events. Validation is the process by which
**formax** determines whether the data in an object is valid
or correct.

Trigger Processing
==================

Events invoke functions, which have trigger points. When
**formax** processes a trigger point, it executes, or fires,
the associated trigger. Every trigger pont has a specific
type of trigger associated with it.

Status
======

Current 0.9.0 is ALPHA and unusable - the generator can only
create a single-block single-page form. runform can display
the page - wait for a key - and exit.

1.0.0 will be able to create, retrieve, update and delete
(CRUD) records in a table by a single-block, single-page,
single-row form. Expected 08/2024.

Feature roadmap
---------------

 - transaction processing
 - validation on field content level
 - multiple blocks and master-detail
 - form for forms - designer module
 - list of values - subforms
 - dev guide en
 - user guide in en, de, fr, ...
 - man pages
 - import script for page layout
 - menue multiple forms
 - cqy.y: resolv 5 shift/reduce conflicts
 - sap-like batchinput
 - triggers with embedded javascript (elk)
 - help screens
 - domains with central maintaining
 - key macros
 - multiple databases
 - database validation (engine responsibility?)

User guide
==========

A form is a fill-in-the-blanks template on your computer
screen that allows you to enter, update, delete and query
informationin a database. Forms are composed of blocks,
records and fields.

Basic tasks
-----------


Playground
----------

    "LineCodePage"="ISO-8859-15:1999 (Latin-9, \"euro\")"

Operations
==========

Install
-------

Fork in github and create codespace or download and extract
https://github.com/freakout42/formax/archive/refs/heads/main.zip
On codespace/debian do the following to build and run the example

    sudo apt-get update
    sudo apt-get -y install unixodbc
    sudo apt-get -y install libsqliteodbc
    sudo apt-get -y install libncurses5-dev
    sudo apt-get -y install flex
    ./configure
    make
    dist/formax/bin/runform -l/usr/lib/x86_64-linux-gnu/odbc/libsqlite3odbc.so dist/formax/lib/scotty.{frm,sq3}

Development
===========

**formax** is implemented in pure C++ without calls to the
OS (except malloc/free). Interface to the user is solely
done with curses-lib. Interface to the database is solely
through ODBC. Both libraries are not used directly but
wrapped by the Screen and Record classes. Record is a simple
ORM inspired by rails ActiceRecord. The form-database is a
sqlite-database. The form generator builds a SQL script that
creates the default form database.

Everything is compiled with -Wall -Werror and memchecked
with valgrind. Only int and char types are used and where
possible static variables are used. Sourcecode lines are
grouped into blocks of maximum 40. These groups have a top
comment which explains the purpose. Other commenting is only
on special cases.

Structure
---------

runform.cpp holds main() and has the only OS-interfaces for
running and checking the command line. record.cpp calls ODBC
and screen.cpp calls curses. All the other sources are pure
C++ without any external library calls. For every main
object is an r-source for reading the form database and a
separate source for the class. The central Function class
holds all the event functions.

Variable names
--------------

One character variable names (pattern like jquery $) are
used for some fundamental values:

  | V | Description            | Type     | Source         |
  | - | ---------------------- | -------- | -------------- |
  | t | char* target with size | Macro    | runform.h      |
  | f | current Form object    | Variable | runform.h      |
  | d | query result data      | Variable | qdata.h        |
  | w | query result pointer   | Method   | qdata.h        |
  | v | query result string    | Type     | qdata.h        |
  | c | query result transfer  | Type     | qdata.h        |
  | n | query result to_int    | Type     | qdata.h        |
  | i | loop integer           | Variable |                |
  | s | status integer         | Variable |                |
  | q | query result object    | Type     | qdata.h        |
  | e | error messages array   | Type     | rerror.h       |
  | d | curses stdscr windows  | Type     | screen.h       |
  | b | blocks array           | Variable | form.h         |
  | l | fields array           | Variable | form.h         |
  | p | pages array            | Variable | form.h         |

Indenting
---------

2 spaces indenting is used and the top level of functions is
not indented. Use 1 space between keyword and opening
bracket. Do not use space between function name and opening
bracket. Opening curly bracket is always at the same line as
keyword (for, while, do, switch, if, ...). Bool is not
compared - no if (ispresent == NULL) or (isempty[0] == '\0')
just use !ispresent or *isempty.

License
=======

Simplified BSD License  
Copyright (c) 2024, Axel K. Reinhold  
All rights reserved.  
