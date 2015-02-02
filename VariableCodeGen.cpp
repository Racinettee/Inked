#include "ICompilerEngine.hpp"
#include "node.h"
#include "VariableNodes.hpp"
Value* NVariableDeclaration::codeGen(ICompilerEngine* context)
{
	std::cout << "Creating variable declaration " << type.name << " " << id.name << std::endl;
	AllocaInst *alloc = new AllocaInst(context->TypeOf(type.name), id.name.c_str(), context->CurrentBBlock());

	if (assignmentExpr != NULL) {
		NAssignment assn(id, *assignmentExpr);
		assn.codeGen(context);
	}
	return alloc;
}

