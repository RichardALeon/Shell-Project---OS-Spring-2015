all:
	lex coolshell.l;
	yacc -d coolshell.y;
	cc lex.yy.c y.tab.c coolshell.c -o main
clean:
	rm -rf *o cool main