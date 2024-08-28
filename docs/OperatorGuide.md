**formax** Operator Guide
=========================

Uncommon for an open source project **formax** was designed
for enterprise use from the ground up. Enterprise
requirements are fundamental for the application:

 - Security

    Since the user of a form needs a local account on the
    database client host and runs a local binary, special
    measures have been setup like 3FA. The credentials to
    access the database can be protected. The development
    guidelines have security in mind, the dependence of 3rd
    party libraries is minimized and there are virtually no
    calls to the operating system from the user binary.

 - Reliability

    Reduced complexity is the key for reliable systems. The
    configuration free "one binary" design together with
    only two interfaces (screen/ncurses + database/ODBC)
    guaranty a fail-savety system. And in case of disruption
    a fairly easy troubleshooting.

 - Cost

    Without license costs and with minimal requirements for
    the system - even no 3rd party with license costs are
    needed - obtaining is for free. Setup for development
    can be done within an hour. Developing forms is
		thousandfold less effort compared to modern web frontend
    frameworks. Same is true for the production deployment.
    User management is simple and even 3FA is a matter of
    scanning a qr-code with a smart-phone.

 - Operational excellence

    Central easy to manage logging for every session and
    every sql command is build into the runtime. Updates are
    a simple copy of the runtime binary (one file). New
    forms are also simple copies of the .frm files.

 - Performance efficience

    The runtime is pure C++ with mostly static variables.
    This means that the database calls are the only
    measurable runtimes. Bandwidth to the client is
    thousandfold less compared to web clients.

 - Sustainability

    Open source and all dependencies also open source with
    a permissive license makes the system totally future
    proof. The database independence of ODBC tops this
    nature further more. Ask old SQL-Forms guys about the
    importance of this topic.

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

Fork in github and create codespace or download and extract:

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
convienience. On the production host `/opt/arx/bin/runform`
should have mode 2751:

    -rwxr-s--x 1 axel devel 266152 Aug 10 16:05 runform

Forms
=====

The forms should be copied into a directory owned by a
developer which primary group matches the group of the
runform binary with mode 751 and the forms 640:

    drwxr-x--x 2 axel devel 6     Jul  4 16:07 lib
    -rw-r----- 1 axel devel 36864 Aug 10 16:05 lib/scotty.frm

Users
=====

Every user needs an account on the production machine. A
ssh-key must be generated and an .ssh/authorized created:

    command="/usr/local/bin/formax",no-X11-forwarding,no-agent-forwarding ssh-rsa AAAA...

In `/usr/local/bin/formax` having mode 755:
~~~
#!/bin/sh
/opt/formax/bin/runform -g/tmp/formax.log \
  /opt/formax/lib/scotty.frm dbuser:dbpass@dbdsn
exit
~~~

Database
========

The target database needs a configured ODBC driver
installed. **formax** is tested with PostgreSQL, MySQL,
Oracle, SQL-Server, SAP Advantage but should work with any
driver. sqlite3 driver is build-in and always available.
The last parameter of the command line is the ODBC-DSN of
the database with optional username:password prefix
separated by a `@`. sqlite3 can be used without DSN by
simply giving the full path of the database file.
 
Security
========

There are no calls to the OS (except malloc/free) bt runform.
Everything is compiled with -Wall -Werror and memchecked
with valgrind. Only int and char types are used and where
possible, static variables are used. Everything that comes
from outside is passed to ODBC with bind variables. There is
no free SQL enterable as is was possible with original
SQL-Forms.

Every executed SQL-statement can be logged in a central
database with the `-g` switch.

The database password can be supplied encrypted with the
`-p` switch. Produce the crypted token with the `-y` switch
as root by supplying the forms .frm file. The token works
only with the supplied form to prohibit usage with self
generated forms against the production database.

If 3FA is required, a totp token must be generated and
supplied with the `-t` switch. The qr code for the token can
be used with any authenticator. The smart-phone of the user
can supply the code to the runform program at startup. To
login the ssh-key file, the passphrase and the smartphone
forms an easy 3FA.
