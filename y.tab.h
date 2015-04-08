/* A Bison parser, made by GNU Bison 3.0.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2013 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    BYE = 258,
    CD = 259,
    PWD = 260,
    SET_ENVIRONMENT = 261,
    UNSET_ENVIRONMENT = 262,
    PRINT_ENVIRONMENT = 263,
    LESSTHAN = 264,
    ALIAS = 265,
    GREATERTHAN = 266,
    QUOT = 267,
    AMP = 268,
    BACKSLASH = 269,
    PIPE = 270,
    UNALIAS = 271,
    PRINTALIAS = 272,
    WORD = 273
  };
#endif
/* Tokens.  */
#define BYE 258
#define CD 259
#define PWD 260
#define SET_ENVIRONMENT 261
#define UNSET_ENVIRONMENT 262
#define PRINT_ENVIRONMENT 263
#define LESSTHAN 264
#define ALIAS 265
#define GREATERTHAN 266
#define QUOT 267
#define AMP 268
#define BACKSLASH 269
#define PIPE 270
#define UNALIAS 271
#define PRINTALIAS 272
#define WORD 273

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE YYSTYPE;
union YYSTYPE
{
#line 16 "coolshell.y" /* yacc.c:1909  */

	int number;
	char *string;

#line 95 "y.tab.h" /* yacc.c:1909  */
};
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
