/* A Bison parser, made by GNU Bison 2.7.  */

/* Bison implementation for Yacc-like parsers in C
   
      Copyright (C) 1984, 1989-1990, 2000-2012 Free Software Foundation, Inc.
   
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
#define YYBISON_VERSION "2.7"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* Copy the first part of user declarations.  */
/* Line 371 of yacc.c  */
#line 4 "Parser.y"

#include <iostream>
#include <string>
#include <map>
#include <cstdlib> //-- I need this for atoi
#include <stdarg.h>
#include <stdio.h>

#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"
#include "glm/mat4x4.hpp"

#include "AST.h"
#include "Location.h"

using namespace std;
using namespace LSystem::AST;

Grammar* mGrammar;

extern char* fileName;

//IdentifierTable				mIdentifiers;
//std::vector<std::string>	mTokenToString;

// #define YYDEBUG 1

//-- Lexer prototype required by bison, aka getNextToken()
int yylex(); 
void yyerror(const char *s, ...);
LSystem::Location Loc();


/* Line 371 of yacc.c  */
#line 103 "Parser.tab.cpp"

# ifndef YY_NULL
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULL nullptr
#  else
#   define YY_NULL 0
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
   by #include "Parser.tab.h".  */
#ifndef YY_YY_PARSER_TAB_H_INCLUDED
# define YY_YY_PARSER_TAB_H_INCLUDED
/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     T_NUM = 258,
     T_FNUM = 259,
     T_NAME = 260,
     T_LP = 261,
     T_RP = 262,
     T_LB = 263,
     T_RB = 264,
     T_LS = 265,
     T_RS = 266,
     T_COLON = 267,
     T_LT = 268,
     T_GT = 269,
     T_CONST = 270,
     T_VAR = 271,
     T_INITIAL = 272,
     T_WHEN = 273,
     T_REPLACE = 274,
     T_DECOMPOSE = 275,
     T_TRANSFORM = 276,
     T_CHANCE = 277,
     T_NAMESPACE = 278,
     T_IMPORT = 279,
     T_COMMA = 280,
     T_QUESTIONMARK = 281,
     T_EXCLAMATION = 282,
     T_XOR = 283,
     T_MOD = 284,
     T_PIPE = 285,
     T_DOT = 286,
     T_AMPERSAND = 287,
     T_EQUALS = 288,
     T_NOTEQUALS = 289,
     T_LE = 290,
     T_GE = 291,
     T_SHR = 292,
     T_SHL = 293,
     T_OR = 294,
     T_AND = 295,
     T_DIV = 296,
     T_MINUS = 297,
     T_MUL = 298,
     T_PLUS = 299,
     T_TILDE = 300,
     T_FALSE = 301,
     T_TRUE = 302,
     T_INT = 303,
     T_INT2 = 304,
     T_INT3 = 305,
     T_INT4 = 306,
     T_FLOAT = 307,
     T_VEC2 = 308,
     T_VEC3 = 309,
     T_VEC4 = 310,
     T_MAT22 = 311,
     T_MAT23 = 312,
     T_MAT33 = 313,
     T_MAT34 = 314,
     T_MAT44 = 315,
     T_BOOL = 316
   };
#endif


#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{
/* Line 387 of yacc.c  */
#line 42 "Parser.y"

  int val; 
  float dval;
  char* sym;
  enum RuleKind ruleKind;
  class ImportList* importList;
  class ASTNode* node;
  class Predecessor* predecessor;
  class Successor* successor;
  class	ModuleDef* moddef;
  class	ModuleDefList* moddeflist;
  class Rule* rule;
  class RuleList* ruleList;
  class RuleDefs* ruleDefs;
  class Name* name;
  class QualifiedName* qualifiedName;
  class NameList* names;
  class Namespace* nameSpace;
  class NamespaceList* namespaceList;
  class Module* module;
  class ModuleList* modules;
  class ModuleExp* modexp;
  class Expression* exp;
  class ArgumentSpec* argspec;
  class ArgumentSpecList* argspecs;


/* Line 387 of yacc.c  */
#line 235 "Parser.tab.cpp"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
} YYLTYPE;
# define yyltype YYLTYPE /* obsolescent; will be withdrawn */
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;
extern YYLTYPE yylloc;
#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */

#endif /* !YY_YY_PARSER_TAB_H_INCLUDED  */

/* Copy the second part of user declarations.  */

/* Line 390 of yacc.c  */
#line 276 "Parser.tab.cpp"

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
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
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
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
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

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(N) (N)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int yyi)
#else
static int
YYID (yyi)
    int yyi;
#endif
{
  return yyi;
}
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
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
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
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
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
#   if ! defined malloc && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL \
	     && defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
  YYLTYPE yyls_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE) + sizeof (YYLTYPE)) \
      + 2 * YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)				\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack_alloc, Stack, yysize);			\
	Stack = &yyptr->Stack_alloc;					\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

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
      while (YYID (0))
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  7
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   265

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  62
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  43
/* YYNRULES -- Number of rules.  */
#define YYNRULES  113
/* YYNRULES -- Number of states.  */
#define YYNSTATES  214

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   316

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
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
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     6,    13,    14,    16,    19,    23,    25,
      27,    31,    33,    34,    36,    39,    45,    52,    62,    68,
      70,    73,    82,    93,    95,    97,    99,   100,   103,   105,
     111,   115,   119,   121,   124,   126,   130,   132,   134,   136,
     140,   142,   145,   147,   152,   154,   158,   161,   164,   167,
     170,   173,   176,   178,   182,   187,   189,   193,   195,   201,
     203,   207,   209,   213,   215,   219,   221,   225,   227,   231,
     233,   237,   241,   243,   247,   251,   255,   259,   261,   265,
     269,   271,   275,   279,   281,   285,   289,   293,   295,   300,
     302,   304,   306,   308,   310,   312,   315,   317,   321,   326,
     330,   332,   334,   336,   338,   340,   342,   344,   346,   348,
     350,   354,   361,   370
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      63,     0,    -1,    64,    68,    -1,    64,    68,    17,     8,
      77,     9,    -1,    -1,    65,    -1,    24,    67,    -1,    65,
      24,    67,    -1,     5,    -1,     5,    -1,    67,    31,     5,
      -1,    69,    -1,    -1,    70,    -1,    69,    70,    -1,    23,
      66,     8,    68,     9,    -1,    23,    66,     8,    68,    71,
       9,    -1,    16,     8,    81,     9,    15,     8,    81,     9,
      72,    -1,    16,     8,    81,     9,    72,    -1,    73,    -1,
      72,    73,    -1,    66,    12,    74,    76,    75,     8,    77,
       9,    -1,    66,    12,    74,    76,    75,    22,     4,     8,
      77,     9,    -1,    20,    -1,    19,    -1,    21,    -1,    -1,
      18,    88,    -1,    83,    -1,    82,    13,    83,    14,    82,
      -1,    82,    13,    83,    -1,    83,    14,    82,    -1,    78,
      -1,    77,    78,    -1,    86,    -1,    79,    77,    80,    -1,
      10,    -1,    11,    -1,    66,    -1,    66,    25,    81,    -1,
      83,    -1,    82,    83,    -1,    67,    -1,    67,     6,    84,
       7,    -1,    85,    -1,    84,    25,    85,    -1,    48,    66,
      -1,    52,    66,    -1,    61,    66,    -1,    53,    66,    -1,
      54,    66,    -1,    55,    66,    -1,    67,    -1,    67,     6,
       7,    -1,    67,     6,    87,     7,    -1,    88,    -1,    87,
      25,    88,    -1,    89,    -1,    89,    26,    88,    12,    88,
      -1,    90,    -1,    89,    39,    90,    -1,    91,    -1,    90,
      40,    91,    -1,    92,    -1,    91,    30,    92,    -1,    93,
      -1,    92,    28,    93,    -1,    94,    -1,    93,    32,    94,
      -1,    95,    -1,    94,    33,    95,    -1,    94,    34,    95,
      -1,    96,    -1,    95,    13,    96,    -1,    95,    14,    96,
      -1,    95,    35,    96,    -1,    95,    36,    96,    -1,    97,
      -1,    96,    37,    97,    -1,    96,    38,    97,    -1,    98,
      -1,    97,    44,    98,    -1,    97,    42,    98,    -1,    99,
      -1,    98,    43,    99,    -1,    98,    41,    99,    -1,    98,
      29,    99,    -1,   101,    -1,     6,   100,     7,    99,    -1,
      48,    -1,    52,    -1,    61,    -1,    53,    -1,    54,    -1,
      55,    -1,   103,   104,    -1,   102,    -1,   104,     6,     7,
      -1,   104,     6,    87,     7,    -1,   104,    31,     5,    -1,
     104,    -1,    44,    -1,    42,    -1,    27,    -1,    45,    -1,
       3,    -1,     4,    -1,    47,    -1,    46,    -1,     5,    -1,
       6,    88,     7,    -1,    53,     6,    97,    25,    97,     7,
      -1,    54,     6,    97,    25,    97,    25,    97,     7,    -1,
      55,     6,    97,    25,    97,    25,    97,    25,    97,     7,
      -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   105,   105,   106,   109,   110,   113,   114,   117,   120,
     121,   124,   125,   128,   129,   132,   133,   136,   137,   140,
     141,   144,   145,   148,   149,   150,   153,   154,   157,   158,
     159,   160,   163,   164,   167,   168,   177,   180,   183,   184,
     187,   188,   191,   192,   195,   196,   199,   200,   201,   202,
     203,   204,   207,   208,   209,   212,   213,   216,   217,   226,
     227,   235,   236,   244,   245,   253,   254,   262,   263,   271,
     272,   278,   286,   287,   293,   299,   305,   313,   314,   320,
     328,   329,   335,   343,   344,   350,   356,   364,   365,   374,
     375,   376,   377,   378,   379,   383,   388,   391,   396,   402,
     408,   411,   412,   413,   414,   417,   418,   419,   420,   421,
     422,   423,   429,   436
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "T_NUM", "T_FNUM", "T_NAME", "T_LP",
  "T_RP", "T_LB", "T_RB", "T_LS", "T_RS", "T_COLON", "T_LT", "T_GT",
  "T_CONST", "T_VAR", "T_INITIAL", "T_WHEN", "T_REPLACE", "T_DECOMPOSE",
  "T_TRANSFORM", "T_CHANCE", "T_NAMESPACE", "T_IMPORT", "T_COMMA",
  "T_QUESTIONMARK", "T_EXCLAMATION", "T_XOR", "T_MOD", "T_PIPE", "T_DOT",
  "T_AMPERSAND", "T_EQUALS", "T_NOTEQUALS", "T_LE", "T_GE", "T_SHR",
  "T_SHL", "T_OR", "T_AND", "T_DIV", "T_MINUS", "T_MUL", "T_PLUS",
  "T_TILDE", "T_FALSE", "T_TRUE", "T_INT", "T_INT2", "T_INT3", "T_INT4",
  "T_FLOAT", "T_VEC2", "T_VEC3", "T_VEC4", "T_MAT22", "T_MAT23", "T_MAT33",
  "T_MAT34", "T_MAT44", "T_BOOL", "$accept", "grammar",
  "import_list_optional", "import_list", "name", "qualified_name",
  "namespace_list_optional", "namespace_list", "namespace", "rule_defs",
  "rule_list", "rule", "rule_kind", "condition", "predecessor",
  "successor_list", "successor", "leftsb", "rightsb", "name_list",
  "module_def_list", "module_def", "argument_spec_list", "argument_spec",
  "module_exp", "parameter_list", "expression", "logical_or_exp",
  "logical_and_exp", "bitwise_or_exp", "xor_exp", "bitwise_and_exp",
  "equality_exp", "relational_exp", "shift_exp", "additive_exp",
  "mult_exp", "cast_exp", "type_spec", "unary_exp", "postfix_exp",
  "unary_op", "primary_exp", YY_NULL
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    62,    63,    63,    64,    64,    65,    65,    66,    67,
      67,    68,    68,    69,    69,    70,    70,    71,    71,    72,
      72,    73,    73,    74,    74,    74,    75,    75,    76,    76,
      76,    76,    77,    77,    78,    78,    79,    80,    81,    81,
      82,    82,    83,    83,    84,    84,    85,    85,    85,    85,
      85,    85,    86,    86,    86,    87,    87,    88,    88,    89,
      89,    90,    90,    91,    91,    92,    92,    93,    93,    94,
      94,    94,    95,    95,    95,    95,    95,    96,    96,    96,
      97,    97,    97,    98,    98,    98,    98,    99,    99,   100,
     100,   100,   100,   100,   100,   101,   101,   102,   102,   102,
     102,   103,   103,   103,   103,   104,   104,   104,   104,   104,
     104,   104,   104,   104
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     2,     6,     0,     1,     2,     3,     1,     1,
       3,     1,     0,     1,     2,     5,     6,     9,     5,     1,
       2,     8,    10,     1,     1,     1,     0,     2,     1,     5,
       3,     3,     1,     2,     1,     3,     1,     1,     1,     3,
       1,     2,     1,     4,     1,     3,     2,     2,     2,     2,
       2,     2,     1,     3,     4,     1,     3,     1,     5,     1,
       3,     1,     3,     1,     3,     1,     3,     1,     3,     1,
       3,     3,     1,     3,     3,     3,     3,     1,     3,     3,
       1,     3,     3,     1,     3,     3,     3,     1,     4,     1,
       1,     1,     1,     1,     1,     2,     1,     3,     4,     3,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       3,     6,     8,    10
};

/* YYDEFACT[STATE-NAME] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       4,     0,     0,    12,     5,     9,     6,     1,     0,     2,
      11,    13,     0,     0,     8,     0,     0,    14,     7,    10,
      12,     0,     0,    36,    52,     0,    32,     0,    34,    15,
       0,     0,     0,     3,    33,     0,     0,    16,   105,   106,
     109,     0,    53,   103,   102,   101,   104,   108,   107,     0,
       0,     0,     0,    55,    57,    59,    61,    63,    65,    67,
      69,    72,    77,    80,    83,    87,    96,     0,   100,    37,
      35,    38,     0,    89,    90,    92,    93,    94,    91,     0,
       0,     0,     0,     0,    54,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    95,     0,     0,     0,
       0,   110,     0,     0,     0,     0,    56,     0,    60,    62,
      64,    66,    68,    70,    71,    73,    74,    75,    76,    78,
      79,    82,    81,    86,    85,    84,    97,     0,    99,    39,
       0,     0,    18,    19,    88,     0,     0,     0,     0,    98,
       0,     0,    20,     0,     0,     0,    58,     0,    24,    23,
      25,     0,   111,     0,     0,     0,    42,    26,     0,    28,
       0,     0,    17,     0,     0,     0,     0,    41,     0,   112,
       0,     0,     0,     0,     0,     0,     0,     0,    44,    27,
       0,     0,    30,    31,    40,     0,    46,    47,    49,    50,
      51,    48,    43,     0,     0,     0,     0,   113,    45,    21,
       0,    29,     0,    22
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     2,     3,     4,   141,    24,     9,    10,    11,    31,
     142,   143,   161,   175,   167,    25,    26,    27,    70,    72,
     168,   177,   187,   188,    28,    52,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    80,    65,
      66,    67,    68
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -170
static const yytype_int16 yypact[] =
{
       5,     9,    33,    67,   105,  -170,    65,  -170,   161,   159,
      67,  -170,     9,   209,  -170,   205,   222,  -170,    65,  -170,
      67,   151,   163,  -170,     4,   173,  -170,   151,  -170,  -170,
     223,   207,    72,  -170,  -170,   175,   161,  -170,  -170,  -170,
    -170,    13,  -170,  -170,  -170,  -170,  -170,  -170,  -170,   226,
     227,   228,    29,  -170,    97,   195,   206,   210,   211,    -8,
      17,   150,    69,    57,  -170,  -170,  -170,   165,     7,  -170,
    -170,   214,   231,  -170,  -170,   226,   227,   228,  -170,   230,
     234,   104,   104,   104,  -170,   104,   104,   104,   104,   104,
     104,   104,   104,   104,   104,   104,   104,   104,   104,   104,
     104,   104,   104,   104,   104,   104,  -170,    88,   237,   161,
      82,  -170,   104,   -10,    38,    59,  -170,   232,   195,   206,
     210,   211,    -8,    17,    17,   150,   150,   150,   150,    69,
      69,    57,    57,  -170,  -170,  -170,  -170,    99,  -170,  -170,
     238,   233,   161,  -170,  -170,   104,   104,   104,   104,  -170,
     161,   208,  -170,    -3,    60,    95,  -170,   239,  -170,  -170,
    -170,     9,  -170,   104,   104,   161,    16,   229,   139,   160,
      20,   103,   161,   154,   104,    34,     9,  -170,     9,  -170,
     104,   161,   161,   161,   161,   161,   161,   115,  -170,  -170,
     151,   245,   236,     9,  -170,    39,  -170,  -170,  -170,  -170,
    -170,  -170,  -170,   154,   194,   243,     9,  -170,  -170,  -170,
     151,     9,   212,  -170
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -170,  -170,  -170,  -170,    12,    -1,   235,  -170,   242,  -170,
      89,  -121,  -170,  -170,  -170,   -26,   -23,  -170,  -170,  -106,
    -169,  -133,  -170,    50,  -170,   149,   -36,  -170,   170,   171,
     169,   172,   174,    98,   129,   -75,   100,    51,  -170,  -170,
    -170,  -170,   193
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -41
static const yytype_int16 yytable[] =
{
       6,    35,    34,   139,   162,    79,   113,   114,   115,   193,
      32,    18,    34,   107,     5,   145,    38,    39,    40,    41,
      15,   152,   173,   129,   130,    92,    93,   179,   169,     1,
      94,    95,   100,     7,   101,    13,    84,   211,   108,   100,
      43,   101,   190,   192,   157,   194,   207,    13,    71,   116,
     117,   152,    96,    97,    85,    44,   191,    45,    46,    47,
      48,    73,   100,   146,   101,    74,    75,    76,    77,    79,
     153,   154,   155,   194,    78,    38,    39,    40,    41,    42,
     100,   100,   101,   101,   147,   163,   102,    14,   170,   171,
       8,    38,    39,    40,    41,   136,    13,   140,   103,    43,
     104,   100,   100,   101,   101,   195,   149,    38,    39,    40,
      41,   100,   156,   101,    44,    43,    45,    46,    47,    48,
     164,    71,   202,    86,    85,    49,    50,    51,   180,    12,
      44,    43,    45,    46,    47,    48,    87,   100,   189,   101,
     203,    49,    50,    51,     5,   100,    44,   101,    45,    46,
      47,    48,   176,   133,   134,   135,     5,    49,    50,    51,
     166,    23,    71,   144,   204,   -40,    14,   166,    38,    39,
      40,   105,    29,   -40,   178,   166,    16,   166,     5,    30,
       5,    34,    33,    23,   212,    23,    69,    98,    99,    34,
     123,   124,   166,   196,   197,   198,   199,   200,   201,     5,
     131,   132,   181,   209,    23,   166,   182,   183,   184,   185,
     166,    47,    48,    20,    19,   186,    37,     5,    49,    50,
      51,   213,    23,   125,   126,   127,   128,   158,   159,   160,
      21,    36,    81,    82,    83,    88,    89,   111,    90,   109,
     110,   112,   138,    91,   148,   151,   150,   174,   165,   205,
     206,   210,    17,   208,   172,    22,   137,   118,   120,   119,
     106,     0,   121,     0,     0,   122
};

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-170)))

#define yytable_value_is_error(Yytable_value) \
  YYID (0)

static const yytype_int16 yycheck[] =
{
       1,    27,    25,   109,     7,    41,    81,    82,    83,   178,
       6,    12,    35,     6,     5,    25,     3,     4,     5,     6,
       8,   142,     6,    98,    99,    33,    34,     7,   161,    24,
      13,    14,    42,     0,    44,    31,     7,   206,    31,    42,
      27,    44,     8,   176,   150,   178,     7,    31,    36,    85,
      86,   172,    35,    36,    25,    42,    22,    44,    45,    46,
      47,    48,    42,    25,    44,    52,    53,    54,    55,   105,
     145,   146,   147,   206,    61,     3,     4,     5,     6,     7,
      42,    42,    44,    44,    25,    25,    29,     5,   163,   164,
      23,     3,     4,     5,     6,     7,    31,    15,    41,    27,
      43,    42,    42,    44,    44,   180,     7,     3,     4,     5,
       6,    42,   148,    44,    42,    27,    44,    45,    46,    47,
      25,   109,     7,    26,    25,    53,    54,    55,    25,    24,
      42,    27,    44,    45,    46,    47,    39,    42,   174,    44,
      25,    53,    54,    55,     5,    42,    42,    44,    44,    45,
      46,    47,    13,   102,   103,   104,     5,    53,    54,    55,
     161,    10,   150,   112,   190,     5,     5,   168,     3,     4,
       5,     6,     9,    13,    14,   176,    17,   178,     5,    16,
       5,   204,     9,    10,   210,    10,    11,    37,    38,   212,
      92,    93,   193,   181,   182,   183,   184,   185,   186,     5,
     100,   101,    48,     9,    10,   206,    52,    53,    54,    55,
     211,    46,    47,     8,     5,    61,     9,     5,    53,    54,
      55,     9,    10,    94,    95,    96,    97,    19,    20,    21,
       8,     8,     6,     6,     6,    40,    30,     7,    28,    25,
       9,     7,     5,    32,    12,    12,     8,    18,     9,     4,
      14,     8,    10,   203,   165,    20,   107,    87,    89,    88,
      67,    -1,    90,    -1,    -1,    91
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    24,    63,    64,    65,     5,    67,     0,    23,    68,
      69,    70,    24,    31,     5,    66,    17,    70,    67,     5,
       8,     8,    68,    10,    67,    77,    78,    79,    86,     9,
      16,    71,     6,     9,    78,    77,     8,     9,     3,     4,
       5,     6,     7,    27,    42,    44,    45,    46,    47,    53,
      54,    55,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   101,   102,   103,   104,    11,
      80,    66,    81,    48,    52,    53,    54,    55,    61,    88,
     100,     6,     6,     6,     7,    25,    26,    39,    40,    30,
      28,    32,    33,    34,    13,    14,    35,    36,    37,    38,
      42,    44,    29,    41,    43,     6,   104,     6,    31,    25,
       9,     7,     7,    97,    97,    97,    88,    88,    90,    91,
      92,    93,    94,    95,    95,    96,    96,    96,    96,    97,
      97,    98,    98,    99,    99,    99,     7,    87,     5,    81,
      15,    66,    72,    73,    99,    25,    25,    25,    12,     7,
       8,    12,    73,    97,    97,    97,    88,    81,    19,    20,
      21,    74,     7,    25,    25,     9,    67,    76,    82,    83,
      97,    97,    72,     6,    18,    75,    13,    83,    14,     7,
      25,    48,    52,    53,    54,    55,    61,    84,    85,    88,
       8,    22,    83,    82,    83,    97,    66,    66,    66,    66,
      66,    66,     7,    25,    77,     4,    14,     7,    85,     9,
       8,    82,    77,     9
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  However,
   YYFAIL appears to be in use.  Nevertheless, it is formally deprecated
   in Bison 2.4.2's NEWS entry, where a plan to phase it out is
   discussed.  */

#define YYFAIL		goto yyerrlab
#if defined YYFAIL
  /* This is here to suppress warnings from the GCC cpp's
     -Wunused-macros.  Normally we don't worry about that warning, but
     some users do, and we want to make it easy for users to remove
     YYFAIL uses, which will produce warnings from Bison 2.5.  */
#endif

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
      YYERROR;							\
    }								\
while (YYID (0))

/* Error token number */
#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)                                \
    do                                                                  \
      if (YYID (N))                                                     \
        {                                                               \
          (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;        \
          (Current).first_column = YYRHSLOC (Rhs, 1).first_column;      \
          (Current).last_line    = YYRHSLOC (Rhs, N).last_line;         \
          (Current).last_column  = YYRHSLOC (Rhs, N).last_column;       \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).first_line   = (Current).last_line   =              \
            YYRHSLOC (Rhs, 0).last_line;                                \
          (Current).first_column = (Current).last_column =              \
            YYRHSLOC (Rhs, 0).last_column;                              \
        }                                                               \
    while (YYID (0))
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K])


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef __attribute__
/* This feature is available in gcc versions 2.5 and later.  */
# if (! defined __GNUC__ || __GNUC__ < 2 \
      || (__GNUC__ == 2 && __GNUC_MINOR__ < 5))
#  define __attribute__(Spec) /* empty */
# endif
#endif

#ifndef YY_LOCATION_PRINT
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL

/* Print *YYLOCP on YYO.  Private, do not rely on its existence. */

__attribute__((__unused__))
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static unsigned
yy_location_print_ (FILE *yyo, YYLTYPE const * const yylocp)
#else
static unsigned
yy_location_print_ (yyo, yylocp)
    FILE *yyo;
    YYLTYPE const * const yylocp;
#endif
{
  unsigned res = 0;
  int end_col = 0 != yylocp->last_column ? yylocp->last_column - 1 : 0;
  if (0 <= yylocp->first_line)
    {
      res += fprintf (yyo, "%d", yylocp->first_line);
      if (0 <= yylocp->first_column)
        res += fprintf (yyo, ".%d", yylocp->first_column);
    }
  if (0 <= yylocp->last_line)
    {
      if (yylocp->first_line < yylocp->last_line)
        {
          res += fprintf (yyo, "-%d", yylocp->last_line);
          if (0 <= end_col)
            res += fprintf (yyo, ".%d", end_col);
        }
      else if (0 <= end_col && yylocp->first_column < end_col)
        res += fprintf (yyo, "-%d", end_col);
    }
  return res;
 }

#  define YY_LOCATION_PRINT(File, Loc)          \
  yy_location_print_ (File, &(Loc))

# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */
#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value, Location); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep, yylocationp)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
    YYLTYPE const * const yylocationp;
#endif
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  if (!yyvaluep)
    return;
  YYUSE (yylocationp);
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
        break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep, yylocationp)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
    YYLTYPE const * const yylocationp;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  YY_LOCATION_PRINT (yyoutput, *yylocationp);
  YYFPRINTF (yyoutput, ": ");
  yy_symbol_value_print (yyoutput, yytype, yyvaluep, yylocationp);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
#else
static void
yy_stack_print (yybottom, yytop)
    yytype_int16 *yybottom;
    yytype_int16 *yytop;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, YYLTYPE *yylsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yylsp, yyrule)
    YYSTYPE *yyvsp;
    YYLTYPE *yylsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       , &(yylsp[(yyi + 1) - (yynrhs)])		       );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, yylsp, Rule); \
} while (YYID (0))

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
#ifndef	YYINITDEPTH
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
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
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
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
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
  YYSIZE_T yysize0 = yytnamerr (YY_NULL, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULL;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - Assume YYFAIL is not used.  It's too flawed to consider.  See
       <http://lists.gnu.org/archive/html/bison-patches/2009-12/msg00024.html>
       for details.  YYERROR is fine as it does not invoke this
       function.
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
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULL, yytname[yyx]);
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

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep, YYLTYPE *yylocationp)
#else
static void
yydestruct (yymsg, yytype, yyvaluep, yylocationp)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
    YYLTYPE *yylocationp;
#endif
{
  YYUSE (yyvaluep);
  YYUSE (yylocationp);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
        break;
    }
}




/* The lookahead symbol.  */
int yychar;


#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval YY_INITIAL_VALUE(yyval_default);

/* Location data for the lookahead symbol.  */
YYLTYPE yylloc
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
  = { 1, 1, 1, 1 }
# endif
;


/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       `yyss': related to states.
       `yyvs': related to semantic values.
       `yyls': related to locations.

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

    /* The location stack.  */
    YYLTYPE yylsa[YYINITDEPTH];
    YYLTYPE *yyls;
    YYLTYPE *yylsp;

    /* The locations where the error started and ended.  */
    YYLTYPE yyerror_range[3];

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;
  YYLTYPE yyloc;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N), yylsp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yylsp = yyls = yylsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  yylsp[0] = yylloc;
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
	YYLTYPE *yyls1 = yyls;

	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yyls1, yysize * sizeof (*yylsp),
		    &yystacksize);

	yyls = yyls1;
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
	YYSTACK_RELOCATE (yyls_alloc, yyls);
#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;
      yylsp = yyls + yysize - 1;

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
      yychar = YYLEX;
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
  *++yylsp = yylloc;
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
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];

  /* Default location.  */
  YYLLOC_DEFAULT (yyloc, (yylsp - yylen), yylen);
  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
/* Line 1792 of yacc.c  */
#line 105 "Parser.y"
    { (yyval.node) = mGrammar = new Grammar(Loc(), (yyvsp[(1) - (2)].importList), (yyvsp[(2) - (2)].namespaceList), nullptr); }
    break;

  case 3:
/* Line 1792 of yacc.c  */
#line 106 "Parser.y"
    { (yyval.node) = mGrammar = new Grammar(Loc(), (yyvsp[(1) - (6)].importList), (yyvsp[(2) - (6)].namespaceList), (yyvsp[(5) - (6)].successor)); }
    break;

  case 4:
/* Line 1792 of yacc.c  */
#line 109 "Parser.y"
    { (yyval.importList) = nullptr; }
    break;

  case 6:
/* Line 1792 of yacc.c  */
#line 113 "Parser.y"
    { (yyval.importList) = new ImportList(Loc(), (yyvsp[(2) - (2)].qualifiedName)); }
    break;

  case 7:
/* Line 1792 of yacc.c  */
#line 114 "Parser.y"
    { (yyval.importList) = (yyvsp[(1) - (3)].importList); (yyval.importList)->AddImport((yyvsp[(3) - (3)].qualifiedName)); }
    break;

  case 8:
/* Line 1792 of yacc.c  */
#line 117 "Parser.y"
    { (yyval.name) = new Name(Loc(), (yyvsp[(1) - (1)].sym)); }
    break;

  case 9:
/* Line 1792 of yacc.c  */
#line 120 "Parser.y"
    { (yyval.qualifiedName) = new QualifiedName(Loc(), (yyvsp[(1) - (1)].sym)); }
    break;

  case 10:
/* Line 1792 of yacc.c  */
#line 121 "Parser.y"
    { (yyval.qualifiedName) = (yyvsp[(1) - (3)].qualifiedName); (yyval.qualifiedName)->AddName((yyvsp[(3) - (3)].sym)); }
    break;

  case 12:
/* Line 1792 of yacc.c  */
#line 125 "Parser.y"
    { (yyval.namespaceList) = nullptr; }
    break;

  case 13:
/* Line 1792 of yacc.c  */
#line 128 "Parser.y"
    { (yyval.namespaceList) = new NamespaceList(Loc(), (yyvsp[(1) - (1)].nameSpace)); }
    break;

  case 14:
/* Line 1792 of yacc.c  */
#line 129 "Parser.y"
    { (yyval.namespaceList) = (yyvsp[(1) - (2)].namespaceList); (yyval.namespaceList)->AddNamespace((yyvsp[(2) - (2)].nameSpace)); }
    break;

  case 15:
/* Line 1792 of yacc.c  */
#line 132 "Parser.y"
    { (yyval.nameSpace) = new Namespace(Loc(), (yyvsp[(2) - (5)].name), (yyvsp[(4) - (5)].namespaceList), nullptr); }
    break;

  case 16:
/* Line 1792 of yacc.c  */
#line 133 "Parser.y"
    { (yyval.nameSpace) = new Namespace(Loc(), (yyvsp[(2) - (6)].name), (yyvsp[(4) - (6)].namespaceList), (yyvsp[(5) - (6)].ruleDefs)); }
    break;

  case 17:
/* Line 1792 of yacc.c  */
#line 136 "Parser.y"
    { (yyval.ruleDefs) = new RuleDefs(Loc(), (yyvsp[(3) - (9)].names), (yyvsp[(7) - (9)].names), (yyvsp[(9) - (9)].ruleList)); }
    break;

  case 18:
/* Line 1792 of yacc.c  */
#line 137 "Parser.y"
    { (yyval.ruleDefs) = new RuleDefs(Loc(), (yyvsp[(3) - (5)].names), nullptr, (yyvsp[(5) - (5)].ruleList)); }
    break;

  case 19:
/* Line 1792 of yacc.c  */
#line 140 "Parser.y"
    { (yyval.ruleList) = new RuleList((yyvsp[(1) - (1)].rule)->GetLocation()); (yyval.ruleList)->AddRule((yyvsp[(1) - (1)].rule)); }
    break;

  case 20:
/* Line 1792 of yacc.c  */
#line 141 "Parser.y"
    { (yyval.ruleList) = (yyvsp[(1) - (2)].ruleList); (yyval.ruleList)->AddRule((yyvsp[(2) - (2)].rule)); }
    break;

  case 21:
/* Line 1792 of yacc.c  */
#line 144 "Parser.y"
    { (yyval.rule) = new Rule((yyvsp[(3) - (8)].ruleKind), (yyvsp[(1) - (8)].name), -1.0f, (yyvsp[(4) - (8)].predecessor), (yyvsp[(5) - (8)].exp), (yyvsp[(7) - (8)].successor)); }
    break;

  case 22:
/* Line 1792 of yacc.c  */
#line 145 "Parser.y"
    { (yyval.rule) = new Rule((yyvsp[(3) - (10)].ruleKind), (yyvsp[(1) - (10)].name), (yyvsp[(7) - (10)].dval), (yyvsp[(4) - (10)].predecessor), (yyvsp[(5) - (10)].exp), (yyvsp[(9) - (10)].successor)); }
    break;

  case 23:
/* Line 1792 of yacc.c  */
#line 148 "Parser.y"
    { (yyval.ruleKind) = Decompose; }
    break;

  case 24:
/* Line 1792 of yacc.c  */
#line 149 "Parser.y"
    { (yyval.ruleKind) = Replace; }
    break;

  case 25:
/* Line 1792 of yacc.c  */
#line 150 "Parser.y"
    { (yyval.ruleKind) = Transform; }
    break;

  case 26:
/* Line 1792 of yacc.c  */
#line 153 "Parser.y"
    { (yyval.exp) = NULL; }
    break;

  case 27:
/* Line 1792 of yacc.c  */
#line 154 "Parser.y"
    { (yyval.exp) = (yyvsp[(2) - (2)].exp); }
    break;

  case 28:
/* Line 1792 of yacc.c  */
#line 157 "Parser.y"
    { (yyval.predecessor) = new Predecessor(NULL, (yyvsp[(1) - (1)].moddef), NULL); }
    break;

  case 29:
/* Line 1792 of yacc.c  */
#line 158 "Parser.y"
    { (yyval.predecessor) = new Predecessor((yyvsp[(1) - (5)].moddeflist), (yyvsp[(3) - (5)].moddef), (yyvsp[(5) - (5)].moddeflist)); }
    break;

  case 30:
/* Line 1792 of yacc.c  */
#line 159 "Parser.y"
    { (yyval.predecessor) = new Predecessor((yyvsp[(1) - (3)].moddeflist), (yyvsp[(3) - (3)].moddef), NULL); }
    break;

  case 31:
/* Line 1792 of yacc.c  */
#line 160 "Parser.y"
    { (yyval.predecessor) = new Predecessor(NULL, (yyvsp[(1) - (3)].moddef), (yyvsp[(3) - (3)].moddeflist)); }
    break;

  case 32:
/* Line 1792 of yacc.c  */
#line 163 "Parser.y"
    { (yyval.successor) = (yyvsp[(1) - (1)].successor); }
    break;

  case 33:
/* Line 1792 of yacc.c  */
#line 164 "Parser.y"
    { (yyval.successor) = (yyvsp[(1) - (2)].successor); (yyval.successor)->MergeSuccessor((yyvsp[(2) - (2)].successor)); }
    break;

  case 34:
/* Line 1792 of yacc.c  */
#line 167 "Parser.y"
    { (yyval.successor) = new Successor((yyvsp[(1) - (1)].modexp)->GetLocation()); (yyval.successor)->AddModuleExp((yyvsp[(1) - (1)].modexp)); }
    break;

  case 35:
/* Line 1792 of yacc.c  */
#line 169 "Parser.y"
    { 
		(yyval.successor) = new Successor((yyvsp[(2) - (3)].successor)->GetLocation()); 
		(yyval.successor)->AddModuleExp(new ModuleExp((yyvsp[(1) - (3)].qualifiedName), NULL)); 
		(yyval.successor)->MergeSuccessor((yyvsp[(2) - (3)].successor)); 
		(yyval.successor)->AddModuleExp(new ModuleExp((yyvsp[(3) - (3)].qualifiedName), NULL)); 
	}
    break;

  case 36:
/* Line 1792 of yacc.c  */
#line 177 "Parser.y"
    { (yyval.qualifiedName) = new QualifiedName(Loc(), (yyvsp[(1) - (1)].sym)); }
    break;

  case 37:
/* Line 1792 of yacc.c  */
#line 180 "Parser.y"
    { (yyval.qualifiedName) = new QualifiedName(Loc(), (yyvsp[(1) - (1)].sym)); }
    break;

  case 38:
/* Line 1792 of yacc.c  */
#line 183 "Parser.y"
    { (yyval.names) = new NameList((yyvsp[(1) - (1)].name)->GetLocation()); (yyval.names)->AddName((yyvsp[(1) - (1)].name)); }
    break;

  case 39:
/* Line 1792 of yacc.c  */
#line 184 "Parser.y"
    { (yyval.names) = (yyvsp[(3) - (3)].names); (yyval.names)->AddName((yyvsp[(1) - (3)].name)); }
    break;

  case 40:
/* Line 1792 of yacc.c  */
#line 187 "Parser.y"
    { (yyval.moddeflist) = new ModuleDefList(Loc()); (yyval.moddeflist)->AddModuleDef((yyvsp[(1) - (1)].moddef)); }
    break;

  case 41:
/* Line 1792 of yacc.c  */
#line 188 "Parser.y"
    { (yyval.moddeflist) = (yyvsp[(1) - (2)].moddeflist); (yyvsp[(1) - (2)].moddeflist)->AddModuleDef((yyvsp[(2) - (2)].moddef)); }
    break;

  case 42:
/* Line 1792 of yacc.c  */
#line 191 "Parser.y"
    { (yyval.moddef) = new ModuleDef((yyvsp[(1) - (1)].qualifiedName), NULL); }
    break;

  case 43:
/* Line 1792 of yacc.c  */
#line 192 "Parser.y"
    { (yyval.moddef) = new ModuleDef((yyvsp[(1) - (4)].qualifiedName), (yyvsp[(3) - (4)].argspecs)); }
    break;

  case 44:
/* Line 1792 of yacc.c  */
#line 195 "Parser.y"
    { (yyval.argspecs) = new ArgumentSpecList(); (yyval.argspecs)->AddArgumentSpec((yyvsp[(1) - (1)].argspec)); }
    break;

  case 45:
/* Line 1792 of yacc.c  */
#line 196 "Parser.y"
    { (yyval.argspecs) = (yyvsp[(1) - (3)].argspecs); (yyval.argspecs)->AddArgumentSpec((yyvsp[(3) - (3)].argspec)); }
    break;

  case 46:
/* Line 1792 of yacc.c  */
#line 199 "Parser.y"
    { (yyval.argspec) = new ArgumentSpec((yyvsp[(2) - (2)].name),  LSystem::ArgumentType::Int); }
    break;

  case 47:
/* Line 1792 of yacc.c  */
#line 200 "Parser.y"
    { (yyval.argspec) = new ArgumentSpec((yyvsp[(2) - (2)].name),  LSystem::ArgumentType::Float); }
    break;

  case 48:
/* Line 1792 of yacc.c  */
#line 201 "Parser.y"
    { (yyval.argspec) = new ArgumentSpec((yyvsp[(2) - (2)].name),  LSystem::ArgumentType::Bool); }
    break;

  case 49:
/* Line 1792 of yacc.c  */
#line 202 "Parser.y"
    { (yyval.argspec) = new ArgumentSpec((yyvsp[(2) - (2)].name),  LSystem::ArgumentType::Vec2); }
    break;

  case 50:
/* Line 1792 of yacc.c  */
#line 203 "Parser.y"
    { (yyval.argspec) = new ArgumentSpec((yyvsp[(2) - (2)].name),  LSystem::ArgumentType::Vec3); }
    break;

  case 51:
/* Line 1792 of yacc.c  */
#line 204 "Parser.y"
    { (yyval.argspec) = new ArgumentSpec((yyvsp[(2) - (2)].name),  LSystem::ArgumentType::Vec4); }
    break;

  case 52:
/* Line 1792 of yacc.c  */
#line 207 "Parser.y"
    { (yyval.modexp) = new ModuleExp((yyvsp[(1) - (1)].qualifiedName), NULL); }
    break;

  case 53:
/* Line 1792 of yacc.c  */
#line 208 "Parser.y"
    { (yyval.modexp) = new ModuleExp((yyvsp[(1) - (3)].qualifiedName), NULL); }
    break;

  case 54:
/* Line 1792 of yacc.c  */
#line 209 "Parser.y"
    { (yyval.modexp) = new ModuleExp((yyvsp[(1) - (4)].qualifiedName), (yyvsp[(3) - (4)].exp)); }
    break;

  case 55:
/* Line 1792 of yacc.c  */
#line 212 "Parser.y"
    { (yyval.exp) = new Expression(Expression::ExpressionList, (yyvsp[(1) - (1)].exp)->GetLocation()); (yyval.exp)->AddOperand((yyvsp[(1) - (1)].exp)); }
    break;

  case 56:
/* Line 1792 of yacc.c  */
#line 213 "Parser.y"
    { (yyval.exp) = (yyvsp[(1) - (3)].exp); (yyvsp[(1) - (3)].exp)->AddOperand((yyvsp[(3) - (3)].exp)); }
    break;

  case 58:
/* Line 1792 of yacc.c  */
#line 218 "Parser.y"
    {
		(yyval.exp) = new Expression(Expression::Conditional, Loc()); 
		(yyval.exp)->AddOperand((yyvsp[(1) - (5)].exp));
		(yyval.exp)->AddOperand((yyvsp[(3) - (5)].exp));
		(yyval.exp)->AddOperand((yyvsp[(5) - (5)].exp));
	}
    break;

  case 60:
/* Line 1792 of yacc.c  */
#line 228 "Parser.y"
    { 
		(yyval.exp) = new Expression(Expression::Or, Loc());
		(yyval.exp)->AddOperand((yyvsp[(1) - (3)].exp));
		(yyval.exp)->AddOperand((yyvsp[(3) - (3)].exp)); 
	}
    break;

  case 62:
/* Line 1792 of yacc.c  */
#line 237 "Parser.y"
    {
		(yyval.exp) = new Expression(Expression::And, Loc());
		(yyval.exp)->AddOperand((yyvsp[(1) - (3)].exp));
		(yyval.exp)->AddOperand((yyvsp[(3) - (3)].exp));
	}
    break;

  case 64:
/* Line 1792 of yacc.c  */
#line 246 "Parser.y"
    { 
		(yyval.exp) = new Expression(Expression::BitwiseOr, Loc());
		(yyval.exp)->AddOperand((yyvsp[(1) - (3)].exp));
		(yyval.exp)->AddOperand((yyvsp[(3) - (3)].exp));
	}
    break;

  case 66:
/* Line 1792 of yacc.c  */
#line 255 "Parser.y"
    {
		(yyval.exp) = new Expression(Expression::XOr, Loc());
		(yyval.exp)->AddOperand((yyvsp[(1) - (3)].exp));
		(yyval.exp)->AddOperand((yyvsp[(3) - (3)].exp));
	}
    break;

  case 68:
/* Line 1792 of yacc.c  */
#line 264 "Parser.y"
    { 
		(yyval.exp) = new Expression(Expression::BitwiseAnd, Loc());
		(yyval.exp)->AddOperand((yyvsp[(1) - (3)].exp));
		(yyval.exp)->AddOperand((yyvsp[(3) - (3)].exp));
	}
    break;

  case 70:
/* Line 1792 of yacc.c  */
#line 273 "Parser.y"
    {
		(yyval.exp) = new Expression(Expression::Equals, Loc());
		(yyval.exp)->AddOperand((yyvsp[(1) - (3)].exp));
		(yyval.exp)->AddOperand((yyvsp[(3) - (3)].exp));
	}
    break;

  case 71:
/* Line 1792 of yacc.c  */
#line 279 "Parser.y"
    {
		(yyval.exp) = new Expression(Expression::NotEquals, Loc());
		(yyval.exp)->AddOperand((yyvsp[(1) - (3)].exp));
		(yyval.exp)->AddOperand((yyvsp[(3) - (3)].exp));
	}
    break;

  case 73:
/* Line 1792 of yacc.c  */
#line 288 "Parser.y"
    {
		(yyval.exp) = new Expression(Expression::LessThan, Loc());
		(yyval.exp)->AddOperand((yyvsp[(1) - (3)].exp));
		(yyval.exp)->AddOperand((yyvsp[(3) - (3)].exp));
	}
    break;

  case 74:
/* Line 1792 of yacc.c  */
#line 294 "Parser.y"
    {
		(yyval.exp) = new Expression(Expression::GreaterThan, Loc());
		(yyval.exp)->AddOperand((yyvsp[(1) - (3)].exp));
		(yyval.exp)->AddOperand((yyvsp[(3) - (3)].exp));
	}
    break;

  case 75:
/* Line 1792 of yacc.c  */
#line 300 "Parser.y"
    {
		(yyval.exp) = new Expression(Expression::LessOrEqual, Loc());
		(yyval.exp)->AddOperand((yyvsp[(1) - (3)].exp));
		(yyval.exp)->AddOperand((yyvsp[(3) - (3)].exp));
	}
    break;

  case 76:
/* Line 1792 of yacc.c  */
#line 306 "Parser.y"
    {
		(yyval.exp) = new Expression(Expression::GreaterOrEqual, Loc());
		(yyval.exp)->AddOperand((yyvsp[(1) - (3)].exp));
		(yyval.exp)->AddOperand((yyvsp[(3) - (3)].exp));
	}
    break;

  case 78:
/* Line 1792 of yacc.c  */
#line 315 "Parser.y"
    {
		(yyval.exp) = new Expression(Expression::ShiftRight, Loc());
		(yyval.exp)->AddOperand((yyvsp[(1) - (3)].exp));
		(yyval.exp)->AddOperand((yyvsp[(3) - (3)].exp));
	}
    break;

  case 79:
/* Line 1792 of yacc.c  */
#line 321 "Parser.y"
    {
		(yyval.exp) = new Expression(Expression::ShiftLeft, Loc());
		(yyval.exp)->AddOperand((yyvsp[(1) - (3)].exp));
		(yyval.exp)->AddOperand((yyvsp[(3) - (3)].exp));
	}
    break;

  case 81:
/* Line 1792 of yacc.c  */
#line 330 "Parser.y"
    {
		(yyval.exp) = new Expression(Expression::Add, Loc());
		(yyval.exp)->AddOperand((yyvsp[(1) - (3)].exp));
		(yyval.exp)->AddOperand((yyvsp[(3) - (3)].exp));
	}
    break;

  case 82:
/* Line 1792 of yacc.c  */
#line 336 "Parser.y"
    {
		(yyval.exp) = new Expression(Expression::Subtract, Loc());
		(yyval.exp)->AddOperand((yyvsp[(1) - (3)].exp));
		(yyval.exp)->AddOperand((yyvsp[(3) - (3)].exp));
	}
    break;

  case 84:
/* Line 1792 of yacc.c  */
#line 345 "Parser.y"
    {
		(yyval.exp) = new Expression(Expression::Multiply, Loc());
		(yyval.exp)->AddOperand((yyvsp[(1) - (3)].exp));
		(yyval.exp)->AddOperand((yyvsp[(3) - (3)].exp));
	}
    break;

  case 85:
/* Line 1792 of yacc.c  */
#line 351 "Parser.y"
    {
		(yyval.exp) = new Expression(Expression::Divide, Loc());
		(yyval.exp)->AddOperand((yyvsp[(1) - (3)].exp));
		(yyval.exp)->AddOperand((yyvsp[(3) - (3)].exp));
	}
    break;

  case 86:
/* Line 1792 of yacc.c  */
#line 357 "Parser.y"
    {
		(yyval.exp) = new Expression(Expression::Remainder, Loc());
		(yyval.exp)->AddOperand((yyvsp[(1) - (3)].exp));
		(yyval.exp)->AddOperand((yyvsp[(3) - (3)].exp));
	}
    break;

  case 87:
/* Line 1792 of yacc.c  */
#line 364 "Parser.y"
    { (yyval.exp) = (yyvsp[(1) - (1)].exp); }
    break;

  case 88:
/* Line 1792 of yacc.c  */
#line 366 "Parser.y"
    {
		(yyval.exp) = new Expression(Expression::TypeCast, Loc());
		(yyval.exp)->AddOperand((yyvsp[(2) - (4)].exp));
		(yyval.exp)->AddOperand((yyvsp[(4) - (4)].exp));
	}
    break;

  case 89:
/* Line 1792 of yacc.c  */
#line 374 "Parser.y"
    { (yyval.exp) = new Expression(Expression::IntType, Loc()); }
    break;

  case 90:
/* Line 1792 of yacc.c  */
#line 375 "Parser.y"
    { (yyval.exp) = new Expression(Expression::FloatType, Loc()); }
    break;

  case 91:
/* Line 1792 of yacc.c  */
#line 376 "Parser.y"
    { (yyval.exp) = new Expression(Expression::BoolType, Loc()); }
    break;

  case 92:
/* Line 1792 of yacc.c  */
#line 377 "Parser.y"
    { (yyval.exp) = new Expression(Expression::Vec2Type, Loc()); }
    break;

  case 93:
/* Line 1792 of yacc.c  */
#line 378 "Parser.y"
    { (yyval.exp) = new Expression(Expression::Vec3Type, Loc()); }
    break;

  case 94:
/* Line 1792 of yacc.c  */
#line 379 "Parser.y"
    { (yyval.exp) = new Expression(Expression::Vec4Type, Loc()); }
    break;

  case 95:
/* Line 1792 of yacc.c  */
#line 384 "Parser.y"
    {
		(yyval.exp) = new Expression((Expression::Type) (yyvsp[(1) - (2)].val), Loc()); 
		(yyval.exp)->AddOperand((yyvsp[(2) - (2)].exp));
	}
    break;

  case 97:
/* Line 1792 of yacc.c  */
#line 392 "Parser.y"
    {
		(yyval.exp) = new Expression(Expression::FuncCall, Loc()); 
		(yyval.exp)->AddOperand((yyvsp[(1) - (3)].exp));
	}
    break;

  case 98:
/* Line 1792 of yacc.c  */
#line 397 "Parser.y"
    { 
		(yyval.exp) = new Expression(Expression::FuncCall, Loc());
		(yyval.exp)->AddOperand((yyvsp[(1) - (4)].exp));
		(yyval.exp)->AddOperand((yyvsp[(3) - (4)].exp));
	}
    break;

  case 99:
/* Line 1792 of yacc.c  */
#line 403 "Parser.y"
    { 
		(yyval.exp) = new Expression(Expression::MemberSelect, Loc());
		(yyval.exp)->AddOperand((yyvsp[(1) - (3)].exp));
		(yyval.exp)->AddOperand(new Expression(Loc(), (yyvsp[(3) - (3)].sym)));
	}
    break;

  case 101:
/* Line 1792 of yacc.c  */
#line 411 "Parser.y"
    { (yyval.val) = Expression::UnaryPlus; }
    break;

  case 102:
/* Line 1792 of yacc.c  */
#line 412 "Parser.y"
    { (yyval.val) = Expression::UnaryMinus; }
    break;

  case 103:
/* Line 1792 of yacc.c  */
#line 413 "Parser.y"
    { (yyval.val) = Expression::Not; }
    break;

  case 104:
/* Line 1792 of yacc.c  */
#line 414 "Parser.y"
    { (yyval.val) = Expression::Negate; }
    break;

  case 105:
/* Line 1792 of yacc.c  */
#line 417 "Parser.y"
    { (yyval.exp) = new Expression(Loc(), (yyvsp[(1) - (1)].val)); }
    break;

  case 106:
/* Line 1792 of yacc.c  */
#line 418 "Parser.y"
    { (yyval.exp) = new Expression(Loc(), (yyvsp[(1) - (1)].dval)); }
    break;

  case 107:
/* Line 1792 of yacc.c  */
#line 419 "Parser.y"
    { (yyval.exp) = new Expression(Loc(), (yyvsp[(1) - (1)].sym)); }
    break;

  case 108:
/* Line 1792 of yacc.c  */
#line 420 "Parser.y"
    { (yyval.exp) = new Expression(Loc(), (yyvsp[(1) - (1)].sym)); }
    break;

  case 109:
/* Line 1792 of yacc.c  */
#line 421 "Parser.y"
    { (yyval.exp) = new Expression(Loc(), (yyvsp[(1) - (1)].sym)); }
    break;

  case 110:
/* Line 1792 of yacc.c  */
#line 422 "Parser.y"
    { (yyval.exp) = (yyvsp[(2) - (3)].exp); }
    break;

  case 111:
/* Line 1792 of yacc.c  */
#line 424 "Parser.y"
    { 
		(yyval.exp) = new Expression(Expression::Vec2, Loc());
		(yyval.exp)->AddOperand((yyvsp[(3) - (6)].exp));
		(yyval.exp)->AddOperand((yyvsp[(5) - (6)].exp));
	}
    break;

  case 112:
/* Line 1792 of yacc.c  */
#line 430 "Parser.y"
    { 
		(yyval.exp) = new Expression(Expression::Vec3, Loc());
		(yyval.exp)->AddOperand((yyvsp[(3) - (8)].exp));
		(yyval.exp)->AddOperand((yyvsp[(5) - (8)].exp));
		(yyval.exp)->AddOperand((yyvsp[(7) - (8)].exp));
	}
    break;

  case 113:
/* Line 1792 of yacc.c  */
#line 437 "Parser.y"
    { 
		(yyval.exp) = new Expression(Expression::Vec4, Loc());
		(yyval.exp)->AddOperand((yyvsp[(3) - (10)].exp));
		(yyval.exp)->AddOperand((yyvsp[(5) - (10)].exp));
		(yyval.exp)->AddOperand((yyvsp[(7) - (10)].exp));
		(yyval.exp)->AddOperand((yyvsp[(9) - (10)].exp));
	}
    break;


/* Line 1792 of yacc.c  */
#line 2500 "Parser.tab.cpp"
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
  *++yylsp = yyloc;

  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
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

  yyerror_range[1] = yylloc;

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
		      yytoken, &yylval, &yylloc);
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

  yyerror_range[1] = yylsp[1-yylen];
  /* Do not reclaim the symbols of the rule which action triggered
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
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

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

      yyerror_range[1] = *yylsp;
      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp, yylsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  yyerror_range[2] = yylloc;
  /* Using YYLLOC is tempting, but would change the location of
     the lookahead.  YYLOC is available though.  */
  YYLLOC_DEFAULT (yyloc, yyerror_range, 2);
  *++yylsp = yyloc;

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
                  yytoken, &yylval, &yylloc);
    }
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp, yylsp);
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
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}


/* Line 2055 of yacc.c  */
#line 445 "Parser.y"


//-- FUNCTION DEFINITIONS ---------------------------------

#define YYPRINT(file, type, value)   yyprint (file, type, value)

void yyerror(const char *s, ...)
{
  va_list ap;
  va_start(ap, s);

  if(yylloc.first_line)
	fprintf(stderr, "%d.%d-%d.%d: error: ", yylloc.first_line, yylloc.first_column,
		yylloc.last_line, yylloc.last_column);
  vfprintf(stderr, s, ap);
  fprintf(stderr, "\n");
}

void lyyerror(YYLTYPE t, char *s, ...)
{
  va_list ap;
  va_start(ap, s);

  if(t.first_line)
	fprintf(stderr, "%d.%d-%d.%d: error: ", t.first_line, t.first_column,
		t.last_line, t.last_column);
  vfprintf(stderr, s, ap);
  fprintf(stderr, "\n");
}

 LSystem::Location Loc() 
{
	return  LSystem::Location(fileName, yylloc.first_line, yylloc.first_column, yylloc.last_line, yylloc.last_column);
}
