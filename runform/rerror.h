/* error messages */
#define MSG_ALLOC    24
#define MSG_OPENKEY  26
#define MSG_SQL      50
#define MSG_HELP     51
#define MSG_JS       52
#define MSG_FIRST    100
#define MSG_EDITKEY  101
#define MSG_RECDIRTY 102
#define MSG_LAST     103
#define MSG_NOREC    104
#define MSG_DELASK   105
#define MSG_QUERYM   106
#define MSG_NOPRIKEY 107
#define MSG_FLDPROT  200
#define MSG_NOMATCH  201
#define MSG_NORANGE  207
#define MSG_RDONLY   208
#define MSG_FLDFORM  209
#define MSG_NOREC2   210
#define MSG_COUNT0   301
#define MSG_CLEAN    401
#define MSG_DIRTY    402
#define MSG_NOTYET   403
#ifndef BEGINSQL
/* error messages */
class rError: public Record {
public:
  int init();
private:
};
#else
CREATE TABLE errors
  (id        INTEGER PRIMARY KEY NOT NULL,
   num       INTEGER NOT NULL DEFAULT 1,
   severity  INTEGER NOT NULL DEFAULT 0,    /* 0:info 1:warn 2:error 3:fatal */
   etext     TEXT    NOT NULL DEFAULT ''
  );
#ifdef FORMAXLANG_DE
#include "rerror.h.de"
#elif FORMAXLANG_FR
#include "rerror.h.fr"
#else
#include "rerror.h.en"
#endif
#endif
