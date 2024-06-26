**formax** is a tool for developing and executing fast,
powerful and user-friedly character-based, interactive
database applications. Do not confuse these applications
with data-management tools like Toad, phpMyAdmin or
SQLServer Management Studio. These are for database-admins.
**formax** builds and runs end-user applications, which can
be used without any SQL-knowledge and ensure only valid data
operations. The user is typically an enterprise employee in
the office or home-office. He is trained to use these forms
with unmatched efficiency. The fast, keyboard-optimized,
no-mouse C++-application and the minimal bandwidth needs,
result in massive savings of working time.

The basic concepts of Oracle SQL-Forms 3.0 (1988) are
combined with curses as the terminal interface, ODBC for
universal database access and Javascript as the trigger
language. The best ideas for a powerful user interface came
from Emacs, Clipper, SAP and Lotus/1-2-3. The result is the
most effective solution possible. By eleminating all
proprietary dependencies und being open-source including all
dependencies with a permissive license the system is
future-proof. The minimal costs to build a **formax**
application makes adapting this technology a no-brainer for
any enterprise.

In every system of decent size is the need for something
like **formax**. The system-adminstrator needs to change a
base-table or master-data. Most of the times he is forced to
use error-prone and dangerous SQL or a database-admin-tool
like phpMyAdmin because there was no web-application build
for all these small things. Or for maintaining customer
information. Beside the new address-data itself there are
only around 20 keystrokes required to edit the record - no
mouse-moving - no scrolling - with milliseconds response
time. Frequently used apps like order-entry for which the
initial training to use a powerful keyboard application pays
off thousand times are even natch candidates for it.

### History/Motivation

Oracle SQL-Forms 3 was superior to all other character-based
enterprise applications that came after it. The terminal
option was very popular and got lost in the late 1990s when
Oracle decided not to patch SQL-Forms 3 for the Y2K-problem.
The following versions neglected this feature and focused on
client-server frontends, which were also abandoned later in
favour of web based as the only option. The result was a
massive increase of complexity and bandwidth requirements in
magnitudes of millions and leaving a big technology hole in
the industry. **formax** wants to bring back this option to
the market for all popular databases and all kinds of
terminals and emulations.

### Goals

**formax** wants to provide a user-friendly, fast method for
entering valid data into the database. This does not mean a
good looking gui. Users will hate the application with the
first look. After using it for a while they usally will love
it due to the time savings they can achieve. The trained
user can enter data with minimal effort and time, due to the
fast application, optimized keyboard handling and ultra low
bandwidth usage. The productivity these forms bring to the
table cannot be achieved with other tools.

The form application can be build without code by setting up
some data in a couple of tables with minimal programming. A
minimal form to maintain a single table with complete CRUD
functionality can be build with a single line of code within
two seconds. Secure access to the form is easily managed by
setting up a PuTTY-session on any windows machine with
zero-administration on the client. This works from anywhere
without any bandwidth-requirements.

Only the concept is inherited from SQL-Forms 3. There is no
compatibility or migration utility for Oracle Forms sources.

### Status

Current version 0.9.0 can do nothing. We expect 1.0.0 be
released 08/2024 with the ability to Insert, Query, Update
and Delete (CRUD) in one table with single-row mode. Full
Forms3 functionality is a big task and depends highly on the
community participation.

### License

Simplified BSD License  
Copyright (c) 2024, Axel K. Reinhold  
All rights reserved.  
