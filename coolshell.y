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

%token <number> BYE CD PWD SET_ENVIRONMENT UNSET_ENVIRONMENT PRINT_ENVIRONMENT LESSTHAN ALIAS GREATERTHAN QUOT AMP BACKSLASH PIPE UNALIAS PRINTALIAS
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
	argument
	|
	setenv
	|
	unsetenv
	|
	printenv
	|
	setalias
	|
	printalias
	|
	removealias
	|
	ioredir
	|
	arg_pipe
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
		/*
		char cwd[1024];
		getcwd(cwd, sizeof(cwd));
		printf("%s\n", cwd);
		*/
		CMD_TABLE[COMMAND_COUNT].commandname = "pwd";
		CMD_TABLE[COMMAND_COUNT].command_code = CMD_PWD;
	}
	;


change_directory:
	CD
	{
	/*
	char* home = getenv("HOME");
	chdir(home);
	*/
	CMD_TABLE[COMMAND_COUNT].commandname="cd home";
	CMD_TABLE[COMMAND_COUNT].command_code = CMD_CD_HOME;
	}
	|
	CD WORD
	{
		/*
		if( -1 == chdir($2)) printf("Invalid directory: %s\n", $2);
		*/
		CMD_TABLE[COMMAND_COUNT].commandname="cd directory";
		CMD_TABLE[COMMAND_COUNT].command_code = CMD_CD_DIR;
		CMD_TABLE[COMMAND_COUNT].args[0] = $2;
		CMD_TABLE[COMMAND_COUNT].num_arguments++;
	}
	;

setenv:
	SET_ENVIRONMENT
	{
		/*
		printf("%d\n",set_env_var($2, $3));
		*/
		CMD_TABLE[COMMAND_COUNT].commandname="setenv";
		CMD_TABLE[COMMAND_COUNT].command_code = CMD_SETENV;
	}
	;

unsetenv:
	UNSET_ENVIRONMENT
	{
		/*
		printf("%d\n", unset_env_var($2));
		*/
		CMD_TABLE[COMMAND_COUNT].commandname="unsetenv";
		CMD_TABLE[COMMAND_COUNT].command_code = CMD_UNSETENV;
	}
	;

printenv:
	PRINT_ENVIRONMENT
	{
		CMD_TABLE[COMMAND_COUNT].commandname="printenv";
		CMD_TABLE[COMMAND_COUNT].command_code = CMD_PRINTENV;
		//print_env_var();
	}
	;

ioredir:
	arg_pipe argument
	{
	//Don't do anything right now
	}
	;

arg_pipe:
	command PIPE
	{
		CMD_TABLE[COMMAND_COUNT].pipe_out = 1;
		num_commands_ahead++;
		COMMAND_COUNT++;
		COMMAND_COUNT % MAX_COMMANDS;
		//externcommand = NULL;
	}
	;


setalias:
	ALIAS WORD WORD
	{
		CMD_TABLE[COMMAND_COUNT].commandname="alias";
		CMD_TABLE[COMMAND_COUNT].command_code = CMD_ALIAS;
		CMD_TABLE[COMMAND_COUNT].args[0] = $2;
		CMD_TABLE[COMMAND_COUNT].args[1] = $3;
		CMD_TABLE[COMMAND_COUNT].num_arguments += 2;
	}
	;

removealias:
	UNALIAS
	{
		//removealias($2);
		CMD_TABLE[COMMAND_COUNT].commandname="unalias";
		CMD_TABLE[COMMAND_COUNT].command_code = CMD_UNALIAS;
	}
	;

printalias:
	ALIAS
	{
		//print_aliases();
		CMD_TABLE[COMMAND_COUNT].commandname="printalias";
		CMD_TABLE[COMMAND_COUNT].command_code = CMD_PRINTALIAS;
	}
	;

argument:
	WORD
	{
		//We must check if the entered WORD is an external command (or alias, but that won't be handled here)
		if(CMD_TABLE[COMMAND_COUNT].is_external == 0) {
		//This is a command (or an alias), not an argument. make a new command
			CMD_TABLE[COMMAND_COUNT].is_external = 1;
			//externcommand = $1;
			CMD_TABLE[COMMAND_COUNT].commandname = $1;
		} else {
		//This is an argument. Add it to the current command's arguments list, if space permits
			int numArgs = CMD_TABLE[COMMAND_COUNT].num_arguments;
			
			if(numArgs < MAX_ARGUMENTS) {
				CMD_TABLE[COMMAND_COUNT].args[numArgs] = $1;
				CMD_TABLE[COMMAND_COUNT].num_arguments++;
			}
		}
	}
	|
	argument WORD
	{
		//This is an argument. Add it to the current command's arguments list, if space permits
			int numArgs = CMD_TABLE[COMMAND_COUNT].num_arguments;
			
			if(numArgs < MAX_ARGUMENTS) {
				CMD_TABLE[COMMAND_COUNT].args[numArgs] = $2;
				CMD_TABLE[COMMAND_COUNT].num_arguments++;
			}
	}
	|
	command WORD
	{
		//This is an argument. Add it to the current command's arguments list, if space permits
			int numArgs = CMD_TABLE[COMMAND_COUNT].num_arguments;
			
			if(numArgs < MAX_ARGUMENTS) {
				CMD_TABLE[COMMAND_COUNT].args[numArgs] = $2;
				CMD_TABLE[COMMAND_COUNT].num_arguments++;
			}		
	}
	;


