/* CONSTANTS */

int set_env_var(char*, char*);
int unset_env_var(char*);
void print_env_var(void);

typedef struct alias alias;

struct alias {
	char *aliasname;
	char *aliascontent;
};

/* ALIAS RELATED CONSTANTS AND THE LIKE */
#define ALIASES_MAXSIZE 10
#define ERROR_NO_SPACE -10
#define ERROR_NOT_FOUND -11
#define OK -1

int alias_count;

int setalias(char*, char*);
int removealias(char*);
void print_aliases(void);
alias aliases[ALIASES_MAXSIZE];

/* COMMAND AND ARGUMENT STUCTURES */
#define MAX_ARGUMENTS 5
#define MAX_COMMANDS 500
char *externcommand;
char *alias_root;
int COMMAND_COUNT;
int num_commands_ahead;

typedef struct com {
	char *commandname;
	int command_code;
	int num_arguments;
	char *args[MAX_ARGUMENTS];
	char *in_file;
	char *out_file;
	int append;
	int is_external;
	int pipe_out;
} COMMAND;

int ignore_EOF;

COMMAND CMD_TABLE[MAX_COMMANDS];

/* BUILTIN COMMANDS */
#define CMD_CD_HOME 0
#define CMD_CD_DIR 1
#define CMD_SETENV 2
#define CMD_UNSETENV 3
#define CMD_PRINTENV 4
#define CMD_PWD 5
#define CMD_ALIAS 6
#define CMD_UNALIAS 7
#define CMD_PRINTALIAS 8
