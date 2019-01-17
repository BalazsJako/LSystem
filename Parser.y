/********************************************************
 * Parser
 ********************************************************/
%{
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

%}

%locations

//-- SYMBOL SEMANTIC VALUES -----------------------------
%union {
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
};

%token <val> T_NUM
%token <dval> T_FNUM
%token <sym> T_NAME T_LP T_RP T_LB T_RB T_LS T_RS T_COLON T_LT T_GT
%token <sym> T_CONST T_VAR T_INITIAL T_WHEN T_REPLACE T_DECOMPOSE T_TRANSFORM T_CHANCE T_NAMESPACE T_IMPORT
%token <sym> T_COMMA T_QUESTIONMARK T_EXCLAMATION T_XOR T_MOD T_PIPE T_DOT
%token <sym> T_AMPERSAND T_EQUALS T_NOTEQUALS T_LE T_GE T_SHR T_SHL T_OR T_AND T_DIV T_MINUS T_MUL T_PLUS T_TILDE
%token <sym> T_FALSE T_TRUE T_INT T_INT2 T_INT3 T_INT4 T_FLOAT T_VEC2 T_VEC3 T_VEC4 T_MAT22 T_MAT23 T_MAT33 T_MAT34 T_MAT44 T_BOOL

%type  <node> grammar
%type  <name> name 
%type  <qualifiedName> qualified_name leftsb rightsb
%type  <names> name_list
%type  <importList> import_list import_list_optional;
%type  <namespaceList> namespace_list namespace_list_optional;
%type  <nameSpace> namespace;
%type  <rule> rule
%type  <ruleList> rule_list
%type  <ruleDefs> rule_defs
%type  <ruleKind> rule_kind
%type  <predecessor> predecessor
%type  <successor> successor successor_list
%type  <moddef> module_def
%type  <moddeflist> module_def_list
%type  <modexp> module_exp
%type  <argspec> argument_spec
%type  <argspecs> argument_spec_list
%type  <exp> expression parameter_list logical_or_exp logical_and_exp bitwise_or_exp 
%type  <exp> bitwise_and_exp xor_exp equality_exp relational_exp shift_exp additive_exp mult_exp 
%type  <exp> unary_exp cast_exp primary_exp postfix_exp
%type  <exp> condition type_spec
%type  <val> unary_op

//-- GRAMMAR RUT_LES ---------------------------------------
%%

grammar
	: import_list_optional namespace_list_optional { $$ = mGrammar = new Grammar(Loc(), $1, $2, nullptr); }
	| import_list_optional namespace_list_optional T_INITIAL T_LB successor_list T_RB { $$ = mGrammar = new Grammar(Loc(), $1, $2, $5); }	

import_list_optional
	: /* empty */ { $$ = nullptr; }
	| import_list

import_list
	: T_IMPORT qualified_name { $$ = new ImportList(Loc(), $2); }
	| import_list T_IMPORT qualified_name { $$ = $1; $$->AddImport($3); }

name
	: T_NAME { $$ = new Name(Loc(), $1); }

qualified_name
	: T_NAME { $$ = new QualifiedName(Loc(), $1); }
	| qualified_name T_DOT T_NAME { $$ = $1; $$->AddName($3); }

namespace_list_optional
	: namespace_list
	| /* empty */ { $$ = nullptr; }

namespace_list
	: namespace { $$ = new NamespaceList(Loc(), $1); }
	| namespace_list namespace { $$ = $1; $$->AddNamespace($2); }

namespace
	: T_NAMESPACE name T_LB namespace_list_optional T_RB { $$ = new Namespace(Loc(), $2, $4, nullptr); }
	| T_NAMESPACE name T_LB namespace_list_optional rule_defs T_RB { $$ = new Namespace(Loc(), $2, $4, $5); }

rule_defs
	: T_VAR T_LB name_list T_RB T_CONST T_LB name_list T_RB rule_list { $$ = new RuleDefs(Loc(), $3, $7, $9); }
	| T_VAR T_LB name_list T_RB rule_list { $$ = new RuleDefs(Loc(), $3, nullptr, $5); }

rule_list 
	: rule { $$ = new RuleList($1->GetLocation()); $$->AddRule($1); }
	| rule_list rule { $$ = $1; $$->AddRule($2); }

rule
	: name T_COLON rule_kind predecessor condition T_LB successor_list T_RB { $$ = new Rule($3, $1, -1.0f, $4, $5, $7); }
	| name T_COLON rule_kind predecessor condition T_CHANCE T_FNUM T_LB successor_list T_RB { $$ = new Rule($3, $1, $7, $4, $5, $9); }

rule_kind
	: T_DECOMPOSE { $$ = Decompose; }
	| T_REPLACE { $$ = Replace; }
	| T_TRANSFORM { $$ = Transform; }

condition
	: /* empty */	{ $$ = NULL; }
	| T_WHEN expression { $$ = $2; }

predecessor
	: module_def { $$ = new Predecessor(NULL, $1, NULL); }
	| module_def_list T_LT module_def T_GT module_def_list { $$ = new Predecessor($1, $3, $5); }
	| module_def_list T_LT module_def { $$ = new Predecessor($1, $3, NULL); }
	| module_def T_GT module_def_list { $$ = new Predecessor(NULL, $1, $3); }

successor_list
	: successor { $$ = $1; }
	| successor_list successor { $$ = $1; $$->MergeSuccessor($2); }

successor
	: module_exp { $$ = new Successor($1->GetLocation()); $$->AddModuleExp($1); }
	| leftsb successor_list rightsb 
	{ 
		$$ = new Successor($2->GetLocation()); 
		$$->AddModuleExp(new ModuleExp($1, NULL)); 
		$$->MergeSuccessor($2); 
		$$->AddModuleExp(new ModuleExp($3, NULL)); 
	}

leftsb
	:  T_LS { $$ = new QualifiedName(Loc(), $1); }

rightsb
	:  T_RS { $$ = new QualifiedName(Loc(), $1); }

name_list
	: name { $$ = new NameList($1->GetLocation()); $$->AddName($1); }
	| name T_COMMA name_list { $$ = $3; $$->AddName($1); }

module_def_list
	: module_def { $$ = new ModuleDefList(Loc()); $$->AddModuleDef($1); }
	| module_def_list module_def { $$ = $1; $1->AddModuleDef($2); }

module_def
	: qualified_name { $$ = new ModuleDef($1, NULL); }
	| qualified_name T_LP argument_spec_list T_RP { $$ = new ModuleDef($1, $3); }

argument_spec_list
	: argument_spec { $$ = new ArgumentSpecList(); $$->AddArgumentSpec($1); }
	| argument_spec_list  T_COMMA argument_spec { $$ = $1; $$->AddArgumentSpec($3); }

argument_spec
	: T_INT name   { $$ = new ArgumentSpec($2,  LSystem::ArgumentType::Int); }
	| T_FLOAT name { $$ = new ArgumentSpec($2,  LSystem::ArgumentType::Float); }
	| T_BOOL name  { $$ = new ArgumentSpec($2,  LSystem::ArgumentType::Bool); }
	| T_VEC2 name  { $$ = new ArgumentSpec($2,  LSystem::ArgumentType::Vec2); }
	| T_VEC3 name  { $$ = new ArgumentSpec($2,  LSystem::ArgumentType::Vec3); }
	| T_VEC4 name  { $$ = new ArgumentSpec($2,  LSystem::ArgumentType::Vec4); }

module_exp
	: qualified_name { $$ = new ModuleExp($1, NULL); }
	| qualified_name T_LP T_RP { $$ = new ModuleExp($1, NULL); }
	| qualified_name T_LP parameter_list T_RP { $$ = new ModuleExp($1, $3); }

parameter_list
	: expression { $$ = new Expression(Expression::ExpressionList, $1->GetLocation()); $$->AddOperand($1); }
	| parameter_list  T_COMMA expression { $$ = $1; $1->AddOperand($3); }

expression
	: logical_or_exp
	| logical_or_exp T_QUESTIONMARK expression T_COLON expression  
	{
		$$ = new Expression(Expression::Conditional, Loc()); 
		$$->AddOperand($1);
		$$->AddOperand($3);
		$$->AddOperand($5);
	}

logical_or_exp
	: logical_and_exp
	| logical_or_exp T_OR logical_and_exp 
	{ 
		$$ = new Expression(Expression::Or, Loc());
		$$->AddOperand($1);
		$$->AddOperand($3); 
	}

logical_and_exp
	: bitwise_or_exp
	| logical_and_exp T_AND bitwise_or_exp
	{
		$$ = new Expression(Expression::And, Loc());
		$$->AddOperand($1);
		$$->AddOperand($3);
	}

bitwise_or_exp
	: xor_exp
	| bitwise_or_exp T_PIPE xor_exp 
	{ 
		$$ = new Expression(Expression::BitwiseOr, Loc());
		$$->AddOperand($1);
		$$->AddOperand($3);
	}

xor_exp
	: bitwise_and_exp
	| xor_exp T_XOR bitwise_and_exp
	{
		$$ = new Expression(Expression::XOr, Loc());
		$$->AddOperand($1);
		$$->AddOperand($3);
	}

bitwise_and_exp
	: equality_exp
	| bitwise_and_exp T_AMPERSAND equality_exp 
	{ 
		$$ = new Expression(Expression::BitwiseAnd, Loc());
		$$->AddOperand($1);
		$$->AddOperand($3);
	}

equality_exp
	: relational_exp
	| equality_exp T_EQUALS relational_exp	
	{
		$$ = new Expression(Expression::Equals, Loc());
		$$->AddOperand($1);
		$$->AddOperand($3);
	}
	| equality_exp T_NOTEQUALS relational_exp
	{
		$$ = new Expression(Expression::NotEquals, Loc());
		$$->AddOperand($1);
		$$->AddOperand($3);
	}

relational_exp
	: shift_exp
	| relational_exp T_LT shift_exp 
	{
		$$ = new Expression(Expression::LessThan, Loc());
		$$->AddOperand($1);
		$$->AddOperand($3);
	}
	| relational_exp T_GT shift_exp
	{
		$$ = new Expression(Expression::GreaterThan, Loc());
		$$->AddOperand($1);
		$$->AddOperand($3);
	}
	| relational_exp T_LE shift_exp
	{
		$$ = new Expression(Expression::LessOrEqual, Loc());
		$$->AddOperand($1);
		$$->AddOperand($3);
	}
	| relational_exp T_GE shift_exp
	{
		$$ = new Expression(Expression::GreaterOrEqual, Loc());
		$$->AddOperand($1);
		$$->AddOperand($3);
	}

shift_exp
	: additive_exp
	| shift_exp T_SHR additive_exp
	{
		$$ = new Expression(Expression::ShiftRight, Loc());
		$$->AddOperand($1);
		$$->AddOperand($3);
	}
	| shift_exp T_SHL additive_exp
	{
		$$ = new Expression(Expression::ShiftLeft, Loc());
		$$->AddOperand($1);
		$$->AddOperand($3);
	}

additive_exp
	: mult_exp
	| additive_exp T_PLUS mult_exp
	{
		$$ = new Expression(Expression::Add, Loc());
		$$->AddOperand($1);
		$$->AddOperand($3);
	}
	| additive_exp T_MINUS mult_exp
	{
		$$ = new Expression(Expression::Subtract, Loc());
		$$->AddOperand($1);
		$$->AddOperand($3);
	}

mult_exp
	: cast_exp
	| mult_exp T_MUL cast_exp
	{
		$$ = new Expression(Expression::Multiply, Loc());
		$$->AddOperand($1);
		$$->AddOperand($3);
	}
	| mult_exp T_DIV cast_exp
	{
		$$ = new Expression(Expression::Divide, Loc());
		$$->AddOperand($1);
		$$->AddOperand($3);
	}
	| mult_exp T_MOD cast_exp
	{
		$$ = new Expression(Expression::Remainder, Loc());
		$$->AddOperand($1);
		$$->AddOperand($3);
	}

cast_exp
	: unary_exp { $$ = $1; }
	| T_LP type_spec T_RP cast_exp
	{
		$$ = new Expression(Expression::TypeCast, Loc());
		$$->AddOperand($2);
		$$->AddOperand($4);
	}
	;

type_spec
	: T_INT	{ $$ = new Expression(Expression::IntType, Loc()); }
	| T_FLOAT { $$ = new Expression(Expression::FloatType, Loc()); }
	| T_BOOL	{ $$ = new Expression(Expression::BoolType, Loc()); }
	| T_VEC2	{ $$ = new Expression(Expression::Vec2Type, Loc()); }
	| T_VEC3	{ $$ = new Expression(Expression::Vec3Type, Loc()); }
	| T_VEC4	{ $$ = new Expression(Expression::Vec4Type, Loc()); }
	;

unary_exp
	: unary_op primary_exp  
	{
		$$ = new Expression((Expression::Type) $1, Loc()); 
		$$->AddOperand($2);
	}
	| postfix_exp

postfix_exp
	: primary_exp T_LP T_RP					
	{
		$$ = new Expression(Expression::FuncCall, Loc()); 
		$$->AddOperand($1);
	}
	| primary_exp T_LP parameter_list T_RP	
	{ 
		$$ = new Expression(Expression::FuncCall, Loc());
		$$->AddOperand($1);
		$$->AddOperand($3);
	}
	| primary_exp T_DOT T_NAME	
	{ 
		$$ = new Expression(Expression::MemberSelect, Loc());
		$$->AddOperand($1);
		$$->AddOperand(new Expression(Loc(), $3));
	}
	| primary_exp

unary_op
	: T_PLUS			{ $$ = Expression::UnaryPlus; }
	| T_MINUS			{ $$ = Expression::UnaryMinus; }
	| T_EXCLAMATION	{ $$ = Expression::Not; }
	| T_TILDE			{ $$ = Expression::Negate; }

primary_exp
	: T_NUM		{ $$ = new Expression(Loc(), $1); }
	| T_FNUM		{ $$ = new Expression(Loc(), $1); }
	| T_TRUE		{ $$ = new Expression(Loc(), $1); }
	| T_FALSE		{ $$ = new Expression(Loc(), $1); }
	| T_NAME		{ $$ = new Expression(Loc(), $1); }
	| T_LP expression T_RP	{ $$ = $2; }
	| T_VEC2 T_LP additive_exp T_COMMA additive_exp T_RP		
	{ 
		$$ = new Expression(Expression::Vec2, Loc());
		$$->AddOperand($3);
		$$->AddOperand($5);
	}
	| T_VEC3 T_LP additive_exp T_COMMA additive_exp T_COMMA additive_exp T_RP		
	{ 
		$$ = new Expression(Expression::Vec3, Loc());
		$$->AddOperand($3);
		$$->AddOperand($5);
		$$->AddOperand($7);
	}
	| T_VEC4 T_LP additive_exp T_COMMA additive_exp T_COMMA additive_exp T_COMMA additive_exp T_RP		
	{ 
		$$ = new Expression(Expression::Vec4, Loc());
		$$->AddOperand($3);
		$$->AddOperand($5);
		$$->AddOperand($7);
		$$->AddOperand($9);
	}

%%

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
