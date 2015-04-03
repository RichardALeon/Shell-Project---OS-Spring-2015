%{
	#include <stdio.h>
	#include <string.h>
	#include <stdlib.h>
	#include "coolshellincludes.h"
	extern char* yytext;

	void yyerror(const char *str)
	{
		fprintf(stderr, "error: %s\n", str);
	}
%}

%union
{
	int number;
	char *string;
}

%token <number> BYE CD PWD
%token <string> WORD

%%
commands: /* empty */
	|	commands command;

command:
	kill_self
	|
	get_path
	|
	change_directory
	|
	nonsense
	;

kill_self:
	BYE
	{
		printf("Goodbye!\n");
		exit(0);
	}
	;

get_path:
	PWD
	{
		char cwd[1024];
		getcwd(cwd, sizeof(cwd));
		printf("%s\n", cwd);
	}
	;


change_directory:
	CD
	{
	char* home = getenv("HOME");
	chdir(home);
	}
	|
	CD WORD
	{
	chdir($2);
	}
	;

nonsense:
	WORD
	{
	printf("I do not recognize that command.\n");
	}
	|
	nonsense WORD
	{
	printf("What?\n");
	}
	;


