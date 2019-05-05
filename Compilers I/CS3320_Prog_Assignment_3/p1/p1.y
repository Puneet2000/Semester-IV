%{
#include<stdio.h>
#include <string.h>
extern int yylex();
extern int yyparse();
void yyerror(const char* s){ 
	printf("%s\n",s);
}
int yywrap(){return 1;}
char board[8][8] = {{'R', 'N', 'B', 'Q', 'K', 'B', 'N', 'R'}
				 ,{'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P'}
				 ,{' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '}
				 ,{' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '}
				 ,{' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '}
				 ,{' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '}
				 ,{'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P'}
				 ,{'R', 'N', 'B', 'Q', 'K', 'B', 'N', 'R'}};

void make_turn(int type, int c1, int r1, int c2, int r2){
	if(type==0){
		char p = board[r1][c1];
		board[r1][c1] = ' '; board[r2][c2] = p;
	}
	else {
		board[r1][c1] = ' ';
		board[r2][c2] = type;
	}
}

void display(){
	printf("\ta\tb\tc\td\te\tf\tg\th\n");
	for(int i=0;i<8;i++){
		printf("%d",i+1);
		for(int j=0;j<8;j++)
			printf("\t%c",board[i][j]);
		printf("\n");
	}
}

int main()
{
    yyparse();
}
%}

%token PIECE COLUMN ROW MOVE CAPTURE PROMOTION LINE SPACE
%union { int a; char ch; }
%start program
%type<a> COLUMN ROW
%type<ch> PIECE 

%%
program : turn SPACE turn LINE program | turn SPACE turn {display();};
turn : move_piece | capture_piece | move_pawn | promote | capture_pawn;
move_piece : PIECE COLUMN ROW MOVE COLUMN ROW {make_turn(0,$2,$3,$5,$6);};
capture_piece : PIECE COLUMN ROW CAPTURE COLUMN ROW {make_turn(0,$2,$3,$5,$6);};
capture_pawn : COLUMN ROW CAPTURE COLUMN ROW {make_turn(0,$1,$2,$4,$5);};
move_pawn : COLUMN ROW MOVE COLUMN ROW {make_turn(0,$1,$2,$4,$5);};
promote : COLUMN ROW MOVE COLUMN ROW PROMOTION PIECE {make_turn($7,$1,$2,$4,$5);};
%%
