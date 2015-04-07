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

%token <number> BYE CD PWD SET_ENVIRONMENT UNSET_ENVIRONMENT PRINT_ENVIRONMENT LESSTHAN GREATERTHAN QUOT AMP BACKSLASH PIPE
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
	|
	setenv
	|
	unsetenv
	|
	printenv
	|
	metacharacter
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
		if( -1 == chdir($2)) printf("Invalid directory: %s\n", $2);
	}
	;

setenv:
	SET_ENVIRONMENT WORD WORD
	{
		printf("%d\n",set_env_var($2, $3));
	}
	;

unsetenv:
	UNSET_ENVIRONMENT WORD
	{
		printf("%d\n", unset_env_var($2));
	}
	;

printenv:
	PRINT_ENVIRONMENT
	{
		print_env_var();
	}
	;

metacharacter:
	LESSTHAN
	{
		printf("lessthan");
	} 
	|
	GREATERTHAN
	{
		printf("greaterthan");
	}
	|
	QUOT
	{
		printf("quotationmark");
	}
	|
	AMP
	{
		printf("ampersand");
	}
	|
	BACKSLASH
	{
		printf("backslash");
	}
	|
	PIPE
	{
		printf("pipe");
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


