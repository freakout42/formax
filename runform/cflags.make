# MacOsX: brew install ncurses unixodbc sqliteodbc
CCVER=$(shell $(CC) -dumpversion | sed 's/\..*//')
ifeq "$(CCVER)" "16"
  DISABLEWARN=-Wno-deprecated-non-prototype -Wno-invalid-source-encoding
endif
LINTING=-Wall -Werror -Wno-write-strings $(DISABLEWARN)
ifeq (test,$(MAKECMDGOALS))
  SUBTARGET=test
  CFLAGS=-g -O0 $(LINTING)
else
  SUBTARGET=all
  CFLAGS=-O2 -DNDEBUG $(LINTING)
endif
CXXFLAGS=$(CFLAGS)