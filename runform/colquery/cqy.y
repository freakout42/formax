%union  {
        int oper;
        char *item;
        }
%type   <item>  list const constm constk constq constd constt
                svalue value comp pexpr expr query match qstr
%token  <oper>  OR
%token  <oper>  AND
%token  <oper>  NOT
%token  <oper>  BETWEEN
%token  <oper>  LPAREN
%token  <oper>  RPAREN
%token  <oper>  STAR
%token  <item>  SQLNULL
%token  <item>  NUMBER
%token  <item>  MDATE
%token  <item>  KWDATE
%token  <item>  QDATE
%token  <item>  DATE
%token  <item>  TIME
%token  <item>  STRING
%token  <item>  PATTERN
%token  <item>  SOUND
%token  <item>  QUERY
%token  <item>  COMPARE
%token  <item>  ALL
%token          ERROR
%left           STAR
%left           LOR
%left           OR
%left           AND
%right          NOT
%nonassoc       BETWEEN
%{
#include <stdio.h>
#include <string.h>
#include "colquery.h"
extern char     *cqop;
extern char     *cqcolumn;
extern int      cqcoltype;
static char     *cqstr();
extern char     *cqbuf;
extern int      cqbufree;
char            tmp[1024];
#ifdef YYDEBUG
int cqdebug = 0;
#endif

static void cqtype(typ)
        int typ;
{
if (cqcoltype == 0 || cqcoltype == typ) cqcoltype = typ;
else cqcoltype = OR;
}
%}
%%
query   : expr
        {
        strcpy (cqop, $1);
        }
        | ALL
        {
        strcpy (cqop, "1=1");
        }
        |
        {
        strcpy (cqop, "1=1");
        }
        ;
expr    : pexpr
        | expr pexpr %prec LOR
        {
        sprintf (tmp, "(%s) OR (%s)", $1, $2);
        $$ = cqstr (tmp);
        }
        ;
pexpr   : value
        | pexpr AND value
        {
        sprintf (tmp, "(%s) AND (%s)", $1, $3);
        $$ = cqstr (tmp);
        }
        ;
value   : QUERY
        {$$ = cqstr ($1);}
        | NOT SQLNULL
        {
        sprintf (tmp, "NOT %s IS NULL", cqcolumn);
        $$ = cqstr (tmp);
        }
        | SQLNULL
        {
        sprintf (tmp, "%s IS NULL", cqcolumn);
        $$ = cqstr (tmp);
        }
        | svalue
        | NOT svalue
        {
        sprintf (tmp, "NOT (%s)", $2);
        $$ = cqstr (tmp);
        }
svalue  : constm
        {
        sprintf (tmp, "TO_CHAR(%s,'YYYYMM') = %s", cqcolumn, $1);
        $$ = cqstr (tmp);
        }
        | comp constm
        {
        sprintf (tmp, "TO_CHAR(%s,'YYYYMM') %s %s", cqcolumn, $1, $2);
        $$ = cqstr (tmp);
        }
        | constm BETWEEN constm
        {
        sprintf (tmp, "TO_CHAR(%s,'YYYYMM') BETWEEN %s AND %s",
                cqcolumn, $1, $3);
        $$ = cqstr (tmp);
        }
        | constk
        {
        sprintf (tmp, "TO_CHAR(%s,'YYYYIW') = %s", cqcolumn, $1);
        $$ = cqstr (tmp);
        }
        | comp constk
        {
        sprintf (tmp, "TO_CHAR(%s,'YYYYIW') %s %s", cqcolumn, $1, $2);
        $$ = cqstr (tmp);
        }
        | constk BETWEEN constk
        {
        sprintf (tmp, "TO_CHAR(%s,'YYYYIW') BETWEEN %s AND %s",
                cqcolumn, $1, $3);
        $$ = cqstr (tmp);
        }
        | constq
        {
        sprintf (tmp, "TO_CHAR(%s,'YYYYQ') = %s", cqcolumn, $1);
        $$ = cqstr (tmp);
        }
        | comp constq
        {
        sprintf (tmp, "TO_CHAR(%s,'YYYYQ') %s %s", cqcolumn, $1, $2);
        $$ = cqstr (tmp);
        }
        | constq BETWEEN constq
        {
        sprintf (tmp, "TO_CHAR(%s,'YYYYQ') BETWEEN %s AND %s",
                cqcolumn, $1, $3);
        $$ = cqstr (tmp);
        }
        | constd
        {
        sprintf (tmp, "%s BETWEEN %s AND %s + .99999", cqcolumn, $1, $1);
        $$ = cqstr (tmp);
        }
        | comp constd
        {
        sprintf (tmp, "%s %s %s", cqcolumn, $1, $2);
        $$ = cqstr (tmp);
        }
        | constd BETWEEN constd
        {
        sprintf (tmp, "%s BETWEEN %s AND %s + .99999", cqcolumn, $1, $3);
        $$ = cqstr (tmp);
        }
        | constt
        {
        sprintf (tmp, "TO_CHAR(%s,'YYYYMMDDHH24MI') = %s", cqcolumn, $1);
        $$ = cqstr (tmp);
        }
        | comp constt
        {
        sprintf (tmp, "TO_CHAR(%s,'YYYYMMDDHH24MI') %s %s",cqcolumn,$1,$2);
        $$ = cqstr (tmp);
        }
        | constt BETWEEN constt
        {
        sprintf (tmp, "TO_CHAR(%s,'YYYYMMDDHH24MI') BETWEEN %s AND %s",
                cqcolumn, $1, $3);
        $$ = cqstr (tmp);
        }
        | comp const
        {
        sprintf (tmp, "%s %s %s", cqcolumn, $1, $2);
        $$ = cqstr (tmp);
        }
        | const %prec LOR
        {
        sprintf (tmp, "%s = %s", cqcolumn, $1);
        $$ = cqstr (tmp);
        }
        | list %prec LOR
        {
        sprintf (tmp, "%s IN (%s)", cqcolumn, $1);
        $$ = cqstr (tmp);
        }
        | match
        {
        sprintf (tmp, "{fn UCASE(%s)} LIKE {fn UCASE(%s)}", cqcolumn, $1);
        $$ = cqstr (tmp);
        cqtype(STRING);
        }
        | PATTERN
        {
        sprintf (tmp, "%s LIKE %s", cqcolumn, $1);
        $$ = cqstr (tmp);
        cqtype(STRING);
        }
        | SOUND
        {
        sprintf (tmp, "{fn SOUNDEX(%s)} = {fn SOUNDEX(%s)}", cqcolumn, $1);
        $$ = cqstr (tmp);
        cqtype(STRING);
        }
        | const BETWEEN const
        {
        sprintf (tmp, "%s BETWEEN %s AND %s", cqcolumn, $1, $3);
        $$ = cqstr (tmp);
        }
        | LPAREN expr RPAREN
        {$$ = $2;}
        | LPAREN RPAREN
        {
        $$ = cqstr ("1=1");
        }
        ;
match   : STAR STRING
        {
        sprintf (tmp, "'%%%s%%'", $2);
        $$ = cqstr (tmp);
        }
        ;
list    : const OR const
        {
        sprintf (tmp, "%s,%s", $1, $3);
        $$ = cqstr (tmp);
        }
        | const OR list
        {
        sprintf (tmp, "%s,%s", $1, $3);
        $$ = cqstr (tmp);
        }
        ;
comp    : COMPARE
        {
        $$ = cqstr ($1);
        }
        ;
constm  : MDATE
        {
        $$ = cqstr ($1);
        cqtype(DATE);
        }
        ;
constk  : KWDATE
        {
        $$ = cqstr ($1);
        cqtype(DATE);
        }
        ;
constq  : QDATE
        {
        $$ = cqstr ($1);
        cqtype(DATE);
        }
        ;
constd  : DATE
        {
        $$ = cqstr ($1);
        cqtype(DATE);
        }
        ;
constt  : TIME
        {
        $$ = cqstr ($1);
        cqtype(DATE);
        }
        ;
qstr    : STRING
        {
        sprintf (tmp, "'%s'", $1);
        $$ = cqstr (tmp);
        }
        | BETWEEN
        {
        sprintf (tmp, "'-'");
        $$ = cqstr (tmp);
        }
        ;
const   : NUMBER
        {
        $$ = cqstr ($1);
        cqtype(NUMBER);
        }
        | qstr
        {
        $$ = cqstr ($1);
        cqtype(STRING);
        }
        ;
%%
static char *cqstr (s)
        char *s;
{
char    *d;
int     l;

d       = cqbuf;
l       = strlen(s);
cqbufree -= l;
strncpy (d, s, cqbufree);
cqbuf   += l + 1;
return d;
}
