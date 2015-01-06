#include <iostream>
#include "codegen.h"
#include "node.h"
using namespace std;

extern int yyparse();
extern NBlock* programBlock;
extern FILE *yyin;

unsigned long long linecount = 0;

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

	CodeGenContext context;
	context.generateCode(*programBlock);
	context.runCode();
	return 0;
}
