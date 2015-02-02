#include <iostream>
#include "CompilerEngine.hpp"
#include "node.h"
using namespace std;

extern int yyparse();
extern NBlock* programBlock;
extern FILE *yyin;

unsigned long long linecount = 0;
ICompilerEngine* ICompilerEngine::engine;

int main(int argc, char** argv)
{
	printf("argc: %d\n", argc);
	if(argc > 1)
	{
		printf("parsing: %s\n", argv[1]);
		yyin = fopen(argv[1], "r");
	}
	else
		yyin = stdin;
	puts("parsing...");
	yyparse();
	puts("parsed");
	cout << programBlock << std::endl;

	using CompEngine = std::unique_ptr<CompilerEngine>;

	CompEngine cengine(new CompilerEngine());

	ICompilerEngine::engine = cengine.get();

	cengine->StartGen(programBlock);
	cengine->Test();
	return 0;
}
