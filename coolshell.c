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


/* Definitions */

#define SHELL_PROMPT	" (﻿ ͡° ͜ʖ ͡°) ~~ "

int set_env_var(char *name, char *value) {
	int indexOf = -1;

	int i = 0;
	//Find index of variable, if it exists
	for(i = 0; i < env_var_count; i++) {
		if(strcmp(name, env_vars[i]) == 0) {
			//Variable already exists, set value
			indexOf = i;
			break;
		}
	}

	//Does the variable already exist?
	if(indexOf != -1) {
		//Modify existing variable
		env_vars_values[indexOf] = value;
		return;
	} else {
		//Create new variable if space permits
		if(env_var_count < ENV_MAXSIZE) {
			//There's space. Fill the first available space
			env_vars[env_var_count] = name;
			env_vars_values[env_var_count] = value;
			env_var_count++;
			return OK;
		} else return ERROR_NO_SPACE;
	}
}

int unset_env_var(char *name) {
	int i = 0;
	//Remove value from the list if variable exists
	for( i = 0; i < env_var_count; i++ ) {
		if(strcmp(name, env_vars[i]) == 0) {
			env_vars_values[i] = NULL;
			return OK;
		}
	}

	//If it was not found return NOT FOUND
	return ERROR_NOT_FOUND;

}

void print_env_var(void) {
	int i = 0;
	for( i = 0; i < env_var_count; i++) {
		printf("%s: %s\n", env_vars[i], env_vars_values[i]);
	}
	return;
}

void shellinit(void) {
	env_var_count = 0;
	set_env_var("PATH", getenv("PATH"));

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