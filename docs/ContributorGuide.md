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

Structure
---------
runform.cpp holds main() and has the only OS-interfaces for
running and checking the command line. record.cpp calls ODBC
and provides an interface in an ORM style like rails
active-record. screen.cpp calls curses. All the other
sources are pure C++ without any external library calls. For
every main object is an r-source for reading the form
database and a separate source for the class. The central
Function class holds all the event functions.

Variable names
--------------
One character variable names (pattern like jquery $) are
used for some fundamental values:

  | V | Description            | Type     | Source         |
  | - | ---------------------- | -------- | -------------- |
  | b | blocks array           | Variable | form.h         |
  | c | query result transfer  | Type     | qdata.h        |
  | d | query result data      | Variable | qdata.h        |
  | e | error messages array   | Type     | rerror.h       |
  | f | current Form object    | Variable | runform.h      |
  | i | loop integer           | Variable |                |
  | g | logger                 | Variable | logger.h       |
  | l | fields array           | Variable | form.h         |
  | n | query result to_int    | Type     | qdata.h        |
  | p | pages array            | Variable | form.h         |
  | q | query result object    | Type     | qdata.h        |
  | r | trigger array          | Variable | form.h         |
  | s | status integer         | Variable |                |
  | t | char* target with size | Macro    | runform.h      |
  | v | query result string    | Type     | qdata.h        |
  | w | query result pointer   | Method   | qdata.h        |
  | y | curses stdscr window   | Type     | screen.h       |

Coding Style
------------
Sourcecode lines are grouped into blocks of maximum 40 lines.
These groups have a top comment which explains the purpose.
Other commenting is only on special cases. Code should be
short and self explaining by variable names.

2 spaces indenting is used and the top level of functions is
not indented. Use 1 space between keyword and opening
bracket. Do not use space between function name and opening
bracket. Opening curly bracket is always at the same line as
keyword (for, while, do, switch, if, ...). Bool is not
compared - no if (ispresent == NULL) or (isempty[0] == '\0')
just use !ispresent or *isempty. Omit curly brackets after
compound statements when possible.

