%{
#include<stdio.h>
#include <string.h>
extern int yylex();
extern int yyparse();
void yyerror(const char* s){ 
	printf("%s\n",s);
}
int yywrap(){return 1;}
void display_conf(char c[]){
	printf("%s\n",c);
}
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
stmt : delay_stmt | select_stmt | declaration_stmt | inp_stmt | conf_stmt | init_stmt | if_stmt;
delay_stmt : DELAY COLON WHITESPACE INTEGER {tab(ntab);printf("delay(%d);\n",$4);};
select_stmt :SELECT COLON WHITESPACE INTEGER {tab(ntab);printf("select(%d);\n",$4);};
declaration_stmt : IDENTIFIER {tab(ntab);printf("int %.*s;\n",int(strlen($1))-1,$1);} COLON WHITESPACE INT ;
inp_stmt : IDENTIFIER {tab(ntab); printf("%s = readInt();\n",$1);} WHITESPACE ASSIGN WHITESPACE READINT;
conf_stmt : CONF {tab(ntab); printf("write(strtol(\"%s\"));\n",$1);};
init_stmt : INIT {init=true; tab(ntab); printf("init();\n"); tab(ntab); printf("while(1)\n"); tab(ntab); printf("{\n"); ntab++;};
if_stmt : IF {tab(ntab); printf("if(");} 
		  WHITESPACE IDENTIFIER {printf("%s",$4);} 
		  WHITESPACE CMPOP {printf("%s",$7);} 
		  WHITESPACE INTEGER {printf("%d)\n",$10); tab(ntab); printf("{\n"); ntab++;} 
		  COLON LINE stmt LINE {ntab--; tab(ntab); printf("}\n");}
		  ELSE {tab(ntab); printf("else\n"); tab(ntab); printf("{\n"); ntab++;} 
		  COLON LINE stmt {ntab--; tab(ntab); printf("}\n");}
%%
