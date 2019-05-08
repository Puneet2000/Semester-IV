p2:
	lex p2.l
	yacc -d p2.y
	g++ -o p2.out lex.yy.c y.tab.c "/usr/lib/x86_64-linux-gnu/libfl.a"
	
clean:
	rm *.yy.c ||:
	rm *.tab.* ||:
	rm *.out ||: