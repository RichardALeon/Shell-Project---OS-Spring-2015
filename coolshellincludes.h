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
