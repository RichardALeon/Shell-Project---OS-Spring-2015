	/* definitions go here */
%{
	#include <stdio.h>
	#include "y.tab.h"
%}

%%
	/* rules go here */
bye		return BYE;
cd		return CD;
pwd		return PWD;
setenv	return SET_ENVIRONMENT;
unsetenv return UNSET_ENVIRONMENT;
printenv return PRINT_ENVIRONMENT;
[a-zA-Z0-9\.\/-]+ 	yylval.string=strdup(yytext); return WORD;
\"(\\.|[^"\n])*\"	yylval.string=strdup(yytext); return WORD;
"<"	return LESSTHAN;
">"	return GREATERTHAN;
"|"	return PIPE;
\"	return QUOT;
"\\"	return BACKSLASH;
"&"	return AMP;
\[<>|&\"] /*ignore escape characters*/



[ \t] /*ignore*/
"\n" return 0;

%%
	/* subroutines here */
int yywrap(void)
{
	return 1;
}