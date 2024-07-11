%a 8000
%k 5000
%o 10000
%p 3000
%START DATET
n       [0-9]+\.?[0-9]*
c       [A-Za-z0-9\.]+
o       \".*\"
p       [A-Za-z0-9\._%]+
s       ~[A-Za-z0-9\.]+
g       <>|[<>]=?|!?=
u       #
y       \[.*\]
w       [ \t]+
day     ([1-9])
lday    ((0[1-9])|([1-2][0-9])|(3[0-1]))
mon     ([1-9])
lmon    ((0[1-9])|(1[0-2]))
ye19    ([5-9][0-9])
ye20    ([0-4][0-9])
year    ([0-9][0-9])
lyear   ((19|20)[0-9][0-9])
ddmys   {lday}{lmon}{ye19}
ddmys2  {lday}{lmon}{ye20}
ddmysl  {lday}{lmon}{lyear}
ddmy    {day}\.{mon}\.{year}
dlmy    {lday}\.{mon}\.{year}
ddly    {day}\.{lmon}\.{year}
dlly    {lday}\.{lmon}\.{year}
ddml    {day}\.{mon}\.{lyear}
dlml    {lday}\.{mon}\.{lyear}
ddll    {day}\.{lmon}\.{lyear}
dlll    {lday}\.{lmon}\.{lyear}
my19    {mon}\.{ye19}
ly19    {lmon}\.{ye19}
my20    {mon}\.{ye20}
ly20    {lmon}\.{ye20}
ml      {mon}\.{lyear}
ll      {lmon}\.{lyear}
%{
#undef input
#undef unput
#ifdef FLEX_SCANNER
#define YY_INPUT(buf,result,max_size) \
        { \
        int c = cqinput(); \
        result = (c == '\n') ? YY_NULL : (buf[0] = c, 1); \
        }
#endif
#include <string.h>
#include "colquery.h"
#include "colq.h"
extern int cqnonumbers;
extern int cqtype;
static char tmp[1024];
/*static char *p;*/

/*
static char *rom2d (roman)
        unsigned char *roman;
{
static char dec[2];

if (*(roman+1)=='/')
        dec[0] = '1';
else if (*(roman+3)=='/')
        dec[0] = '3';
else if (*(roman+1)=='V'||*(roman+1)=='v')
        dec[0] = '4';
else    dec[0] = '2';
dec[1] = '\0';
return dec;
}
 */
%}
%%
        {if (cqtype==DATE) BEGIN DATET;}
","     {return(OR);}
"&"     {return(AND);}
"-"     {return(BETWEEN);}
"!"     {return(NOT);}
"("     {return(LPAREN);}
")"     {return(RPAREN);}
"*"     {return(STAR);}
"%"     {return(ALL);}
{u}     {return(SQLNULL);}
{g}     {
         cqlval.item = (char*) yytext;
         return(COMPARE);
        }
{my19}  {sprintf (tmp, "19%s0%.1s", yytext+2, yytext);
         cqlval.item = tmp;
         return(MDATE);}
{my20}  {sprintf (tmp, "20%.2s0%.1s", yytext+2, yytext);
         cqlval.item = tmp;
         return(MDATE);}
{ly19}  {sprintf (tmp, "19%s%.2s", yytext+3, yytext);
         cqlval.item = tmp;
         return(MDATE);}
{ly20}  {sprintf (tmp, "20%.2s%.2s", yytext+3, yytext);
         cqlval.item = tmp;
         return(MDATE);}
{ml}    {sprintf (tmp, "%s0%.1s", yytext+2, yytext);
         cqlval.item = tmp;
         return(MDATE);}
{ll}    {sprintf (tmp, "%s%.2s", yytext+3, yytext);
         cqlval.item = tmp;
         return(MDATE);}
<DATET>{ddmys}  {
         sprintf (tmp, "TO_DATE('%c%c.%c%c.19%c%c','DD.MM.RR')",
                                         *(yytext+0), *(yytext+1),
                                         *(yytext+2), *(yytext+3),
                                         *(yytext+4), *(yytext+5));
         cqlval.item = tmp;
         return(DATE);
        }
<DATET>{ddmys2} {
         sprintf (tmp, "TO_DATE('%c%c.%c%c.20%c%c','DD.MM.RR')",
                                         *(yytext+0), *(yytext+1),
                                         *(yytext+2), *(yytext+3),
                                         *(yytext+4), *(yytext+5));
         cqlval.item = tmp;
         return(DATE);
        }
<DATET>{ddmysl} {
         sprintf (tmp, "TO_DATE('%c%c.%c%c.%c%c%c%c','DD.MM.YYYY')",
                                         *(yytext+0), *(yytext+1),
                                         *(yytext+2), *(yytext+3),
                                         *(yytext+4), *(yytext+5),
                                         *(yytext+6), *(yytext+7));
         cqlval.item = tmp;
         return(DATE);
        }
{ddmy}  {
         sprintf (tmp, "TO_DATE('%s','DD.MM.RR')", yytext);
         cqlval.item = tmp;
         return(DATE);
        }
{ddly}  {
         sprintf (tmp, "TO_DATE('%s','DD.MM.RR')", yytext);
         cqlval.item = tmp;
         return(DATE);
        }
{dlmy}  {
         sprintf (tmp, "TO_DATE('%s','DD.MM.RR')", yytext);
         cqlval.item = tmp;
         return(DATE);
        }
{dlly}  {
         sprintf (tmp, "TO_DATE('%s','DD.MM.RR')", yytext);
         cqlval.item = tmp;
         return(DATE);
        }
{ddml}  {
         sprintf (tmp, "TO_DATE('%s','DD.MM.YYYY')", yytext);
         cqlval.item = tmp;
         return(DATE);
        }
{dlml}  {
         sprintf (tmp, "TO_DATE('%s','DD.MM.YYYY')", yytext);
         cqlval.item = tmp;
         return(DATE);
        }
{ddll}  {
         sprintf (tmp, "TO_DATE('%s','DD.MM.YYYY')", yytext);
         cqlval.item = tmp;
         return(DATE);
        }
{dlll}  {
         sprintf (tmp, "TO_DATE('%s','DD.MM.YYYY')", yytext);
         cqlval.item = tmp;
         return(DATE);
        }
{n}     {if (cqnonumbers)
          {sprintf (tmp, "'%s'", yytext); cqlval.item = tmp;}
         else
          {cqlval.item = (char*) yytext;}
         return(NUMBER);}
{o}     {*(yytext+yyleng-1) = '\0';
         sprintf (tmp, "%s", yytext+1);
         cqlval.item = tmp;
         return(STRING);
        }
{c}     {
         sprintf (tmp, "%s", yytext);
         cqlval.item = tmp;
         return(STRING);
        }
{y}     {*(yytext+yyleng-1) = '\0';
         cqlval.item = (char*) yytext+1;
         return(QUERY);}
{s}     {sprintf (tmp, "'%s'", yytext+1);
         cqlval.item = tmp;
         return(SOUND);}
{p}     {
         sprintf (tmp, "'%s'", yytext);
         cqlval.item = tmp;
         return(PATTERN);
        }
{w}     ;
"\n"    {return(EOF);}
.       {return(ERROR);}
%%
