#include "node.h"
#include "ICompilerEngine.hpp"
#include "parser.parser.hh"
using namespace std;
Value* NMethodCall::codeGen(ICompilerEngine* context)
{
	Function *function = context->CurrentModule()->getFunction(id.name.c_str());
	//std::cout << "Creating call to function: " << id.name.c_str() <<
	//  function << std::endl;

	if (function == NULL) {
		std::cerr << "no such function " << id.name << std::endl;
		return nullptr;
	}
	std::vector<Value*> args;
	ExpressionList::const_iterator it;
	for (it = arguments.begin(); it != arguments.end(); it++) {
		args.push_back((**it).codeGen(context));
	}
	CallInst *call = CallInst::Create(function, args, "", context->CurrentBBlock());
	//std::cout << "Creating method call: " << id.name << std::endl;
	return call;
}
Value* NFunctionPrototype::codeGen(ICompilerEngine* ctx)
{
  vector<Type*> argTypes;
	VariableList::const_iterator it;
	for (it = arguments.begin(); it != arguments.end(); it++) {
		argTypes.push_back(ctx->TypeOf((**it).type.name));
	}

	FunctionType *ftype = FunctionType::get(ctx->TypeOf(type), argTypes, false);

  llvm::Constant *putsFunc = ctx->CurrentModule()->getOrInsertFunction(id.name.c_str(), ftype);

	return putsFunc;
}
Value* NFunctionDeclaration::codeGen(ICompilerEngine* context)
{
	vector<Type*> argTypes;
	VariableList::const_iterator it;
	for (it = arguments.begin(); it != arguments.end(); it++) {
		argTypes.push_back(context->TypeOf((**it).type.name));
	}
	FunctionType *ftype = FunctionType::get(context->TypeOf(type), argTypes, false);
	Function *function = Function::Create(ftype, GlobalValue::InternalLinkage, id.name, context->CurrentModule());
	BasicBlock *bblock = BasicBlock::Create(getGlobalContext(), "entry", function, 0);

	context->PushFunction(function);

	int ctr = 0;
	for(auto itr = function->arg_begin(); itr != function->arg_end(); itr++)
  {
    itr->setName(arguments[ctr]->id.name);
    ctr++;
  }

	context->PushBlock(bblock);

	// This appears to cause allocation of the parameter types in the function body. Not needed? It also
	// causes additional copys of the parameter with number names to be created. This causes
	// problems and I am going to leave it commented out here
	//for (it = arguments.begin(); it != arguments.end(); it++) {
	//	(**it).codeGen(context);
	//}

	block.codeGen(context);
	ReturnInst::Create(getGlobalContext(), bblock);

	context->PopBlock();

	context->PopFunction();
	std::cout << "Creating function: " << id.name << std::endl;
	return function;
}
