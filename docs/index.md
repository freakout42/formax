**formax** is a tool for developing and executing fast,
powerful and user-friedly character-based interactive
database applications. Do not confuse these applications
with data-management tools like Toad, phpMyAdmin or
SQLServer Management Studio. These are for database-admins.
**formax** builds and runs end-user applications, which can
be used without any SQL-knowledge and ensure only valid data
operations. The user is typically an enterprise employee in
the office or home-office. He is trained to use these forms
with the full potential with unmatched efficiency. With the
fast C++ keyboard optimized, no mouse application and the
minimal bandwidth need of a terminal massive savings in
working time are possible.

The basic concepts of Oracle SQL-Forms 3.0 are combined with
curses as the terminal interface, ODBC for universal
database access and the best ideas for a powerful user
interfaces from Emacs, Clipper, SAP and Lotus/1-2-3. The
result is the most effective solution possible.

In every system of decent size is the need for something
like **formax**. The system-adminstrator needs to change a
base-table or master-data. Most of the times he is forced to
use error-prone and dangerous SQL or a database-admin-tool
like phpMyAdmin because there was no web-application build
for all these small things. Or maintaining customer
information. Beside the new address-data itself there are
only around 20 keystrokes required to edit the record - no
mouse-moving - no scrolling. Frequently used apps like
order-entry for which the initial training to use a keyboard
application pays off thousand times are also candidates for
a fast terminal-application.

### History/Motivation

Oracle SQL-Forms 3 was superior to all other character-based
applications that came after it. This very popular option
was lost in the late 1990s when Oracle decided not to patch
SQL-Forms 3 for the Y2K-problem. The following versions
neglected this feature and focused on client-server
frontends, which was also abandoned later in favour of web
based as the only option. The result was a massive increase
of complexity and bandwidth requirements in magnitudes of
millions and leaving a big technology hole in the industry.

### Goals

**formax** wants to provide a user-friendly, fast method for
entering valid data into the database. The trained user can
enter data with minimal effort and time. The form
application can be build without code by setting up some
data in a couple of tables with minimal programming. A
minimal form to maintain a single table with complete CRUD
functionality can be build with a single line of code within
two seconds. Secure access to the form is easily managed by
setting up a PuTTY-session on any windows machine with
zero-administration on the client. This works from anywhere
without any bandwidth-requirements.

Only the concepts is inherited from SQL-Forms 3. There is no
compatibility or migration utility for Oracle Forms sources.

### license

Simplified BSD License  
Copyright (c) 2024, Axel K. Reinhold  
All rights reserved.  
