/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.0.4"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* Copy the first part of user declarations.  */
#line 1 "ar.y" /* yacc.c:339  */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "include/token_tab.h"
#include "include/fct_utilitaires.h"
#include "include/quad.h"
#include "include/mips.h"
extern quad globalcode[300];
extern int nextquad;
extern int ntp;

void yyerror(char*);
int yylex();
void lex_free();

int file_code_mips;

#define SIZE_INSTR 100
void MIPS_OPREL_COMP(quad* q)
{
    char instr[SIZE_INSTR];
    switch (q->type)
    {
    case Q_SUP:
        snprintf(instr, SIZE_INSTR - strlen(instr), "bgt ");
        break;

    case Q_SUPEQ:
        snprintf(instr, SIZE_INSTR - strlen(instr), "bge ");
        break;
    case Q_INF:
        snprintf(instr, SIZE_INSTR - strlen(instr), "blt ");
        break;
    case Q_INFEQ:
        snprintf(instr, SIZE_INSTR - strlen(instr), "ble ");
        break;
    case Q_EQ:
        snprintf(instr, SIZE_INSTR - strlen(instr), "ble ");
        break;
    }

    if (strlen(instr) != 0 && (q->type == Q_SUP || q->type == Q_SUPEQ || q->type == Q_INF || q->type == Q_INFEQ || q->type == Q_EQ))
    {
        if (q->op1->type == QO_CST)
            snprintf(&instr[strlen(instr)], SIZE_INSTR - strlen(instr), "%i, ", q->op1->u.cst);
        else
            snprintf(&instr[strlen(instr)], SIZE_INSTR - strlen(instr), "$%s, ", q->op1->u.name);

        if (q->op2->type == QO_CST)
            snprintf(&instr[strlen(instr)], SIZE_INSTR - strlen(instr), "%i, ", q->op2->u.cst);
        else
            snprintf(&instr[strlen(instr)], SIZE_INSTR - strlen(instr), "$%s, ", q->op2->u.name);

        snprintf(&instr[strlen(instr)], SIZE_INSTR - strlen(instr), "QUAD_%i\n", q->res->u.cst);
		//printf("%s\n", instr);
	}
    else {
        snprintf(instr, SIZE_INSTR, "quad pas encore pris en charge\n");
    }
	instr[strlen(instr)]='\0';
	//printf("chaine :%s:\n", instr);
	write(file_code_mips, instr, strlen(instr));
}

// void MIPS_OPREL_RESTE(quad* q)
// {
//     char instr[SIZE_INSTR];
//     switch (q->type)
//     {
//     case Q_GOTO:
//         snprintf(instr, SIZE_INSTR - strlen(instr), "j QUAD_%i\n", q->res->u.cst);
//         break;
// 	// case Q_AFFECT: aucune idée de comment récupérer le registre de la variable initiale
//     //     snprintf(instr, SIZE_INSTR - strlen(instr), "lw $t0, %i\n", q->res->u.cst);
//     //     break;

//     case Q_WRITE:
//         snprintf(instr, SIZE_INSTR - strlen(instr), "li $v0 ";
// 		if(q->res->type == QO_CST)
// 			snprintf(instr, SIZE_INSTR - strlen(instr), "1\nlw $a0, %i", q->res->u.cst);
// 		else//QO_NAME
// 			snprintf(instr, SIZE_INSTR - strlen(instr), "1\nlw $a0, %s", q->res->u.name);
// 		snprintf(instr, SIZE_INSTR - strlen(instr), "\nsyscall");
//         break;
//     }

// 	instr[strlen(instr)]='\0';
// 	printf("chaine :%s:\n", instr);
// 	write(file_code_mips, instr, strlen(instr));
// }


void trad_mips_all(int argc, char **argv)
{
    file_code_mips = open(argv[1], O_CREAT | O_APPEND | O_WRONLY | O_TRUNC);
    if (file_code_mips == -1)
    {
        fprintf(stderr, "Erreur durant l'ouverture du fichier\n");
        exit(-1);
    }
	for(int i = 0 ; i < nextquad; i++){
		//printf("%i %i\n", i, nextquad);
		MIPS_OPREL_COMP(&globalcode[i]);
	}
    int ret_close = close(file_code_mips);
    if (ret_close == -1)
    {
        fprintf(stderr, "Erreur durant la fermeture du fichier\n");
        exit(-1);
    }
}



#line 186 "y.tab.c" /* yacc.c:339  */

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* In a future release of Bison, this section will be replaced
   by #include "y.tab.h".  */
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
    ENDIF = 291,
    WHILE = 292,
    DO = 293,
    DONE = 294,
    RETURN = 295,
    NEG = 296
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
#define ENDIF 291
#define WHILE 292
#define DO 293
#define DONE 294
#define RETURN 295
#define NEG 296

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 121 "ar.y" /* yacc.c:355  */

	char *strval;
	int intval;
	struct P_symb **psymb;
	struct ident_list* list;
	struct quadop* exprval;
	struct {
		struct lpos* true;
		struct lpos* false;
	} tf;
	struct lpos* lpos;
	int actualquad;

#line 322 "y.tab.c" /* yacc.c:355  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 339 "y.tab.c" /* yacc.c:358  */

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

#ifndef YY_ATTRIBUTE
# if (defined __GNUC__                                               \
      && (2 < __GNUC__ || (__GNUC__ == 2 && 96 <= __GNUC_MINOR__)))  \
     || defined __SUNPRO_C && 0x5110 <= __SUNPRO_C
#  define YY_ATTRIBUTE(Spec) __attribute__(Spec)
# else
#  define YY_ATTRIBUTE(Spec) /* empty */
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# define YY_ATTRIBUTE_PURE   YY_ATTRIBUTE ((__pure__))
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# define YY_ATTRIBUTE_UNUSED YY_ATTRIBUTE ((__unused__))
#endif

#if !defined _Noreturn \
     && (!defined __STDC_VERSION__ || __STDC_VERSION__ < 201112)
# if defined _MSC_VER && 1200 <= _MSC_VER
#  define _Noreturn __declspec (noreturn)
# else
#  define _Noreturn YY_ATTRIBUTE ((__noreturn__))
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif


#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYSIZE_T yynewbytes;                                            \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / sizeof (*yyptr);                          \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  4
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   125

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  47
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  15
/* YYNRULES -- Number of rules.  */
#define YYNRULES  50
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  94

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   296

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      45,    46,     2,     2,    44,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    43,    42,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   168,   168,   171,   172,   173,   175,   178,   179,   182,
     184,   185,   186,   189,   194,   199,   208,   216,   221,   226,
     227,   228,   233,   240,   241,   242,   246,   247,   248,   249,
     256,   265,   271,   277,   282,   287,   296,   303,   312,   313,
     314,   315,   316,   319,   320,   321,   322,   323,   324,   326,
     330
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "PROGRAM", "VAR", "ID", "STR", "NUM",
  "UNIT", "BOOL", "INT", "PLUS", "AFFECT", "TIMES", "MINUS", "DIVIDE",
  "POWER", "TRUE", "FALSE", "INF", "INFEQ", "SUP", "SUPEQ", "DIFF", "EQ",
  "AND", "OR", "XOR", "NOT", "SBEGIN", "SEND", "WRITE", "READ", "IF",
  "THEN", "ELSE", "ENDIF", "WHILE", "DO", "DONE", "RETURN", "NEG", "';'",
  "':'", "','", "'('", "')'", "$accept", "program", "vardecllist",
  "varsdecl", "identlist", "typename", "atomictype", "instr", "sequence",
  "E", "cond", "opb", "oprel", "M", "tag", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,    59,    58,    44,    40,    41
};
# endif

#define YYPACT_NINF -41

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-41)))

#define YYTABLE_NINF -25

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int8 yypact[] =
{
       1,    10,    22,    26,   -41,    21,    73,   -10,   -41,   -35,
      31,    63,     6,    28,     7,   -41,     6,   -41,    26,    74,
      39,     6,   -41,    11,    18,   -41,   -41,     6,     6,    61,
     -41,   -41,   -41,     7,     7,   101,    16,     7,    61,   -41,
     -41,   -41,   -41,   -41,   -41,   -41,    61,    40,    29,   -41,
      23,   -41,   -41,   -41,   -41,   -41,     6,   -41,    43,    35,
     -41,   -41,   -41,   -41,   -41,   -41,     6,   -41,   -41,   -41,
      -7,    73,   -41,   -41,    61,   -41,    61,     7,     7,    73,
     -41,   -41,   -41,    55,    50,    73,   -41,    52,    49,   -41,
     -41,    73,    54,   -41
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,     0,     5,     1,     0,     0,     3,     7,     0,
       0,     0,     0,     0,     0,    49,    18,     2,     5,     0,
       0,     0,    20,    25,     0,    26,    27,     0,     0,    22,
      21,    36,    37,     0,     0,     0,     0,     0,    17,     4,
      10,    11,    12,     6,     9,     8,    13,    49,     0,    30,
       0,    38,    40,    39,    41,    42,     0,    33,     0,     0,
      43,    44,    45,    46,    48,    47,     0,    49,    49,    49,
       0,     0,    19,    28,    29,    34,    35,     0,     0,     0,
      49,    23,    32,    31,    50,     0,    14,     0,     0,    49,
      16,     0,     0,    15
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -41,   -41,    79,   -41,   -41,   -41,   -41,    -6,    20,   -11,
     -31,   -41,   -41,   -40,   -41
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
      -1,     2,     6,     7,     9,    43,    44,    23,    24,    35,
      36,    56,    66,    37,    87
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int8 yytable[] =
{
      17,    29,    57,    59,     1,    38,    70,    71,    19,    20,
      46,    25,    25,    26,    26,     3,    49,    50,    67,    68,
      27,    27,     4,    58,    31,    32,     8,    77,    78,    79,
       5,    80,    18,    30,    51,    33,    52,    53,    54,    55,
      85,    67,    68,    21,    45,    74,    82,    83,    48,    91,
      69,    28,    34,    47,    51,    76,    52,    53,    54,    55,
      67,    68,    60,    61,    62,    63,    64,    65,    10,    73,
     -24,    72,    51,    84,    52,    53,    54,    55,    10,    88,
      67,    75,    40,    41,    42,    92,    86,    89,    90,    73,
      93,    81,    11,    22,    12,    13,    14,    39,     0,     0,
      15,     0,    11,    16,    12,    13,    14,     0,     0,     0,
      15,     0,    51,    16,    52,    53,    54,    55,     0,     0,
      60,    61,    62,    63,    64,    65
};

static const yytype_int8 yycheck[] =
{
       6,    12,    33,    34,     3,    16,    37,    47,    43,    44,
      21,     5,     5,     7,     7,     5,    27,    28,    25,    26,
      14,    14,     0,    34,    17,    18,     5,    67,    68,    69,
       4,    38,    42,     5,    11,    28,    13,    14,    15,    16,
      80,    25,    26,    12,     5,    56,    77,    78,    30,    89,
      34,    45,    45,    42,    11,    66,    13,    14,    15,    16,
      25,    26,    19,    20,    21,    22,    23,    24,     5,    46,
      30,    42,    11,    79,    13,    14,    15,    16,     5,    85,
      25,    46,     8,     9,    10,    91,    36,    35,    39,    46,
      36,    71,    29,    30,    31,    32,    33,    18,    -1,    -1,
      37,    -1,    29,    40,    31,    32,    33,    -1,    -1,    -1,
      37,    -1,    11,    40,    13,    14,    15,    16,    -1,    -1,
      19,    20,    21,    22,    23,    24
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     3,    48,     5,     0,     4,    49,    50,     5,    51,
       5,    29,    31,    32,    33,    37,    40,    54,    42,    43,
      44,    12,    30,    54,    55,     5,     7,    14,    45,    56,
       5,    17,    18,    28,    45,    56,    57,    60,    56,    49,
       8,     9,    10,    52,    53,     5,    56,    42,    30,    56,
      56,    11,    13,    14,    15,    16,    58,    57,    56,    57,
      19,    20,    21,    22,    23,    24,    59,    25,    26,    34,
      57,    60,    42,    46,    56,    46,    56,    60,    60,    60,
      38,    55,    57,    57,    54,    60,    36,    61,    54,    35,
      39,    60,    54,    36
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    47,    48,    49,    49,    49,    50,    51,    51,    52,
      53,    53,    53,    54,    54,    54,    54,    54,    54,    54,
      54,    54,    54,    55,    55,    55,    56,    56,    56,    56,
      56,    57,    57,    57,    57,    57,    57,    57,    58,    58,
      58,    58,    58,    59,    59,    59,    59,    59,    59,    60,
      61
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     4,     1,     3,     0,     4,     1,     3,     1,
       1,     1,     1,     3,     6,    10,     7,     2,     1,     4,
       2,     2,     2,     4,     2,     1,     1,     1,     3,     3,
       2,     4,     4,     2,     3,     3,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     0,
       0
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                  \
do                                                              \
  if (yychar == YYEMPTY)                                        \
    {                                                           \
      yychar = (Token);                                         \
      yylval = (Value);                                         \
      YYPOPSTACK (yylen);                                       \
      yystate = *yyssp;                                         \
      goto yybackup;                                            \
    }                                                           \
  else                                                          \
    {                                                           \
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;                                                  \
    }                                                           \
while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256



/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)

/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*----------------------------------------.
| Print this symbol's value on YYOUTPUT.  |
`----------------------------------------*/

static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
  YYUSE (yytype);
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyoutput, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yytype_int16 *yyssp, YYSTYPE *yyvsp, int yyrule)
{
  unsigned long int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[yyssp[yyi + 1 - yynrhs]],
                       &(yyvsp[(yyi + 1) - (yynrhs)])
                                              );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
yystrlen (const char *yystr)
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            /* Fall through.  */
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (! (yysize <= yysize1
                         && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                    return 2;
                  yysize = yysize1;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
      return 2;
    yysize = yysize1;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
{
  YYUSE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        YYSTYPE *yyvs1 = yyvs;
        yytype_int16 *yyss1 = yyss;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * sizeof (*yyssp),
                    &yyvs1, yysize * sizeof (*yyvsp),
                    &yystacksize);

        yyss = yyss1;
        yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yytype_int16 *yyss1 = yyss;
        union yyalloc *yyptr =
          (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
                  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 3:
#line 171 "ar.y" /* yacc.c:1646  */
    {}
#line 1486 "y.tab.c" /* yacc.c:1646  */
    break;

  case 4:
#line 172 "ar.y" /* yacc.c:1646  */
    {}
#line 1492 "y.tab.c" /* yacc.c:1646  */
    break;

  case 5:
#line 173 "ar.y" /* yacc.c:1646  */
    {}
#line 1498 "y.tab.c" /* yacc.c:1646  */
    break;

  case 6:
#line 175 "ar.y" /* yacc.c:1646  */
    {create_symblist("var",(yyvsp[-2].list), (yyvsp[0].strval));}
#line 1504 "y.tab.c" /* yacc.c:1646  */
    break;

  case 7:
#line 178 "ar.y" /* yacc.c:1646  */
    {(yyval.list) = create_identlist((yyvsp[0].strval));}
#line 1510 "y.tab.c" /* yacc.c:1646  */
    break;

  case 8:
#line 179 "ar.y" /* yacc.c:1646  */
    {(yyval.list) = add_to_identlist((yyvsp[-2].list), (yyvsp[0].strval));}
#line 1516 "y.tab.c" /* yacc.c:1646  */
    break;

  case 9:
#line 182 "ar.y" /* yacc.c:1646  */
    {(yyval.strval) = (yyvsp[0].strval);}
#line 1522 "y.tab.c" /* yacc.c:1646  */
    break;

  case 10:
#line 184 "ar.y" /* yacc.c:1646  */
    {(yyval.strval) = "unit";}
#line 1528 "y.tab.c" /* yacc.c:1646  */
    break;

  case 11:
#line 185 "ar.y" /* yacc.c:1646  */
    {(yyval.strval) = "bool";}
#line 1534 "y.tab.c" /* yacc.c:1646  */
    break;

  case 12:
#line 186 "ar.y" /* yacc.c:1646  */
    {(yyval.strval) = "int";}
#line 1540 "y.tab.c" /* yacc.c:1646  */
    break;

  case 13:
#line 190 "ar.y" /* yacc.c:1646  */
    {
	 	  quad q = quad_make(Q_AFFECT, (yyvsp[0].exprval), NULL, quadop_name((yyvsp[-2].strval)));
	 	  gencode(q);
	  }
#line 1549 "y.tab.c" /* yacc.c:1646  */
    break;

  case 14:
#line 195 "ar.y" /* yacc.c:1646  */
    {
		  complete((yyvsp[-4].tf).true,(yyvsp[-2].actualquad));
		  (yyval.lpos) = concat((yyvsp[-4].tf).false,crelist(nextquad));
	  }
#line 1558 "y.tab.c" /* yacc.c:1646  */
    break;

  case 15:
#line 200 "ar.y" /* yacc.c:1646  */
    {
		  complete((yyvsp[-8].tf).true, (yyvsp[-6].actualquad));
		  complete((yyvsp[-8].tf).false, (yyvsp[-2].actualquad));
		  (yyval.lpos) = concat((yyvsp[-5].lpos), (yyvsp[-4].lpos));
		  (yyval.lpos) = concat((yyval.lpos), crelist(nextquad));
		  quad q = quad_make(Q_GOTO,NULL,NULL,quadop_cst(-1));
		  gencode(q);
	  }
#line 1571 "y.tab.c" /* yacc.c:1646  */
    break;

  case 16:
#line 209 "ar.y" /* yacc.c:1646  */
    {
	  		complete((yyvsp[-4].tf).true, (yyvsp[-2].actualquad));
			complete((yyvsp[-1].lpos), (yyvsp[-5].actualquad));
			quad q = quad_make(Q_GOTO,NULL,NULL,quadop_cst((yyvsp[-5].actualquad)));
			gencode(q);
			(yyval.lpos) = (yyvsp[-4].tf).false;
    }
#line 1583 "y.tab.c" /* yacc.c:1646  */
    break;

  case 17:
#line 217 "ar.y" /* yacc.c:1646  */
    {
		  quad q = quad_make(Q_RET,NULL,NULL,(yyvsp[0].exprval));
		  gencode(q);
	  }
#line 1592 "y.tab.c" /* yacc.c:1646  */
    break;

  case 18:
#line 222 "ar.y" /* yacc.c:1646  */
    {
		  quad q = quad_make(Q_RET,NULL,NULL,NULL);
		  gencode(q);
	  }
#line 1601 "y.tab.c" /* yacc.c:1646  */
    break;

  case 19:
#line 226 "ar.y" /* yacc.c:1646  */
    {(yyval.lpos) = (yyvsp[-2].lpos); }
#line 1607 "y.tab.c" /* yacc.c:1646  */
    break;

  case 20:
#line 227 "ar.y" /* yacc.c:1646  */
    { }
#line 1613 "y.tab.c" /* yacc.c:1646  */
    break;

  case 21:
#line 229 "ar.y" /* yacc.c:1646  */
    {
		  quad q = quad_make(Q_READ, NULL, NULL, quadop_name((yyvsp[0].strval)));
		  gencode(q);
	  }
#line 1622 "y.tab.c" /* yacc.c:1646  */
    break;

  case 22:
#line 234 "ar.y" /* yacc.c:1646  */
    {
		  quad q = quad_make(Q_WRITE, NULL, NULL, (yyvsp[0].exprval));
		  gencode(q);
	  }
#line 1631 "y.tab.c" /* yacc.c:1646  */
    break;

  case 23:
#line 240 "ar.y" /* yacc.c:1646  */
    { (yyvsp[-3].lpos) = crelist(nextquad);complete((yyvsp[-3].lpos), (yyvsp[-1].actualquad)); (yyval.lpos) = (yyvsp[0].lpos);}
#line 1637 "y.tab.c" /* yacc.c:1646  */
    break;

  case 24:
#line 241 "ar.y" /* yacc.c:1646  */
    { (yyval.lpos) = (yyvsp[-1].lpos); }
#line 1643 "y.tab.c" /* yacc.c:1646  */
    break;

  case 25:
#line 242 "ar.y" /* yacc.c:1646  */
    { (yyval.lpos) = (yyvsp[0].lpos); }
#line 1649 "y.tab.c" /* yacc.c:1646  */
    break;

  case 26:
#line 246 "ar.y" /* yacc.c:1646  */
    { (yyval.exprval) = quadop_name((yyvsp[0].strval));}
#line 1655 "y.tab.c" /* yacc.c:1646  */
    break;

  case 27:
#line 247 "ar.y" /* yacc.c:1646  */
    { (yyval.exprval) = quadop_cst((yyvsp[0].intval));}
#line 1661 "y.tab.c" /* yacc.c:1646  */
    break;

  case 28:
#line 248 "ar.y" /* yacc.c:1646  */
    { (yyval.exprval) = (yyvsp[-1].exprval);}
#line 1667 "y.tab.c" /* yacc.c:1646  */
    break;

  case 29:
#line 250 "ar.y" /* yacc.c:1646  */
    {
	  quadop* t = new_temp();
	  quad q = quad_make((yyvsp[-1].intval), (yyvsp[-2].exprval), (yyvsp[0].exprval), t);
	  gencode(q);
	  (yyval.exprval) = t;
}
#line 1678 "y.tab.c" /* yacc.c:1646  */
    break;

  case 30:
#line 257 "ar.y" /* yacc.c:1646  */
    {
	quadop* t = new_temp();
	quad q = quad_make(Q_NEG, (yyvsp[0].exprval), NULL, t);
	gencode(q);
	(yyval.exprval) = t;
}
#line 1689 "y.tab.c" /* yacc.c:1646  */
    break;

  case 31:
#line 266 "ar.y" /* yacc.c:1646  */
    {
		(yyval.tf).true = concat ((yyvsp[-3].tf).true, (yyvsp[0].tf).true);
		complete((yyvsp[-3].tf).false, (yyvsp[-1].actualquad));
		(yyval.tf).false = (yyvsp[0].tf).false;
	}
#line 1699 "y.tab.c" /* yacc.c:1646  */
    break;

  case 32:
#line 272 "ar.y" /* yacc.c:1646  */
    {
		(yyval.tf).false = concat ((yyvsp[-3].tf).false, (yyvsp[0].tf).false);
		complete((yyvsp[-3].tf).true, (yyvsp[-1].actualquad));
		(yyval.tf).true = (yyvsp[0].tf).true;
	}
#line 1709 "y.tab.c" /* yacc.c:1646  */
    break;

  case 33:
#line 278 "ar.y" /* yacc.c:1646  */
    {
		(yyval.tf).true = (yyvsp[0].tf).false;
		(yyval.tf).false = (yyvsp[0].tf).true;
	}
#line 1718 "y.tab.c" /* yacc.c:1646  */
    break;

  case 34:
#line 283 "ar.y" /* yacc.c:1646  */
    {
		(yyval.tf).true = (yyvsp[-1].tf).true;
		(yyval.tf).false = (yyvsp[-1].tf).false;
	}
#line 1727 "y.tab.c" /* yacc.c:1646  */
    break;

  case 35:
#line 288 "ar.y" /* yacc.c:1646  */
    {
		(yyval.tf).true = crelist(nextquad);
		quad q = quad_make((yyvsp[-1].intval),(yyvsp[-2].exprval),(yyvsp[0].exprval),NULL);
		gencode (q); // if ($1 rel $3)     goto ?
		(yyval.tf).false = crelist(nextquad);
		quad q2 = quad_make(Q_GOTO,NULL,NULL,quadop_cst(-1));
		gencode(q2);
	}
#line 1740 "y.tab.c" /* yacc.c:1646  */
    break;

  case 36:
#line 297 "ar.y" /* yacc.c:1646  */
    {
		(yyval.tf).true = crelist(nextquad);
		quad q2 = quad_make(Q_GOTO,NULL,NULL,quadop_cst(-1));
		gencode(q2);
		(yyval.tf).false = NULL;
	}
#line 1751 "y.tab.c" /* yacc.c:1646  */
    break;

  case 37:
#line 304 "ar.y" /* yacc.c:1646  */
    {
		(yyval.tf).false = crelist(nextquad);
		quad q2 = quad_make(Q_GOTO,NULL,NULL,quadop_cst(-1));
		gencode(q2);
		(yyval.tf).true = NULL;
	}
#line 1762 "y.tab.c" /* yacc.c:1646  */
    break;

  case 38:
#line 312 "ar.y" /* yacc.c:1646  */
    { (yyval.intval) = Q_PLUS; }
#line 1768 "y.tab.c" /* yacc.c:1646  */
    break;

  case 39:
#line 313 "ar.y" /* yacc.c:1646  */
    { (yyval.intval) = Q_MINUS; }
#line 1774 "y.tab.c" /* yacc.c:1646  */
    break;

  case 40:
#line 314 "ar.y" /* yacc.c:1646  */
    { (yyval.intval) = Q_TIMES; }
#line 1780 "y.tab.c" /* yacc.c:1646  */
    break;

  case 41:
#line 315 "ar.y" /* yacc.c:1646  */
    { (yyval.intval) = Q_DIVIDE; }
#line 1786 "y.tab.c" /* yacc.c:1646  */
    break;

  case 42:
#line 316 "ar.y" /* yacc.c:1646  */
    { (yyval.intval) = Q_POWER; }
#line 1792 "y.tab.c" /* yacc.c:1646  */
    break;

  case 43:
#line 319 "ar.y" /* yacc.c:1646  */
    { (yyval.intval) = Q_INF; }
#line 1798 "y.tab.c" /* yacc.c:1646  */
    break;

  case 44:
#line 320 "ar.y" /* yacc.c:1646  */
    { (yyval.intval) = Q_INFEQ; }
#line 1804 "y.tab.c" /* yacc.c:1646  */
    break;

  case 45:
#line 321 "ar.y" /* yacc.c:1646  */
    { (yyval.intval) = Q_SUP; }
#line 1810 "y.tab.c" /* yacc.c:1646  */
    break;

  case 46:
#line 322 "ar.y" /* yacc.c:1646  */
    { (yyval.intval) = Q_SUPEQ; }
#line 1816 "y.tab.c" /* yacc.c:1646  */
    break;

  case 47:
#line 323 "ar.y" /* yacc.c:1646  */
    { (yyval.intval) = Q_EQ; }
#line 1822 "y.tab.c" /* yacc.c:1646  */
    break;

  case 48:
#line 324 "ar.y" /* yacc.c:1646  */
    { (yyval.intval) = Q_DIFF; }
#line 1828 "y.tab.c" /* yacc.c:1646  */
    break;

  case 49:
#line 326 "ar.y" /* yacc.c:1646  */
    { (yyval.actualquad) = nextquad; }
#line 1834 "y.tab.c" /* yacc.c:1646  */
    break;

  case 50:
#line 330 "ar.y" /* yacc.c:1646  */
    {
	  (yyval.lpos) = crelist(nextquad);
	  quad q = quad_make(Q_GOTO,NULL,NULL,quadop_cst(-1));
	  gencode(q);
}
#line 1844 "y.tab.c" /* yacc.c:1646  */
    break;


#line 1848 "y.tab.c" /* yacc.c:1646  */
      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYTERROR;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  return yyresult;
}
#line 337 "ar.y" /* yacc.c:1906  */

void yyerror (char *s) {
	fprintf(stderr, "[Yacc] error: %s\n", s);
}


int main(int argc, char** argv) {
	init_symb_tab();
	printf("Enter your code:\n");

	yyparse();
	printf("-----------------\nSymbol table:\n-----------------\n");
	print_tab();
	printf("Quad list:\n");
	for (int i=0; i<nextquad; i++) {
		printf("idx : %i\t\t", i);affiche(globalcode[i]);
	}
	trad_mips_all(argc, argv);
	// Be clean.===> Ofc As always
	lex_free();
	return 0;
}

/*
*	Test fonctionnel : creation de variable:
*
*	Ce test contient tout type de symbole afin de recouvrir la totalité
*	des cas : symboles doublons d'indice de hachage mais symb différent,
*	test avec symbole doublon (et donc refus d'ajouter dans la table),
*	ajout symbole classique.
*
*	./ar < file_test/test_declaration_var
*	./ar < file_test/test_instr
*	./ar < file_test/test_suite_instr
*
*
*	changement fait par rapport branche dev:
*	fonction affichage des tokens de la tab
*   toutes les fonctions liés au mips 
*
*   commande pour tester la fonctionnalité (yet):
*   ./ar mario < file_test/test_suite_instr
*   (mario cest le fichier decriture, pas d'inspiration pour le nom
*   et vu que je suis en kigu de mario toute la journée voila)
*
*/