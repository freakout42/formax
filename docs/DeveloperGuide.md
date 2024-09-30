What is **formax**?
===================

**formax** is a "no-code" development framework for forms
based applications. These applications can enter, query,
update and delete data in any database which has an
ODBC-driver. With no programming simply by assigning form
fields to database tables and columns a working form can be
build within 5 minutes without any coding.

The forms run within a terminal. They are full-screen,
interactive and keyboard driven. Any terminal with a
terminfo (ncurses) database is supported with usable
function keys and color support.

The system is enterprise-ready. Security, reliability, cost
and operational excellence are fully covered and the
important properties for developers - performance and
sustainability are on top with C++ and open source.

Terms
=====

| Term             | Explanation                            |
|------------------|----------------------------------------|
| Application      | forms that satisfy a business function |
| Base table       | database table a block is based on     |
| Base table field | field that corresponds to a column in the base table of the fields block     |
| Block            | logical collection of fields in a form corresponds to table in the database   |
| Constant text    | text that appears in a running form    |
| Context          | concept that you can use to determine what parts of a form you can access    |
| Designer         | application developer or programmer    |
| Field            | area on a page that can display data and accept user input                  |
| Form             | collection of blocks, fields, triggers |
| Form database    | sqlite-database containing the form    |
| Multirecord block| block with more than one record        |
| Object           | group of data, such as a form, block, field or trigger                       |
| Operator         | administrator of an application responsible for a stable, performant and secure runtime environment         |
| Page             | collection of display information      |
| Pop-up window    | overlays an area of the current display displays in response to user action    |
| Record           | data from one row in a database table  |
| Scope            | domain in which a trigger operates determined by the level (form, block, or field) of the trigger definition    |
| Trigger          | piece of logic executed at a form event|
| User             | end-user of a **formax** application   |
| Column           | "vertical" group of cells in a database|
| Constraint       | rule or restriction concerning a piece of data that is enforced at data level, rather than the application level      |
| Database         | collection of tables                   |
| Foreign key      | column that refers to a primary key    |
| Index            | optional structure of a table that is used by the database software to locate rows quickly and optionally guarantee that every row is unique               |
| Lock             | restriction that assigns control of a resource to a user - can prevent other users from changing data meanwhile     |
| Primary key      | information used to identify a row     |
| Row              | "horizontal" group of cells in database|
| Table            | basic unit of information in database 2 dimensional grid of rows and columns |
| Transaction      | logical unit of work                   |

Programs
========

**formax** contains the following programs, that you can
execute independently from the command line:

 - makeform

is a shell script which can produce a form for a table with
default functionality. The input parameters are just the table
and column names - the output is a sql script (.inp). This
script can create the form database (.frm). In practice the
produced sql script represents the source code of the form.
You edit the script to change the default behaviour or to
add objects to the form. The form database is all you need
to run the application.

 - editform

is another shell script. It offers a more convenient way to
change a form by three steps. First it extracts the
boilerplate from the main page and loads it into your
preferred editor. Edit the form layout and the field
positions and lengths with a WYSIWYG concept. The edited
file is inserted back to the form. The next step is running
a form to modify all the fields properties, which makes up a
great portion of any form development work. Last the editor
is called with the .inp file to make other fine adjustments
before the .frm is recreated.

 - runform

is the heart of the system. This single executable runs
forms (.frm) interactively on a terminal. It needs only two
parameters to run - the form database file and the ODBC-DSN.
It is run by the end-user typically wrapped into a shell
script within his ssh startup command.

Objects
=======

A **formax** application is made up of objects. These
objects contain all the information that is needed for an
application. They have a 1:1 relationship to the tables in
the form-database and to the classes in the C++ source. In
the source-code there is a separate class for every object 
for reading the form-database and for processing.

Form
----

The primary object of a **formax** application is the form.
A form is made up of additional objects. These objects link
the form to database elements, such as columns and tables,
and provide control over the flow of execution.

Block
-----

Describe each section or subsection of the form, and serve
as the basis of default database interaction.

Field
-----

Represent columns or data entry areas and describe how the
data should be displayed and validated and how an operator
should interact with the data while it is entered.

Page
----

Are collections of display information, such as constant
text. All fields are displayed on some page.

Trigger
-------

Are sets of processing commands associated with event
points, such as when a particular function key is pressed by
the operator.

Processing
==========

When a form is executed runform follows a pre-defined set
of rules for how actions should occur. These actions include
navigation whithin the application and the validation of
data. Within the processing rules, you can customize the
default behaviour to meet the needs of your application.

Status and mode
---------------

On top of the screen runform displays a status line. The
status line indicates the current form, block, field, record
and mode. The current mode determines the action that will
be processed with the current data (block, field, record) by
using the keyboard. There are four main modes for the four
main actions and the special Edit-mode for entering data.

| Mode             | Action                                |
|------------------|---------------------------------------|
| Insert-Mode      | Create   -> Enter new records/data    |
| Query-Mode       | Retrieve -> Search in database        |
| Update-Mode      | Update   -> Display and change data   |
| Delete-Mode      | Delete   -> Delete records/data       |
| Edit-Mode        |             Edit data                 |

Events and Functions
--------------------

All processing centers around events. Put simply, events are
things that occur when a form is exeecuted. runform knows
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
specific events. runform perfoms navigation primarily to
move the cursor from one location to another. The main
concepts of navigation are the navigation unit and the
cursor. The navigation unit is always the field.

Validation
----------

Validation is an internal function that is invoked by
specific events. Validation is the process by which runform
determines whether the data in an object is valid or
correct.

Trigger Processing
------------------

Events invoke functions, which have trigger points. When
runform processes a trigger point, it executes, or fires,
the associated trigger. Every trigger pont has a specific
type of trigger associated with it.

Development
===========

In version 1.x developing with **formax** is kind of a
rudimentary	task only supported by a simple script
`editform`. Version 1 first attention applys to user
experience and documentation. Anything beyond layout editing
and field properties has to be done by editing the .inp
file and needs knowledge from the source code or adapted
experience from Oracle Forms jobs. Since 1.x can only handle
one block, table and page this is not a big deal anyway.
Follow the "Getting started" instruction in the README first
to get the basic idea for development quickly.

Page layout
-----------

The `editform` script can extract the pages boilerplate
(background) into your preferred editor. The placeholder for
the fields are `$nn___` with nn as the field number and the
`_`s imply the display length of the field. A trailing `.
` means the length is only 1 character (for binary 0/1
fields). The edited text file is incorporated into the form
database after writing.

Field properties
----------------

After the layout `editform` brings up runform itself with a
form for the forms fields. The effect of the properties is
as follows:

| Property | Effect for runform                            |
|----------|-----------------------------------------------|
| Id       | primary key   - DO NOT CHANGE                 |
| form_id  | form  id = 1  - DO NOT CHANGE                 |
| blkn     | block id = 1  - DO NOT CHANGE                 |
| pagn     | page  id = 1  - DO NOT CHANGE                 |
| name     | name corresponds to column name of the table  |
| Help     | help text                                     |
| Seq      | sequence for user navigation                  |
| Line     | line position in layout (*1)                  |
| Col      | column position in layout (*1)                |
| Dlen     | display length in layout (*1)                 |
| Type     | column type 0:ALL 1:CHAR 2:INT 3:FLOAT 4:DATE |
| Length   | column length                                 |
| Table    | 0=no database column 1=database column        |
| Pkey     | primary key   - DO NOT CHANGE                 |
| Default  | default value in Insert-Mode                  |
| Enter    | allowed to edit in Insert-Mode                |
| Query    | allowed to edit in Query-Mode                 |
| Update   | allowed to edit in Update-Mode                |
| UpdateNul| allowed to edit in Update-Mode when NULL      |
| Mandatory| not NULL in Insert-Mode                       |
| Uppercase| convert to uppercase                          |
| Low      | minimum value                                 |
| High     | maximum value                                 |
| Pattern  | regular expression (pcre)                     |
| ListOfVal| title                                         |

(*1) This property is adjusted by the `editform` script upon
     the field placeholders from the boilerplate text file.

.inp file edit
--------------

At the end `editform` brings up the editor with the .inp
file loaded. Look into the source code and into the table
schemas for informations to alter the forms behaviour.

Trigger programming
===================

Elk is a tiny embeddable JavaScript engine that implements a
small but usable subset of ES6.

## Supported features

- Operations: all standard JS operations except:
   - `!=`, `==`. Use strict comparison `!==`, `===`
   - No computed member access `a[b]`
   - No exponentiation operation `a ** b`
- Typeof: `typeof('a') === 'string'`
- For loop: `for (...;...;...)  ...`
- Conditional: `if (...) ... else ...`
- Ternary operator `a ? b : c`
- Simple types: `let a, b, c = 12.3, d = 'a', e = null, f = true, g = false;`
- Functions: `let f = function(x, y) { return x + y; };`
- Objects: `let obj = {f: function(x) { return x * 2}}; obj.f(3);`
- Every statement must end with a semicolon `;`
- Strings are binary data chunks, not Unicode strings: `'Київ'.length === 8`

## Not supported features

- No `var`, no `const`. Use `let` (strict mode only)
- No `do`, `switch`, `while`. Use `for`
- No `=>` functions. Use `let f = function(...) {...};`
- No arrays, closures, prototypes, `this`, `new`, `delete`
- No standard library: no `Date`, `Regexp`, `Function`, `String`, `Number`

Note: Elk uses `snprintf()` standard function to format numbers (double).
On some architectures, for example AVR Arduino, that standard function does
not support float formatting - therefore printing numbers may output nothing
or `?` symbols.
