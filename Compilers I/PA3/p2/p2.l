%{
#include "y.tab.h"  // to get the token types that we return
%}

%%
(?i:init)			return INIT;
(?i:select)			return SELECT;
(?i:if) 			return IF;
(?i:else)			return ELSE;
(?i:delay)			return DELAY;
(?i:int) 			return INT;
(?i:readint)		return READINT;
=					return ASSIGN;
(==|!=|>=|<=|>|<)	{ yylval.ch = yytext;
					 return CMPOP;}
:					return COLON;
[0-9]+				{ yylval.a = atoi(yytext);
				 	return INTEGER;}
#(.)*(\n)+				;
(\t[0-1]){8}		{for(int i=1;i<16;i=i+2)
						yylval.conf[i/2]=yytext[i];
				 	 yylval.conf[8]='\0';
				 	 return CONF;}
(\n)+				return LINE;
" "					return WHITESPACE;
[_a-zA-Z][_a-zA-Z0-9]{0,30}	{yylval.ch = yytext;
				 	 	 return IDENTIFIER;}
%%