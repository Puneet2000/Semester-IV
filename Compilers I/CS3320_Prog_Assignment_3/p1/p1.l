%{
  #include <stdio.h>
  #include "y.tab.h"
  int c;
%}

%%
[KQRBN]   { c = yytext[0];
            yylval.ch = c;
            return PIECE;}
[a-h]     { c = yytext[0];
            yylval.a = c - 'a';
            return COLUMN;}
[1-8]     { c = yytext[0];
            yylval.a = c - '1';
            return ROW;}
-          return MOVE;
"x"        return CAPTURE;
=          return PROMOTION;
\n		   return LINE;
" "		   return SPACE;
%%