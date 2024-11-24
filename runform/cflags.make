# MacOsX: brew install ncurses unixodbc sqliteodbc
# Windows: MSYS2 ucrt ncurses
CCVER=$(shell $(CC) -dumpversion | sed 's/\..*//')
ifeq "$(CCVER)" "16"
  DISABLEWARN=-Wno-deprecated-non-prototype -Wno-invalid-source-encoding
endif
ifeq "$(CCVER)" "3"
ifeq (run,$(MAKECMDGOALS))
  CXX=g++4
endif
else
ifeq "$(CCVER)" "6"
  CC=gcc
else
  STACKPROTECTION=-fstack-protector
endif
endif
LINTING=-Wall -Werror -Wno-write-strings $(DISABLEWARN)
CURSESLIB=-lcurses
ifeq (test,$(MAKECMDGOALS))
  SUBTARGET=test
  CFLAGS=-g -O0 $(LINTING) -Incurses/include
  CURSESLIB=ncurses/lib/libncurses.a
else ifeq (runform0,$(MAKECMDGOALS))
  SUBTARGET=small
  CFLAGS=-Os -DPURUNFORM -DNDEBUG $(LINTING)
else ifeq (small,$(MAKECMDGOALS))
  CFLAGS=-Os -DNDEBUG $(LINTING)
else
  SUBTARGET=all
  CFLAGS=-O3 -DNDEBUG $(STACKPROTECTION) $(LINTING)
  CLDFLAGS=$(STACKPROTECTION)
endif
CXXFLAGS=$(CFLAGS)
