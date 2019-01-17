%{

#include <iostream>

#include "Parser.tab.h"
#include "AST.h"
#include "Location.h"

using namespace std;

/* handle locations */
int yycolumn = 1;
char * fileName = "stdin";

LSystem::Location Loc();

#define YY_USER_ACTION yylloc.first_line = yylloc.last_line = yylineno; \
    yylloc.first_column = yycolumn; yylloc.last_column = yycolumn + yyleng; \
    yycolumn += yyleng;

%}

%option noyywrap
%option yylineno

%%

"//"[^\r\n]*			{ /* comment */ }
"initial"				{ return T_INITIAL; }
"const"					{ return T_CONST; }
"namespace"				{ return T_NAMESPACE; }
"chance"				{ return T_CHANCE; }
"var"					{ return T_VAR; }
"replace"				{ return T_REPLACE; }
"decompose"				{ return T_DECOMPOSE; }
"transform"				{ return T_TRANSFORM; }
"when"					{ return T_WHEN; }
"int"					{ return T_INT; }
"float"					{ return T_FLOAT; }
"vec2"					{ return T_VEC2; }
"vec3"					{ return T_VEC3; }
"vec4"					{ return T_VEC4; }
"mat22"					{ return T_MAT22; }
"mat23"					{ return T_MAT23; }
"mat33"					{ return T_MAT33; }
"mat34"					{ return T_MAT34; }
"mat44"					{ return T_MAT44; }
"bool"					{ return T_BOOL; }
"true"					{ return T_TRUE; }
"false"					{ return T_FALSE; }
[a-zA-Z_][a-zA-Z_0-9]*	{ yylval.sym = yytext; return T_NAME; }
[0-9]+\.[0-9]+			{ yylval.dval = (float) atof(yytext); return T_FNUM; }
[0-9]+					{ yylval.val = atoi(yytext); return T_NUM; }
"||"					{ return T_OR; }
"&&"					{ return T_AND; }
"=="					{ return T_EQUALS; }
"!="					{ return T_NOTEQUALS; }
"<="					{ return T_LE; }
">="					{ return T_GE; }
">>"					{ return T_SHR; }
"<<"					{ return T_SHL; }
"&"						{ return T_AMPERSAND; }
"^"						{ return T_XOR; }
"%"						{ return T_MOD; }
"*"						{ return T_MUL; }
"|"						{ return T_PIPE; }
"/"						{ return T_DIV; }
":"						{ return T_COLON; }
"("						{ return T_LP; }
")"						{ return T_RP; }
"{"						{ return T_LB; }
"}"						{ return T_RB; }
\[						{ yylval.sym = yytext; return T_LS; }
\]						{ yylval.sym = yytext; return T_RS; }
"<"						{ return T_LT; }
">"						{ return T_GT; }
","						{ return T_COMMA; }
"?"						{ return T_QUESTIONMARK; }
"!"						{ return T_EXCLAMATION; }
"+"						{ return T_PLUS; }
"-"						{ return T_MINUS; }
"~"						{ return T_TILDE; }
"."						{ return T_DOT; }
[ \t]+					{ }
\n						{ yycolumn = 1; }
\r                      {}
.						{ cerr << Loc().ToString() << ": unrecognized token: '" << yytext << "'" << endl; }
<<EOF>>					{ return 0; }

%%
