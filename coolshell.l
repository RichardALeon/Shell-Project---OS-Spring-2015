	/* definitions go here */
%{
	#include <stdio.h>
	#include "y.tab.h"
	#include "coolshellincludes.h"
%}

%%
	/* rules go here */
^bye	return BYE;
<<EOF>> {if(ignore_EOF == 0) return BYE; else return 0;}
^cd		return CD;
^pwd	return PWD;
^setenv	return SET_ENVIRONMENT;
^alias	return ALIAS;
^unalias	return UNALIAS;
^unsetenv return UNSET_ENVIRONMENT;
^printenv return PRINT_ENVIRONMENT;
"${"[a-zA-z0-9]+"}"		{
						char substr[yyleng-2];
						strncpy(substr, yytext + 2, (yyleng - 3));
						substr[yyleng - 3] = '\0';
						yylval.string=strdup(substr);
						//return VARIABLE;
						}
[a-zA-Z0-9\.\/-]+ 	yylval.string=strdup(yytext); return WORD;
\"(\\.|[^"\n])*\"	{
						char substr[yyleng - 1];
						strncpy(substr, yytext + 1, (yyleng - 2));
						substr[yyleng - 2] = '\0';
						yylval.string=strdup(substr); 
						return WORD;
					}
"<"	return LESSTHAN;
">"	return GREATERTHAN;
"|"	return PIPE;
\"	return QUOT;
"\\"	return BACKSLASH;
"&"	return AMP;
"\\"[<>|&\"] /*ignore escape characters*/



[ \t] /*ignore*/
"\n" return 0;

%%
	/* subroutines here */
int yywrap(void)
{
	return 1;
}

void parse_scan_string(char* str) {
	yy_scan_string(str);
	yyparse();
	yylex_destroy();
}