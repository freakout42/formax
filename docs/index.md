**formax** is a tool for developing and executing fast,
powerful and user-friendly character-based, interactive
database applications. Do not confuse these applications
with data-management tools like Toad, phpMyAdmin or
SQLServer Management Studio. These are for database-admins.
**formax** builds and runs end-user applications, which can
be used without any SQL knowledge and ensure only valid data
operations. The user is typically an enterprise employee in
the office or homeoffice. He is trained to use these forms
with unmatched efficiency. The fast, keyboard-optimized,
nomouse C++-application and the minimal bandwidth needs,
result in massive savings of working time for data
maintenance.
~~~
Don't look at formax as a romantic nostalgica!
It is provided as serious alternative for specific
use-cases for which it can have massive advantages
~~~
The basic concepts of Oracle SQL-Forms 3.0 (1988) are
combined with modern standards and current database best
practices and technologies. Using ODBC for universal database
access and JavaScript as the trigger language opens this
concept universally. The best ideas for a powerful user
interface came from Emacs, Clipper, SAP, Lightroom and
Lotus/1-2-3. The result is the most effective solution
possible for the end-user. By eleminating all proprietary
dependencies und being opensource with a permissive license
the system is totally futureproof. The minimal costs to
build a **formax** application makes adapting this
technology a no-brainer for any enterprise.

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
the industry. **formax** wants to bring back this lean
option to the market. And not only for Oracle-PL/SQL. With
ODBC/JavaScript being open for everybody.

### Goals

#### End-User Application

**formax** wants to provide a user-friendly, fast method for
entering valid data into the database. This does not mean a
good looking gui. Users will hate the application with the
first look. After using it for a while they will start
loving it due to the time savings they can achieve. The
trained user can enter data with minimal effort and time,
due to the fast application, optimized keyboard handling and
ultra low bandwidth usage. The productivity these forms
bring to the table cannot be achieved with any other tool
out there.

#### Development

The form application can be build without code by setting up
some data in a couple of tables with minimal programming. A
minimal form to maintain a single table with complete CRUD
functionality can be build with a single line of code within
two seconds. Anyways the primary goal is the efficiency of
the end-user application. Developer comfort is on second
place.

Only the concept is inherited from SQL-Forms 3. There is no
compatibility or migration utility for Oracle Forms sources.
The source code is completely a new development of 2024.
The database access layer OCI is replaced by ODBC, which
opens the system to all kinds of databases. The terminal
access layer Oracle-Terminal is replaced with ncurses which
runs on any kind of system and terminal. PL/SQL as the
trigger language is replaced by JavaScript.

#### Operations / Deployment / Security / Logging

Enterprise requirements are fundamental for this product.
Zero administration cost is ensured by the leightweight
concept itself. A single executable needs to be installed
anywhere on the PATH (Yes - One=1 file is needed). Secure
access to the form is easily managed by setting up a
PuTTY-session on any windows machine with zero
administration on the client. This works securely from
anywhere without any bandwidth requirements. With a simple
encrypted key 2FA is for free. Even 3-factor-authentication
is easy and comfortable by adding a TOTP login. Every user
action (SQL) can be logged in a central database.

### Status

Current v1.2.0 is stable - the generator can create a
single-block single-page form. You are able to create,
retrieve, update and delete (CRUD) records in the table. No
triggers, no transactions, no help yet. The designer-tool is
able to edit the page markup and to integrate the field
positions and lengths into the form. There is also a form to
maintain the field properties. The rest must be done by
editing the .inp-file.

Full Forms3 functionality is a big task and depends highly
on the community participation in the future. The database
has already all the configuration data for full Forms3
functionality. Most of the later functionality of Oracle
Forms was needed for the client-server and web frontends and
did not extend the real application functionality.

### License

GNU General Public License _Version 3, 29 June 2007_  
Copyright (c) 2024, Axel K. Reinhold.  
All rights reserved.

