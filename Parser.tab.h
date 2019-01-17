/* A Bison parser, made by GNU Bison 2.7.  */

/* Bison interface for Yacc-like parsers in C
   
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
/* Line 2058 of yacc.c  */
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


/* Line 2058 of yacc.c  */
#line 146 "Parser.tab.h"
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
