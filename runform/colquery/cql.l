%option nounput
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
g       [<>]
u       #
y       \[.*\]
w       [ \t]+
day     ([1-9])
lday    ((0[1-9])|([1-2][0-9])|(3[0-1]))
mon     ([1-9])
lmon    ((0[1-9])|(1[0-2]))
ye19    ([7-9][0-9])
ye20    ([0-6][0-9])
year    ((19|20)[0-9][0-9])
ddmys   {lday}{lmon}{ye19}
ddmys2  {lday}{lmon}{ye20}
ddmysl  {lday}{lmon}{year}
ddm9    {day}\.{mon}\.{ye19}
dem9    {lday}\.{mon}\.{ye19}
ddn9    {day}\.{lmon}\.{ye19}
den9    {lday}\.{lmon}\.{ye19}
ddm2    {day}\.{mon}\.{ye20}
dem2    {lday}\.{mon}\.{ye20}
ddn2    {day}\.{lmon}\.{ye20}
den2    {lday}\.{lmon}\.{ye20}
ddmy    {day}\.{mon}\.{year}
demy    {lday}\.{mon}\.{year}
ddny    {day}\.{lmon}\.{year}
deny    {lday}\.{lmon}\.{year}
een9    {lmon}\/{lday}\/{ye19}
een2    {lmon}\/{lday}\/{ye20}
eeny    {lmon}\/{lday}\/{year}
my19    {mon}\.{ye19}
ly19    {lmon}\.{ye19}
my20    {mon}\.{ye20}
ly20    {lmon}\.{ye20}
ml      {mon}\.{year}
ll      {lmon}\.{year}
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
         sprintf (tmp, "19%c%c%c%c%c%c", *(yytext+4), *(yytext+5), *(yytext+2), *(yytext+3), *(yytext+0), *(yytext+1));
         cqlval.item = tmp;
         return(DATE);
        }
<DATET>{ddmys2} {
         sprintf (tmp, "20%c%c%c%c%c%c", *(yytext+4), *(yytext+5), *(yytext+2), *(yytext+3), *(yytext+0), *(yytext+1));
         cqlval.item = tmp;
         return(DATE);
        }
<DATET>{ddmysl} {
         sprintf (tmp, "%c%c%c%c%c%c%c%c", *(yytext+4), *(yytext+5), *(yytext+6), *(yytext+7), *(yytext+2), *(yytext+3), *(yytext+0), *(yytext+1));
         cqlval.item = tmp;
         return(DATE);
        }
{ddm9}  {
         sprintf (tmp, "19%c%c0%c0%c", *(yytext+4), *(yytext+5), *(yytext+2), *(yytext+0));
         cqlval.item = tmp;
         return(DATE);
        }
{dem9}  {
         sprintf (tmp, "19%c%c0%c%c%c", *(yytext+5), *(yytext+6), *(yytext+3), *(yytext+0), *(yytext+1));
         cqlval.item = tmp;
         return(DATE);
        }
{ddn9}  {
         sprintf (tmp, "19%c%c%c%c0%c", *(yytext+5), *(yytext+6), *(yytext+2), *(yytext+3), *(yytext+0));
         cqlval.item = tmp;
         return(DATE);
        }
{den9}  {
         sprintf (tmp, "19%c%c%c%c%c%c", *(yytext+6), *(yytext+7), *(yytext+3), *(yytext+4), *(yytext+0), *(yytext+1));
         cqlval.item = tmp;
         return(DATE);
        }
{ddm2}  {
         sprintf (tmp, "20%c%c0%c0%c", *(yytext+4), *(yytext+5), *(yytext+2), *(yytext+0));
         cqlval.item = tmp;
         return(DATE);
        }
{dem2}  {
         sprintf (tmp, "20%c%c0%c%c%c", *(yytext+5), *(yytext+6), *(yytext+3), *(yytext+0), *(yytext+1));
         cqlval.item = tmp;
         return(DATE);
        }
{ddn2}  {
         sprintf (tmp, "20%c%c%c%c0%c", *(yytext+5), *(yytext+6), *(yytext+2), *(yytext+3), *(yytext+0));
         cqlval.item = tmp;
         return(DATE);
        }
{den2}  {
         sprintf (tmp, "20%c%c%c%c%c%c", *(yytext+6), *(yytext+7), *(yytext+3), *(yytext+4), *(yytext+0), *(yytext+1));
         cqlval.item = tmp;
         return(DATE);
        }
{ddmy}  {
         sprintf (tmp, "%c%c%c0%c%c0%c", *(yytext+4), *(yytext+5), *(yytext+6), *(yytext+7), *(yytext+2), *(yytext+0));
         cqlval.item = tmp;
         return(DATE);
        }
{demy}  {
         sprintf (tmp, "%c%c%c%c0%c%c%c", *(yytext+5), *(yytext+6), *(yytext+7), *(yytext+8), *(yytext+3), *(yytext+0), *(yytext+1));
         cqlval.item = tmp;
         return(DATE);
        }
{ddny}  {
         sprintf (tmp, "%c%c%c%c%c%c0%c", *(yytext+5), *(yytext+6), *(yytext+7), *(yytext+8), *(yytext+2), *(yytext+3), *(yytext+0));
         cqlval.item = tmp;
         return(DATE);
        }
{deny}  {
         sprintf (tmp, "%c%c%c%c%c%c%c%c", *(yytext+6), *(yytext+7), *(yytext+8), *(yytext+9), *(yytext+3), *(yytext+4), *(yytext+0), *(yytext+1));
         cqlval.item = tmp;
         return(DATE);
        }
{een9}  {
         sprintf (tmp, "19%c%c%c%c%c%c", *(yytext+6), *(yytext+7), *(yytext+0), *(yytext+1), *(yytext+3), *(yytext+4));
         cqlval.item = tmp;
         return(DATE);
        }
{een2}  {
         sprintf (tmp, "20%c%c%c%c%c%c", *(yytext+6), *(yytext+7), *(yytext+0), *(yytext+1), *(yytext+3), *(yytext+4));
         cqlval.item = tmp;
         return(DATE);
        }
{eeny}  {
         sprintf (tmp, "%c%c%c%c%c%c%c%c", *(yytext+6), *(yytext+7), *(yytext+8), *(yytext+9), *(yytext+0), *(yytext+1), *(yytext+3), *(yytext+4));
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
{s}     {sprintf (tmp, "'%%%s%%'", yytext+1);
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
