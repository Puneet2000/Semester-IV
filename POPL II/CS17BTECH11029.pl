% ------------------------------------------------
% An arithmetic Puzzle solution.
% author@ Puneet Mangla (CS17BTECH11029).
% -----------------------------------------------

evaluate([A],A). % One number in list is number itself.

% evaluate tries a splitting of list of numbers into two parts & tries to solve both parts.
% checks if both parts can be merged using any of operators.
evaluate(A,B):- divide(A,C,D), evaluate(C,E), evaluate(D,F), (B=E*F; B=E+F; (B=E/F, G is F, G\=0); B=E-F).

nonEmpty([_|_]). % checks if a list is non empty or not.

% divide the list into two non empty exhaustive disjoint parts.
% A is actual list , B and C are splitted parts.
divide(A,B,C):- nonEmpty(B), nonEmpty(C), append(B,C,A). 

% A is list of first N-1 numbers, B is output expression, C is output we want.
find(A,B,C):- evaluate(A,B), X is B, X==C. % evaluates if an assignment is correct or not.

% inp_out returns the first N-1 list and last number as output to feed into find function.
inp_out([A|B],A,X):- reverse(B,X). 

run(_,0). % stops when test cases finishes.

% else read a test case find an assignment and recursively call it again.
% F is file instance and C is number of cases left.
run(F,C) :- read(F,X),reverse(X,X1), inp_out(X1,A,B), 
			((find(B,O,A),write(O),write('='),write(A)); write("IMPOSSIBLE")),
			nl, C2 is C-1, run(F,C2).

% get the file instance , read the number of test cases and run.
main :-  open('input.txt',read,F), read(F,X),run(F,X).