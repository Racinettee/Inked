#include "ICompilerEngine.hpp"
#include "node.h"
#include "VariableNodes.hpp"
Value* NVariableDeclaration::codeGen(ICompilerEngine* context)
{
  // If this variable is declared in a class context
  // then add it to the class's member variables
  if(context->CurrentCtxtTy() == ICompilerEngine::Ctxt::Class)
  {
    auto crrt_class = context->CurrentClass();
    if(!crrt_class->HasMemberVar(id.name))
    {
      crrt_class->AddMemberVar(id.name, type.name);
    }
    else
    {
      std::cout << "Warning: redecl of " <<
        id.name << ", ignored." << std::endl;
    }
    return nullptr;
  }
	//std::cout << "Creating variable declaration " << type.name << " " << id.name << std::endl;
	AllocaInst *alloc = new AllocaInst(
    context->TypeOf(type.name),
    id.name.c_str(),
    context->CurrentBBlock());

	if (assignmentExpr != NULL) {
		NAssignment assn(id, *assignmentExpr);
		assn.codeGen(context);
	}
	return alloc;
}

