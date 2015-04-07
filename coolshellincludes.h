/* Environment Variables and related items*/
#define ENV_MAXSIZE 10
#define ERROR_NO_SPACE -10
#define ERROR_NOT_FOUND -11
#define OK -1
char *env_vars[ENV_MAXSIZE];
char *env_vars_values[ENV_MAXSIZE];


int env_var_count;
int set_env_var(char*, char*);
int unset_env_var(char*);
void print_env_var(void);

struct alias {
	char *aliasname;
	char *aliascontent;
};