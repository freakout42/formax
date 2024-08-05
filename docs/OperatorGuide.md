Installation
============

Deployment
==========

Security
========
There are no calls to the OS (except malloc/free).
Everything is compiled with -Wall -Werror and memchecked
with valgrind. Only int and char types are used and where
possible static variables are used. Everything that comes
from outside is passed to ODBC with bind variables. There is
no free SQL enterable as is was possible with original
SQL-Forms. The password for the database is encrypted on the
command line. Every executed SQL-statement can be logged in
a central database.

