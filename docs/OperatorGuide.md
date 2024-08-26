Installation

Security
Reliability
Cost
Operational excellence
Performance efficience
Sustainability

============

Install
=======
System preparation is needed to install the dependencies:

On codespace/debian/Ubuntu do the following:
~~~
sudo apt-get -y update
sudo apt-get -y install build-essential
sudo apt-get -y install python
sudo apt-get -y install unixodbc
sudo apt-get -y install libsqliteodbc
sudo apt-get -y install libncurses5-dev
sudo apt-get -y install flex
~~~

On RHEL/CentOS/Oracle/Alma/Rocky do the following:
~~~
sudo yum -y update
sudo yum -y group install "Development Tools"
sudo yum -y install python3
sudo yum -y install unixODBC
sudo yum -y install unixODBC-devel
sudo yum -y install ncurses-devel
sudo yum -y install sqlite
rpm -i https://github.com/freakout42/formax/releases/download/v0.9.9/sqliteodbc-0.9998-1.x86_64.rpm
~~~

Build
=====

Fork in github and create codespace or download and extract
https://github.com/freakout42/formax/archive/refs/heads/main.zip  
Then do the following to build and install into `/opt/arx/`.
Nothing outside `/opt/arx/` will be touched in any way by
the installation with the exception of /etc/profile.d/arx.sh
which only adds `/opt/arx/bin` to the PATH.

~~~
./configure
make
sudo make install
. /etc/profile.d/arx.sh
~~~

Deployment
==========

Copy /opt/arx to your production/development/test machines.
Also /etc/profile.d/arx.sh should be installed for
convienience. THIS IS NOT COMPLETE - MORE DOCS ARE NEEDED. 

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

