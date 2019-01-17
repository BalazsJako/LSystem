// LSystem.cpp : Defines the entry point for the console application.
//

#include <stdio.h>
#include <iostream>

#include "Parser.tab.h"
#include "LSystem.h"

using namespace std;

extern LSystem::AST::Grammar * mGrammar;
extern char * fileName;
extern FILE * yyin;
int yyparse ();

int main(int argc, char ** argv)
{
#if defined(YYDEBUG) && defined(DEBUG)
	//	yydebug = 1;
#endif

	try
	{
		if (argc > 1)
		{
			if (NULL == (yyin = fopen(argv[1], "rt")))
			{
				cerr << "error: cannot open: " << argv[1] << endl;
				exit(1);
			}
			fileName = argv[1];
		}
		//yydebug = 1;
		yyparse();

		if (mGrammar != NULL)
		{
			LSystem::LSystem lsys(mGrammar);
			lsys.Run();
		}
	}
	catch (const std::exception& x)
	{
		cerr << "ERROR: exception caught: " << x.what() << endl;
	}
	catch (...)
	{
		cerr << "ERROR: unknown exception caught" << endl;
	}
	return 0;
}

