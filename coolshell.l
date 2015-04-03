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
[a-zA-Z0-9\/-]+ 	yylval.string=strdup(yytext); return WORD;
\"(.)*\"	return WORD;

[ \t] /*ignore*/
"\n" return 0;

%%
	/* subroutines here */
int yywrap(void)
{
	return 1;
}