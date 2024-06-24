formax: enterprise-applications on terminals
============================================

ALPHA - ALPHA - ALPHA - ALPHA - ALPHA - ALPHA

**formax** is a tool for developing and executing fast,
powerful and user-friedly character-based interactive
database applications. The basic concepts of Oracle
SQL-Forms 3.0 are combined with curses as the terminal
interface, ODBC for universal database access and the best
ideas for a powerful user interface from Emacs, Clipper, SAP
and Lotus/1-2-3. The result is the most effective solution
possible.

Introduction
============

**formax** forms access the database and generate a screen
that presents the data. The source form (*.inp in practice a
sql-script to create a sqlite3 database) is compiled into a
binary (*.frm in practice a sqlite3 database), that is run
(interpreted) by the runtime module. The form is used to
view and edit data in a database-driven manner. The
environment supplies built-in record creation, query, delete
and update modes, each with its own default data
manipulations. This minimizes the need for program code.

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

TODO

generator
---------

TODO

runform
-------

TODO

colquery
--------

TODO

regex
-----

TODO

elk
---

TODO

docs
----

TODO

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

TODO

Blocks
------

TODO

Fields
------

TODO

Pages
-----

TODO

Triggers
--------

TODO

Processing
==========

TODO

Events
------

TODO

Functions
---------

TODO

Trigger Points
--------------

TODO

Navigation
==========

TODO

Validation
==========

TODO

Trigger Processing
==================

TODO

Database Terms
==============

 - Application
 - Base table
 - Base table field
 - Form database
 - ... TODO

Status
======

Current 0.9.0 is ALPHA and unusable - the generator can only
create a single-block single-page form. runform can display
the page - wait for a key - and exit.

1.0.0 is able to query, update, create and delete records in
a table by a single-block, single-page, single-row form.
Expected 08/2024.

Feature roadmap
---------------

 - current block field
 - CRUD
 - encrypted password for database connection
 - man pages
 - import script for page layout
 - form for forms
 - menue multiple forms
 - cqy.y: resolv 5 shift/reduce conflicts
 - sap-like batchinput
 - triggers with embedded javascript (elk)
 - multiple blocks master-detail
 - help
 - domains with central maintaining
 - dev guide en
 - user guide in en, de, fr, ...
 - trigger
 - direct field keys 1..9 ?
 - key macro

User guide
==========

PuTTY
-----

    "LineCodePage"="ISO-8859-15:1999 (Latin-9, \"euro\")"

Development
===========

**formax** is implemented in pure C++ without calls to the
os. Interface to the user is solely done with curses-lib.
Interface to the database is solely through ODBC. Both
libraries are not used directly but wrapped by the Screen
and Record classes. Record is a simple ORM inspired by rails
ActiceRecord. The form-database is a sqlite-database.

Modules
-------

 - makeform
 - runform
 - colquery
 - tiny-regex-c elk
 - elk

Skills
------

TODO

Resources
---------

TODO

Style
-----

 - structure
 - variable names
 - intenting
 - commenting
 - style
 - if bool

License
=======

Simplified BSD License  
Copyright (c) 2024, Axel K. Reinhold  
All rights reserved.  
