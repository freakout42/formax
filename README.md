formax: enterprise-applications on terminals
============================================

# ALPHA - ALPHA - ALPHA - ALPHA - ALPHA - ALPHA

**formax** is a tool for developing and executing fast,
powerful and user-friedly character-based, interactive
database applications. The basic concepts of Oracle
SQL-Forms 3.0 are combined with curses as the terminal
interface, ODBC for universal database access and Javascript
as the trigger language. The best ideas for a powerful user
interface from Emacs, Clipper, SAP and Lotus/1-2-3. The
result is the most effective solution possible for the
end-user.

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

generator
---------

TODO

designer
--------

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

Form Terms
==========

 - Application

    A form or a series of forms that satisfies a business function.

 - Base table

    The database table on which a block is based.

 - Base table field

    A field that corresponds to a column in the base table
    of the block that owned the field.

 - Block
 - Constant text
 - Context
 - Designer
 - Field
 - Form
 - Form database
 - Form-level procedure
 - Global variable
 - Login account
 - Multi-record block
 - Object
 - Operator
 - Page
 - Pop-up window
 - Record
 - Scope
 - Screen painter
 - Single-record block
 - System variable
 - Trigger

Database Terms
==============

 - Column
 - Constraint
 - Data dictionary
 - Database
 - DBA
 - Foreign key
 - Index
 - Lock
 - Primary key
 - Row
 - SQL
 - Table
 - Transaction

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

 - import block/field config
 - current block field
 - CRUD
 - man pages
 - import script for page layout
 - multiple blocks master-detail
 - form for forms
 - menue multiple forms
 - cqy.y: resolv 5 shift/reduce conflicts
 - sap-like batchinput
 - triggers with embedded javascript (elk)
 - help
 - domains with central maintaining
 - dev guide en
 - user guide in en, de, fr, ...
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
OS. Interface to the user is solely done with curses-lib.
Interface to the database is solely through ODBC. Both
libraries are not used directly but wrapped by the Screen
and Record classes. Record is a simple ORM inspired by rails
ActiceRecord. The form-database is a sqlite-database.

Modules
-------

 - makeform
 - runform
 - colquery
 - regex
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
 - indenting
 - commenting
 - style
 - bool

License
=======

Simplified BSD License  
Copyright (c) 2024, Axel K. Reinhold  
All rights reserved.  
