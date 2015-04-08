/* Inclusions */
#include <stdio.h>
#include "y.tab.h"
#include "coolshellincludes.h"
#include <stdlib.h>
#include <string.h>

/* Extern */
extern int yylex();
extern int yylineno;
extern char* yytext;
extern char **environ;


/* Definitions */

#define SHELL_PROMPT	" (﻿ ͡° ͜ʖ ͡°) ~~ "

int set_env_var(char *name, char *value) {
	return setenv(name, value, 1);
}

int unset_env_var(char *name) {
	return unsetenv(name);
}

int setalias(char *name, char *value) {
	alias newalias;
	newalias.aliasname = name;
	newalias.aliascontent = value;

	int indexOf = -1;

	int i = 0;
	//Find index of variable, if it exists
	for(i = 0; i < alias_count; i++) {
		if(strcmp(name, aliases[i].aliasname) == 0) {
			//Variable already exists, set value
			indexOf = i;
			break;
		}
	}

	//Does the variable already exist?
	if(indexOf != -1) {
		//Modify existing variable
		aliases[i] = newalias;
		return;
	} else {
		//Create new variable if space permits
		if(alias_count < ALIASES_MAXSIZE) {
			//There's space. Fill the first available space
			aliases[i] = newalias;
			alias_count++;
			return OK;
		} else return ERROR_NO_SPACE;
	}

}

int removealias(char *name) {
	int indexOf = -1;
	int i = 0;
	
	for(i = 0; i < alias_count; i++) {
		if(strcmp(aliases[i].aliasname, name) == 0) {
			//If there is a match, move all entries down
			for(indexOf = i; indexOf < alias_count; indexOf++) {
				aliases[indexOf] = aliases[indexOf + 1];
			}
			alias_count--;
			return OK;
		}
	}
	return ERROR_NOT_FOUND;
	//Find the alias

}

void print_aliases(void) {
	int i = 0;
	for(i = 0; i < alias_count; i++) {
		printf("%s : %s\n", aliases[i].aliasname, aliases[i].aliascontent);
	}
}

void print_env_var(void) {
	int i = 0;
	while(environ[i]) {
		printf("%s\n", environ[i++]);
	}}

void shellinit(void) {
	alias_count = 0;
	COMMAND_COUNT = 0;
	externcommand = NULL;
}

void execute_command(void) {
	/*
	* This method checks the current command and infers what needs to be done with the current command.
	* First it checks for externcommand to see if the entered command is NOT built in.
	* If externcommand is NULL, the current command is a BUILT IN command.
	* Otherwise, it may be an ALIAS or an EXTERNAL command.
	* If it is an ALIAS, edit the current command to trace back to the content of the ALIAS and re-execute.
	* If it is an EXTERNAL command, carry out the necessary work to execute it.
	*/
	
	int currcmd = COMMAND_COUNT;

	if(externcommand == NULL) {
		/*
		* HANDLING BUILT IN COMMANDS
		*/
		switch(CMD_TABLE[currcmd].command_code) {
			case CMD_CD_HOME: {
				char* home = getenv("HOME");
				chdir(home);
				break;
			}
			case CMD_CD_DIR: {
				if( chdir(CMD_TABLE[currcmd].args[0]) == -1 ) printf("Invalid directory: %s\n", CMD_TABLE[currcmd].args[0]);
				break;
			}
			case CMD_SETENV: {
				set_env_var(CMD_TABLE[currcmd].args[0], CMD_TABLE[currcmd].args[1]);
				break;
			}
			case CMD_UNSETENV: {
				unset_env_var(CMD_TABLE[currcmd].args[0]);
				break;
			}
			case CMD_PRINTENV: {
				print_env_var();
				break;
			}
			case CMD_PWD: {
				char cwd[1024];
				getcwd(cwd, sizeof(cwd));
				printf("%s\n", cwd);
				break;
			}
			case CMD_ALIAS: {
				setalias(CMD_TABLE[currcmd].args[0], CMD_TABLE[currcmd].args[1]);
				break;
			}
			case CMD_UNALIAS: {
				removealias(CMD_TABLE[currcmd].args[0]);
				break;
			}
			case CMD_PRINTALIAS: {
				print_aliases();
				break;
			}
		}
	} else {
		/*
		* HANDLING ALIASES AND SYSTEM COMMANDS
		*/
		printf("Command to execute: %s", CMD_TABLE[currcmd].commandname);
	}

	// RESET externcommand AND INCREMENT COMMAND_COUNT
		externcommand = NULL;
		COMMAND_COUNT = (COMMAND_COUNT + 1) % 100;
}

int main(void) {

	shellinit();

	//cool shell begins here
	char *string;
	while(1) {
		printf(SHELL_PROMPT);
		//fgets(str, 100, stdin);
		yyparse();
		execute_command();
		
	}
	return 0;
}