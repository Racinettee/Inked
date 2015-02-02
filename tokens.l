%{
#include <string>
#include "node.h"
#include "parser.parser.hh"
#define SAVE_TOKEN yylval.string = new std::string(yytext, yyleng)
#define TOKEN(t) (yylval.token = t)
extern "C" int yywrap() { puts("yywraped"); return 1; }
extern unsigned long long linecount;
%}

%%

as                      return TOKEN(TASKW);
break                   return TOKEN(TBREAKKW);
const                   return TOKEN(TCONSTKW);
class                   return TOKEN(TCLSKW);
continue                return TOKEN(TCONTKW);
do                      return TOKEN(TDOKW);
else                    return TOKEN(TELSEKW);
end                     return TOKEN(TENDKW);
exit                    return TOKEN(TEXITKW);
for                     return TOKEN(TFORKW);
function                return TOKEN(TFCTNKW);
if                      return TOKEN(TIFKW);
proto							return TOKEN(TPROTOKW);
unless                  return TOKEN(TUNLKW);
while                   return TOKEN(TWHLKW);

\"(\\.|[^"])*\"         {SAVE_TOKEN;return  TSLITERAL;}

[ \t]                   ;
\n                      ++linecount;
[a-zA-Z_][a-zA-Z0-9_]*  SAVE_TOKEN; return TIDENTIFIER;
[0-9]+\.[0-9]*          SAVE_TOKEN; return TDOUBLE;
[0-9]+                  SAVE_TOKEN; return TINTEGER;
"="                     return TOKEN(TEQUAL);
"=="                    return TOKEN(TCEQ);
"!="                    return TOKEN(TCNE);
"<"                     return TOKEN(TCLT);
"<="                    return TOKEN(TCLE);
">"                     return TOKEN(TCGT);
">="                    return TOKEN(TCGE);
"("                     return TOKEN(TLPAREN);
")"                     return TOKEN(TRPAREN);
"{"                     return TOKEN(TLBRACE);
"}"                     return TOKEN(TRBRACE);
"."                     return TOKEN(TDOT);
","                     return TOKEN(TCOMMA);
"+"                     return TOKEN(TPLUS);
"-"                     return TOKEN(TMINUS);
"*"                     return TOKEN(TMUL);
"/"                     return TOKEN(TDIV);
"\""                    return TOKEN(TQUOTE);
.                       printf("Unknown token!\n"); yyterminate();
<<EOF>>                 yyterminate();
%%
