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

    A logical collection of fields in a form. A block can
    correspond to one table in the database or to no table.

 - Constant text

    Text that appears in a form time it is run.

 - Context

    A concept that you can use to determine what parts of a
    form you can currently access from within the interface.

 - Designer

    An application developer or programmer who uses
    **formax** to create and modigy forms.

 - Field

    An area on a page that can display data and accept
    operator input. The data that they display can
    correspond to data from a column in a database table.

 - Form

    A logical collection of blocks, fields and triggers.

 - Form database

    A sqlite-database containing all the objects that make
    up the form.

 - Multi-record block

    A block that can display more than one record at a time.

 - Object

    A group of data, such as a form, block, field or trigger.

 - Operator

    An end-user of a **formax** application.

 - Page

    A collection of display information. Similar in concept
    to a slide Projection, a page displays fields and
    constant text to operators on their computer or terminal
    screen when they execute a form.

 - Pop-up window

    A **formax** object that overlays an area of the current
    display. The form displays a pop-up window in response
    to some event or user action.

 - Record

    Data from one row in a database table or view, as
    presented in a form.

 - Scope

    The domain, or range, in which a trigger operates. This
    domain is determined by the level (form, block, or field)
    at which you define the trigger.

 - Single-record block

    A block that can display only one record at a time.

 - Trigger

    A piece of logic that is executed at, or "tiggered" by,
    a form event.

Database Terms
==============

 - Column

    In a database table, a "vertical" group of cells that
    represent one kind of data.

 - Constraint

    A rule or restriction concerning a piece of data that is
    enforced at the data level, rather than the object or
    application level.

 - Database

    A collection of tables controlled by one data dictionary.

 - Foreign key

    A value or column in one table that refers to a primary
    key in another table.

 - Index

    An optional structure associated with a table that is
    used by the database software to locate rows of the
    table quickly, and optionally to guarantee that every
    row is unique.

 - Lock

    A restriction that assigns temporary ownership, or
    control, of a database resource (table or row) to a
    user. A lock can prevent other users from changing data
    that a user is updating.

 - Primary key

    Information used to identify a row in a table. Also
    known as a key.

 - Row

    In a database table, a "horizontal" group of column
    values.

 - Table

    The basic unit of information in a relational database
    management system. A table is a two dimensional grid
    that is made up of rows and columns.

 - Transaction

    A logical unit of work. Specifically, a transaction is
    the group of events that occurs between the data changes
    and the user commiting them.

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

