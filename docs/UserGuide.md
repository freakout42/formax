What is a form?
===============

A form is a fill-in-the-blanks template on your computer
screen that allows you to enter, update, delete and query
information in a database. Forms are composed of blocks,
records and fields.

Forms and the database
======================

When you run **formax** you will be entering, modifying, or
deleting data contained in a database. The database
organizes data into tables of related information. These
tables ar made up of rows and columns (like a spreadsheet).

When you retrieve data into a form (execute a query), each
record that is displayed comes from a row in a table.
Therefore, if you enter, modify, or delete a record, your
action causes a corresponding row in table to be entered,
modified, or deleted.

Screen (output)
===============

A typical **formax** screen displays explantory text and
fields that accept data values. At the top of the screen is
a status line that can also display messages and acts as the
place for entering and updating data.

Status line
-----------

From left to right, the status line contains:
~~~
 (1) (2)    (3)  (4)  (5) (6)(7) (8)    (9) (10)
   1-scotty axel emps id  1/  14 Update Ins Enter-Query
 (1) Form number
 (2) Form name
 (3) Logged-in user
 (4) Block name
 (5) Field name
 (6) Active record number
 (7) Number of records retrieved by a query
 (8) Mode (Insert, Query, Update, Delete)
 (9) Character mode (Ins or Rep)
(10) Action for the Enter/Commit key
~~~

Status and mode
===============

As the status line indicates, the form has always a current
form, block, field, record and mode. The current mode
determines the action that will be processed with the current
data (block, field, record) by using the keyboard.

Keyboard (input)
================

When you are running **formax**, the keyboard is the only
input method for the form. There is no slow menu, mouse or
touch interface - the fingers never leave the keyboard -
this is the reason for the unmatched efficiency and
ergonomics. You will press certain keys to move the cursor,
enter or modify data, or signal your computer to store
information. The keys can be grouped into the following
categories:

 - Modifier = [Shift] [Ctrl]

    These keys act as modifier for other keys by holding
    them while pressing other keys. The [Shift] key modifies
    to the uppercase or the alternate character of the key.
    The [Ctrl] key modifies to the control character of the
    [A-Z] keys.

 - Alphanumeric = [A-Z] [0-9]

    In Edit-Mode these keys add characters at the cursor
    position - depending on the character mode the new
    character replaces or inserts at the cursor position.
    In any other mode the Edit-Mode is started by replacing
    the fields text with the character at first position.

 - Special character = []()*&^%!",;: ...

    Same behaviour as alphanumeric keys with the exception
    of the shifted [1-9] keys which act as speed navigation
    in non Edit-Mode for the respective fields.

 - Navigation = [Left] [Right] [Up] [Down] [PageUp] [PageDn]
                [Home] [End] [Tab] [Backtab]

    These keys navigate through the form changing the
    current item. [Left] and [Right] move the cursor in Edit-
    Mode and enter Edit-Mode for the current field in any
    other mode. [Left] positions the cursor at the end and
    [Right] at the beginning of the fields text. In
    contrast to enter a character key which erases the
    text (see above) this allows to change it.
    [Tab] and [Backtab] move the cursor to the next
    respectively previous enterable field in the current
    record. [Up] [Down] [PageUp] [PageDn] move the cursor to
    next respectively previous record or set of records in
    the current block. 

 - Function = [F1-10] [Ins] [Del] [Backspace]

    [Backspace] Marks the current record as deleted and
          starts Delete-Mode. In Edit-Mode the character
          before the cursor is deleted.

    [F1]  Displays a brief message for the current field.

    [F2]  Copies the text of the current field into the
          paste buffer for later use with [F3].

    [F3]  Pastes text in paste buffer at current cursor
          location.

    [F4]  Activates a list of values, if there is on
          available for this field.

    [F5]  Copies all field values from the previous record
          into a new record.

    [F6|Ins] Inserts a new record after the current record
          and starts Insert-Mode. In Edit-Mode the character
          mode is toggled.

    [F7|Del] Marks the current record as deleted and starts
          Delete-Mode. In Edit-Mode the current character is
          deleted.

    [F8]  Exits the current form and returns to the system
          after committing outstanding changes.

    [F9]  Exits the current form and returns to the system
          after canceling outstanding changes.

    [F10] Enter Query-Mode allowing to enter query criteria.

 - Action = [Enter] [Esc]

    [Enter] acts upon the current mode in committing manner.
    The current action for the [Enter] key is always shown
    in the rightmost position (10) of the status line.
    In Edit-Mode [Enter] saves edited text into the field.
    [Esc] cancels the current changes in Edit-Mode and
    returns to Update-Mode without action in Insert- and
    Delete-Mode.

Basic tasks
===========

Editing a field - Edit-Mode
---------------------------

In Edit-Mode the current fields text is displayed in the
status line and can be modified. The current fields text is
still visible on the screen. To cancel the changes press
[Esc]. Edit-Mode is activated by pressing any character key
with new text and by pressing [Left] or [Right] to modify
the text from the right or left respectively. Any
navigational key or [Enter] will save the text to the field.

Retrieving records - Query-Mode
-------------------------------

Press [F10] to activate Query-Mode or simply [Enter] in
Update-Mode. It is clearly signaled with turquois background
fields. Edit the text in the respective fields that should
match the data you want to retrieve. You can use exact
values like numbers (ie: 309) or text (ie: Hank), or comma
separated lists (ie: 309,507,1003) or ranges (ie:
2000-4000). Tilde prefix will search for textparts (ie:
~mountain). [Enter] will start the query and goes into
Update-Mode.

Modifying a record - Update-Mode
--------------------------------

In Update-Mode which is activated after a search you can
browse through the retrieved records with the [Up] [Down]
[PageUp] [PageDn] keys. It is signaled with magenta
background fields. To navigate between the fields use the
[Tab] and [Backtab] and the speed keys [Shift+1-9]. To edit
the current field (blue background) start typing character
keys or use [Left] or [Right] to move the cursor in the
text. [Enter] will save the text to the field. [Esc] cancels
the changes.

Inserting a record - Insert-Mode
--------------------------------

Since this is the major used action most forms start in this
mode. It is clearly signaled with yellow background fields.
Press [F6|Ins] to activate Insert-Mode manually with a new
record. [Enter] will save the new record. [Esc] cancels the
changes. The [F3] and [F4] keys copy data from a previous
record.

Deleting a record - Delete-Mode
-------------------------------

Press [F7|Del] to activate Delete-Mode. It is clearly
signaled with red-blinking background fields. [Enter] will
delete the record. [Esc] cancels the action.

Keyboard alternative [Ctrl] keys
================================

Every Navigation-, Function- and Action-Key has an
alternative [Ctrl+A-Z] key. In case of a missing special key
on a specific keyboard or when you prefer to use the
character keys, these can be used. In this table you find
also the keys action in Edit-Mode if it has a different
action there.

| Key         | Ctrl+    | Function            | Edit-Mode
|-------------|----------|---------------------|------------------
| [F1]        | [Ctrl+@] | Help                |
| [Home]      | [Ctrl+a] | Previous block      | Beginning of line
| [Left]      | [Ctrl+b] | Edit field from end | Previous char
| [F2]        | [Ctrl+c] | Copy                |
| [Del]       | [Ctrl+d] | Delete record       | Delete char
| [End]       | [Ctrl+e] | Next block          | End of line
| [Right]     | [Ctrl+f] | Edit field          | Next char
| [Backtab]   | [Ctrl+g] | Previous field      |
| [Backspace] | [Ctrl+h] | Delete record       | Delete left char
| [Tab]       | [Ctrl+i] | Next field          |
| [Ins]       | [Ctrl+j] | Insert record       | Toggle Ins/Rep
| [F7|Del]    | [Ctrl+k] | Delete record       | Delete char
| [F0]        | [Ctrl+l] | Refresh             |
| [Enter]     | [Ctrl+m] | Accept Commit Ok    |
| [Down]      | [Ctrl+n] | Next record         |
| [F6]        | [Ctrl+o] | Insert record       |
| [Up]        | [Ctrl+p] | Previous record     |
| [PageUp]    | [Ctrl+r] | Previous record set |
| [F4]        | [Ctrl+t] | Copy record         |
| [F5]        | [Ctrl+u] | List of values      |
| [F3]        | [Ctrl+v] | Paste / Copy field  |
| [PageDn]    | [Ctrl+w] | Next record set     |
| [F10]       | [Ctrl+x] | Query               |
| [F9]        | [Ctrl+y] | Rollback Cancel     |
| [F8]        | [Ctrl+z] | Save and exit       |
