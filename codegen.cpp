#include "node.h"
#include "ICompilerEngine.hpp"
#include "parser.parser.hh"

using namespace std;
using namespace llvm;

/* Returns an LLVM type based on the identifier */
static Type* typeOf(const NIdentifier& type)
{
  if (type.name == "void") {
    return Type::getVoidTy(getGlobalContext());
  }
	else if (type.name == "int") {
		return Type::getInt32Ty(getGlobalContext());
	}
	else if (type.name == "long") {
    return Type::getInt64Ty(getGlobalContext());
	}
	else if (type.name == "double") {
		return Type::getDoubleTy(getGlobalContext());
	}
	else if (type.name == "cstring") {
    return Type::getInt8PtrTy(getGlobalContext());
	}
	cout << "Warning: Type of '" << type.name << "' unrecognized\n";
	return Type::getVoidTy(getGlobalContext());
}

/* -- Code Generation -- */

Value* NInteger::codeGen(ICompilerEngine* context)
{
	std::cout << "Creating integer: " << value << std::endl;
	return ConstantInt::get(Type::getInt64Ty(getGlobalContext()), value, true);
}

Value* NDouble::codeGen(ICompilerEngine* context)
{
	std::cout << "Creating double: " << value << std::endl;
	return ConstantFP::get(Type::getDoubleTy(getGlobalContext()), value);
}

Value* NString::codeGen(ICompilerEngine* context)
{
	std::cout << "Creating string: " << value << std::endl;
	return context->builder.CreateGlobalStringPtr(value);
}

Value* NIdentifier::codeGen(ICompilerEngine* context)
{
	std::cout << "Creating identifier reference: " << name << std::endl;
  auto current_block = context->CurrentBBlock();
	auto sym_table = context->CurrentValSymTable();
	auto val = sym_table->lookup(name);

  if(val == nullptr){
    std::cerr << "undeclared variable " << name << std::endl;
    return NULL;
  }
  return context->builder.CreateLoad(val);
}

Value* NMethodCall::codeGen(ICompilerEngine* context)
{
	Function *function = context->CurrentModule()->getFunction(id.name.c_str());
	std::cout << "Creating call to function: " << id.name.c_str() <<
	  function << std::endl;

	if (function == NULL) {
		std::cerr << "no such function " << id.name << std::endl;
	}
	std::vector<Value*> args;
	ExpressionList::const_iterator it;
	for (it = arguments.begin(); it != arguments.end(); it++) {
		args.push_back((**it).codeGen(context));
	}
	CallInst *call = CallInst::Create(function, args, "", context->CurrentBBlock());
	std::cout << "Creating method call: " << id.name << std::endl;
	return call;
}

Value* NBinaryOperator::codeGen(ICompilerEngine* context)
{
	std::cout << "Creating binary operation " << op << std::endl;
	Instruction::BinaryOps instr;
	switch (op) {
		case TPLUS: 	instr = Instruction::Add; goto math;
		case TMINUS: 	instr = Instruction::Sub; goto math;
		case TMUL: 		instr = Instruction::Mul; goto math;
		case TDIV: 		instr = Instruction::SDiv; goto math;

		/* TODO comparison */
	}

	return NULL;
math:
	return BinaryOperator::Create(instr, lhs.codeGen(context),
		rhs.codeGen(context), "", context->CurrentBBlock());
}

Value* NAssignment::codeGen(ICompilerEngine* context)
{
	std::cout << "Creating assignment for " << lhs.name << std::endl;
	auto current_block = context->CurrentBBlock();
	auto sym_table = current_block->getValueSymbolTable();
	auto val = sym_table->lookup(lhs.name);
	if(val == nullptr){
    auto paren = current_block->getParent();
    sym_table = &paren->getValueSymbolTable();
    val = sym_table->lookup(lhs.name);
  }
  if(val == nullptr){
    std::cerr << "undeclared variable " << lhs.name << std::endl;
    return NULL;
  }
	return new StoreInst(rhs.codeGen(context),
      context->CurrentValSymTable()->lookup(lhs.name), false, context->CurrentBBlock());
}

Value* NBlock::codeGen(ICompilerEngine* context)
{
	StatementList::const_iterator it;
	Value *last = NULL;
	for (it = statements.begin(); it != statements.end(); it++) {
		std::cout << "Generating code for " << typeid(**it).name() << std::endl;
		last = (**it).codeGen(context);
	}
	std::cout << "Creating block" << std::endl;
	return last;
}

Value* NExpressionStatement::codeGen(ICompilerEngine* context)
{
	std::cout << "Generating code for " << typeid(expression).name() << std::endl;
	return expression.codeGen(context);
}

Value* NVariableDeclaration::codeGen(ICompilerEngine* context)
{
	std::cout << "Creating variable declaration " << type.name << " " << id.name << std::endl;
	AllocaInst *alloc = new AllocaInst(typeOf(type), id.name.c_str(), context->CurrentBBlock());

	if (assignmentExpr != NULL) {
		NAssignment assn(id, *assignmentExpr);
		assn.codeGen(context);
	}
	return alloc;
}

Value* NFunctionPrototype::codeGen(ICompilerEngine* ctx)
{
  puts("Creating function prototype");
  vector<Type*> argTypes;
	VariableList::const_iterator it;
	for (it = arguments.begin(); it != arguments.end(); it++) {
		argTypes.push_back(typeOf((**it).type));
	}
	FunctionType *ftype = FunctionType::get(typeOf(type), argTypes, false);

  llvm::Constant *putsFunc = ctx->CurrentModule()->getOrInsertFunction(id.name.c_str(), ftype);

	return putsFunc;
}

Value* NFunctionDeclaration::codeGen(ICompilerEngine* context)
{
	vector<Type*> argTypes;
	VariableList::const_iterator it;
	for (it = arguments.begin(); it != arguments.end(); it++) {
		argTypes.push_back(typeOf((**it).type));
	}
	FunctionType *ftype = FunctionType::get(typeOf(type), argTypes, false);
	Function *function = Function::Create(ftype, GlobalValue::InternalLinkage, id.name.c_str(), context->CurrentModule());
	BasicBlock *bblock = BasicBlock::Create(getGlobalContext(), "entry", function, 0);

	int ctr = 0;
	for(auto itr = function->arg_begin(); itr != function->arg_end(); itr++)
  {
    itr->setName(arguments[ctr]->id.name);
    ctr++;
  }

	context->PushBlock(bblock);

	// This appears to cause allocation of the parameter types? as well as creating
	// additional copys of the parameter with different names
	//for (it = arguments.begin(); it != arguments.end(); it++) {
	//	(**it).codeGen(context);
	//}

	block.codeGen(context);
	ReturnInst::Create(getGlobalContext(), bblock);

	context->PopBlock();
	std::cout << "Creating function: " << id.name << std::endl;
	return function;
}
