/* Inclusions */
#include <stdio.h>
#include "y.tab.h"
#include "coolshellincludes.h"
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

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

int is_alias(char *name) {
	//Returns the index of the alias if the entered string is an alias. Otherwise returns -1

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

	return indexOf;
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
	num_commands_ahead = 0;
	alias_count = 0;
	alias_root = NULL;
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

	COMMAND_COUNT = (COMMAND_COUNT - num_commands_ahead) % MAX_COMMANDS;
	int currcmd = COMMAND_COUNT;

	if(CMD_TABLE[currcmd].is_external == 0) {
		/*
		* HANDLING BUILT IN COMMANDS
		*/
		alias_root = NULL;

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

		int aliasindex = is_alias(CMD_TABLE[currcmd].commandname);

		if(aliasindex != -1 ) {
			//ALIAS
			//printf("Alias found!\n");
			externcommand = NULL;

			//HANDLE INFINITE ALIAS LOOPS
			if(alias_root == NULL) {
				//Assign the first alias to be encountered as the "root alias"
				alias_root = aliases[aliasindex].aliasname;
			}

			//Check any other aliases to see if they point back to the first. It will only detect the loop once the alias referring back to the root is executed.
			if(strcmp(alias_root, aliases[aliasindex].aliascontent) == 0) {
				//Root alias name and content of current alias are the same. LOOP
				printf("ALIAS LOOP FOUND. Terminating\n");
				return;
			} else {
				//No alias loop found. Trace the alias to a command
				parse_scan_string(aliases[aliasindex].aliascontent);
				execute_command();
			}

			
		} else {
			//EXTERNAL COMMAND
			//First, fork.
			//Second, We need to make a string that contains all possible folders in PATH, to check for the command
				alias_root = NULL;
				pid_t childPid = fork();
				int status;
				int success = -1;

					while(waitpid(childPid, &status, 0) == -1) {
						if (errno != EINTR) {
							status = -1;
							break;
						}
					}				

				if(childPid < 0) {
					exit(1);
				}
				else if (childPid == 0) {
					char temp[256];
		 			char* allpaths = strcpy(temp,getenv("PATH"));
					char* token = strtok(allpaths,":");
					char* compare = "/.";

					while (token) {
					    //Try to execute the command
						char path[255];

						if( CMD_TABLE[currcmd].commandname[0] == compare[0] || CMD_TABLE[currcmd].commandname[0] == compare[1]) {
							//The user entered a directory with the command (hopefully). Set path to be this command instead
							strcpy( path, CMD_TABLE[currcmd].commandname );
						}
						else {
							//Make a string that contains the path (that the command might be in)
							strcpy( path, token );
							strcat( path, "/");
							//Append the command name to the path
							strcat(path, CMD_TABLE[currcmd].commandname);
						}

						//Create a NULL-terminating array to use for execv command
						char *commandarray[CMD_TABLE[currcmd].num_arguments + 2];
						commandarray[0] = path;
						commandarray[CMD_TABLE[currcmd].num_arguments + 1] = (char*)NULL;
						
						//Add the arguments to the array
						int i = 0;
						for(i = 0; i < CMD_TABLE[currcmd].num_arguments; i++) {
							commandarray[i + 1] = CMD_TABLE[currcmd].args[i];
						}

						//Try to execute the command. If it fails, go through the loop again with a new path
						if(execv(path, commandarray) == -1) {
							token = strtok(NULL, ":");
							continue;
						} else { 
							_exit(0); 
							success = 1;
							break; }						
					}

					if(success == -1) {
						printf("Oh no! :( Command not found: %s\n", CMD_TABLE[currcmd].commandname );
						_exit(0);
					}
				}


		}

	}

	// RESET externcommand AND INCREMENT COMMAND_COUNT
		//externcommand = NULL;
		//alias_root = NULL;
		COMMAND_COUNT = (COMMAND_COUNT + 1) % 100;
		CMD_TABLE[COMMAND_COUNT].is_external = 0;
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