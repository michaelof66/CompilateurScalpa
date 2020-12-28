/* A Bison parser, made by GNU Bison 3.5.1.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2020 Free Software Foundation,
   Inc.

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

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

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
    PROGRAM = 258,
    VAR = 259,
    ID = 260,
    STR = 261,
    NUM = 262,
    UNIT = 263,
    BOOL = 264,
    INT = 265,
    PLUS = 266,
    AFFECT = 267,
    TIMES = 268,
    MINUS = 269,
    DIVIDE = 270,
    POWER = 271,
    TRUE = 272,
    FALSE = 273,
    INF = 274,
    INFEQ = 275,
    SUP = 276,
    SUPEQ = 277,
    DIFF = 278,
    EQ = 279,
    AND = 280,
    OR = 281,
    XOR = 282,
    NOT = 283,
    SBEGIN = 284,
    SEND = 285,
    WRITE = 286,
    READ = 287,
    IF = 288,
    THEN = 289,
    ELSE = 290,
    WHILE = 291,
    DO = 292,
    RETURN = 293,
    LOWER_THAN_ELSE = 294,
    NEG = 295
  };
#endif
/* Tokens.  */
#define PROGRAM 258
#define VAR 259
#define ID 260
#define STR 261
#define NUM 262
#define UNIT 263
#define BOOL 264
#define INT 265
#define PLUS 266
#define AFFECT 267
#define TIMES 268
#define MINUS 269
#define DIVIDE 270
#define POWER 271
#define TRUE 272
#define FALSE 273
#define INF 274
#define INFEQ 275
#define SUP 276
#define SUPEQ 277
#define DIFF 278
#define EQ 279
#define AND 280
#define OR 281
#define XOR 282
#define NOT 283
#define SBEGIN 284
#define SEND 285
#define WRITE 286
#define READ 287
#define IF 288
#define THEN 289
#define ELSE 290
#define WHILE 291
#define DO 292
#define RETURN 293
#define LOWER_THAN_ELSE 294
#define NEG 295

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 216 "ar.y"

	char *strval;
	int intval;
	struct P_symb **psymb;
	struct ident_list* list;
	struct quadop* exprval;
	struct {
		struct lpos* false;
		struct lpos* true;
	} tf;
	struct lpos* lpos;
	int actualquad;

#line 151 "y.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
