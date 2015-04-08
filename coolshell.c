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

}

int main(void) {

	shellinit();

	//cool shell begins here
	char *string;
	while(1) {
		printf(SHELL_PROMPT);
		//fgets(str, 100, stdin);
		yyparse();
		
	}
	return 0;
}