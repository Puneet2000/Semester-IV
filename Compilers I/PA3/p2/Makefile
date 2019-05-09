p2:
	lex p2.l
	yacc -d p2.y
	g++ -o p2.out lex.yy.c y.tab.c -lm -ll
	
clean:
	rm *.yy.c ||:
	rm *.tab.* ||:
	rm *.out ||: