p1:
	lex p1.l
	yacc -d p1.y
	g++ -o p1.out lex.yy.c y.tab.c "/usr/lib/x86_64-linux-gnu/libfl.a"
	
clean:
	rm *.yy.c ||:
	rm *.tab.* ||:
	rm *.out ||: