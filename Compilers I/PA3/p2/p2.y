%{
#include<stdio.h>
#include <string.h>
extern int yylex();
extern int yyparse();
void yyerror(const char* s){ 
	printf("%s\n",s);
}
int yywrap(){return 1;}

const char *initial = "#include<stdio.h>\n#include<stdlib.h>\n#include<seven_segment.h>\nint main()\n{\n";
void tab(int t){
	for(int i=0;i<t;i++)
		printf("\t");
}
int ntab = 0;
bool init = false;
int main()
{
	printf("%s",initial);
	ntab++;
    yyparse();

    if(init){
	    ntab--;
	    tab(ntab);
	    printf("}\n");
    }

    ntab--;
    tab(ntab);
    printf("}\n");
}
%}

%token INIT SELECT DELAY IF ELSE INT READINT CMPOP COLON INTEGER CONF LINE WHITESPACE IDENTIFIER ASSIGN
%union { int a; char* ch; char conf[9];}
%type<a> INTEGER
%type<ch> IDENTIFIER CMPOP
%type<conf> CONF 
%start program

%%
program : stmt LINE program | stmt LINE | stmt ;
stmt : delay_stmt | select_stmt | declaration_stmt | inp_stmt | conf_stmt | init_stmt | if_stmt ;
delay_stmt : DELAY COLON WHITESPACE INTEGER {tab(ntab);printf("delay(%d);\n",$4);};
select_stmt :SELECT COLON WHITESPACE INTEGER {tab(ntab);printf("select(%d);\n",$4);};
declaration_stmt : IDENTIFIER {tab(ntab);printf("int %.*s;\n",int(strlen($1))-1,$1);} COLON WHITESPACE INT ;
inp_stmt : IDENTIFIER {tab(ntab); printf("%s = ",$1);} WHITESPACE ASSIGN WHITESPACE a3;
conf_stmt : CONF {tab(ntab); printf("write(strtol(\"%s\"));\n",$1);};
init_stmt : INIT {init=true; tab(ntab); printf("init();\n"); tab(ntab); printf("while(1)\n"); tab(ntab); printf("{\n"); ntab++;};
if_stmt : IF {tab(ntab); printf("if(");} 
		  WHITESPACE a1
		  WHITESPACE CMPOP {printf("%s",$6);} 
		  WHITESPACE a2
		  COLON LINE stmt LINE {ntab--; tab(ntab); printf("}\n");}
		  ELSE {tab(ntab); printf("else\n"); tab(ntab); printf("{\n"); ntab++;} 
		  COLON LINE stmt {ntab--; tab(ntab); printf("}\n");}
a1 : IDENTIFIER {printf("%s",$1);} | INTEGER {printf("%d",$1);} ;
a2 : IDENTIFIER {printf("%s)\n",$1); tab(ntab); printf("{\n"); ntab++;} | INTEGER {printf("%d)\n",$1); tab(ntab); printf("{\n"); ntab++;} ;
a3 : READINT {printf("readInt();\n");} | INTEGER {printf("%d;\n",$1);} | IDENTIFIER {printf("%s;\n",$1);};
%%
