#include <llvm/IR/ValueSymbolTable.h>
#include "node.h"
#include "codegen.h"
#include "parser.parser.hh"

using namespace std;
using namespace llvm;

void CompilerEngine::StartGen()
{
  vector<Type*> argTypes;
	FunctionType* ftype =
    FunctionType::get(
      Type::getVoidTy(getGlobalContext()),
      argTypes,
      false
    );
  mainFunction = Function::Create(ftype, GlobalValue::InternalLinkage, "main", module);
	BasicBlock *bblock = BasicBlock::Create(getGlobalContext(), "entry", mainFunction, 0);
	puts("Main function created. Generating code...");
	this->PushBlock(bblock);
}

/* Compile the AST into a module */
void CodeGenContext::generateCode(NBlock& root)
{
  vector<Type*> argTypes;
	FunctionType* ftype =
    FunctionType::get(
      Type::getVoidTy(getGlobalContext()),
      argTypes,
      false
    );
  mainFunction = Function::Create(ftype, GlobalValue::InternalLinkage, "main", module);
	BasicBlock *bblock = BasicBlock::Create(getGlobalContext(), "entry", mainFunction, 0);
	builder.SetInsertPoint(bblock);

	puts("Generating code...");//cout << "Generating code...\n";

  module->dump();
	/* Push a new variable/block context */
	pushBlock(bblock);
	root.codeGen(*this); /* emit bytecode for the toplevel block */
	ReturnInst::Create(getGlobalContext(), bblock);
	popBlock();

	/* Print the bytecode in a human-readable format
	   to see if our program compiled properly
	 */
	puts("generated");
	PassManager pm;
	//pm.add(createPrintModulePass(&outs()));
	module->dump();
	pm.run(*module);
}

/* Executes the AST by running the main function */
GenericValue CodeGenContext::runCode() {

  InitializeNativeTarget();

	puts("Running code...");
	EngineBuilder builder(module);

	auto ee = builder.create();
	vector<GenericValue> noargs;
	GenericValue v = ee->runFunction(mainFunction, noargs);
	puts("The code was run.");
  return v;
}

/* Returns an LLVM type based on the identifier */
//static const Type *typeOf(const NIdentifier& type)
static Type* typeOf(const NIdentifier& type)
{
  if (type.name == "void") {
    return Type::getVoidTy(getGlobalContext());
  }
	else if (type.name.compare("int") == 0) {
		return Type::getInt32Ty(getGlobalContext());
	}
	else if (type.name.compare("long") == 0) {
    return Type::getInt64Ty(getGlobalContext());
	}
	else if (type.name.compare("double") == 0) {
		return Type::getDoubleTy(getGlobalContext());
	}
	else if (type.name.compare("cstring") == 0) {
    return Type::getInt8PtrTy(getGlobalContext());
	}
	cout << "Warning: Type of '" << type.name << "' unrecognized\n";
	return Type::getVoidTy(getGlobalContext());
}

/* -- Code Generation -- */

Value* NInteger::codeGen(CodeGenContext& context)
{
	std::cout << "Creating integer: " << value << std::endl;
	return ConstantInt::get(Type::getInt64Ty(getGlobalContext()), value, true);
}

Value* NDouble::codeGen(CodeGenContext& context)
{
	std::cout << "Creating double: " << value << std::endl;
	return ConstantFP::get(Type::getDoubleTy(getGlobalContext()), value);
}

Value* NString::codeGen(CodeGenContext& context)
{
	std::cout << "Creating string: " << value << std::endl;

	return context.builder.CreateGlobalStringPtr(value);
}

Value* NIdentifier::codeGen(CodeGenContext& context)
{
  context.module->dump();
	std::cout << "Creating identifier reference: " << name << std::endl;
  auto current_block = context.currentBlock();
	auto sym_table = current_block->getValueSymbolTable();
	auto val = sym_table->lookup(name);

  if(val == nullptr){
    std::cerr << "undeclared variable " << name << std::endl;
    return NULL;
  }
//	if (context.locals().find(name) == context.locals().end()) {
//		std::cerr << "undeclared variable " << name << std::endl;
//		return nullptr;
//	}
  context.module->dump();
  return new LoadInst(val, "", current_block);
	//return new LoadInst(context.locals()[name], "", true, current_block);
}

Value* NMethodCall::codeGen(CodeGenContext& context)
{
	Function *function = context.module->getFunction(id.name.c_str());
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
	//CallInst *call = CallInst::Create(function, args.begin(), args.end(), "", context.currentBlock());
	CallInst *call = CallInst::Create(function, args, "", context.currentBlock());
	std::cout << "Creating method call: " << id.name << std::endl;
	return call;
}

Value* NBinaryOperator::codeGen(CodeGenContext& context)
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
		rhs.codeGen(context), "", context.currentBlock());
}

Value* NAssignment::codeGen(CodeGenContext& context)
{
	std::cout << "Creating assignment for " << lhs.name << std::endl;
	//if (context.locals().find(lhs.name) == context.locals().end()) {
	auto current_block = context.currentBlock();
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
	return new StoreInst(rhs.codeGen(context), context.locals()[lhs.name], false, context.currentBlock());
}

Value* NBlock::codeGen(CodeGenContext& context)
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

Value* NExpressionStatement::codeGen(CodeGenContext& context)
{
	std::cout << "Generating code for " << typeid(expression).name() << std::endl;
	return expression.codeGen(context);
}

Value* NVariableDeclaration::codeGen(CodeGenContext& context)
{
	std::cout << "Creating variable declaration " << type.name << " " << id.name << std::endl;
	AllocaInst *alloc = new AllocaInst(typeOf(type), id.name.c_str(), context.currentBlock());
	context.locals()[id.name] = alloc;
	if (assignmentExpr != NULL) {
		NAssignment assn(id, *assignmentExpr);
		assn.codeGen(context);
	}
	return alloc;
}

Value* NFunctionPrototype::codeGen(CodeGenContext& ctx)
{
  vector<Type*> argTypes;
	VariableList::const_iterator it;
	for (it = arguments.begin(); it != arguments.end(); it++) {
		argTypes.push_back(typeOf((**it).type));
	}
	FunctionType *ftype = FunctionType::get(typeOf(type), argTypes, false);
  //std::vector<llvm::Type *> putsArgs;
  //putsArgs.push_back(builder.getInt8Ty()->getPointerTo());
  llvm::ArrayRef<llvm::Type*>  argsRef(argTypes);//putsArgs);

  llvm::FunctionType *putsType =
    llvm::FunctionType::get(typeOf(type)/*builder.getInt32Ty()*/, argsRef, false);
  llvm::Constant *putsFunc = ctx.module->getOrInsertFunction(id.name.c_str()/*"puts"*/, putsType);

	return putsFunc;
}

Value* NFunctionDeclaration::codeGen(CodeGenContext& context)
{
	vector<Type*> argTypes;
	VariableList::const_iterator it;
	for (it = arguments.begin(); it != arguments.end(); it++) {
		argTypes.push_back(typeOf((**it).type));
	}
	FunctionType *ftype = FunctionType::get(typeOf(type), argTypes, false);
	Function *function = Function::Create(ftype, GlobalValue::InternalLinkage, id.name.c_str(), context.module);
	BasicBlock *bblock = BasicBlock::Create(getGlobalContext(), "entry", function, 0);

	int ctr = 0;
	for(auto itr = function->arg_begin(); itr != function->arg_end(); itr++)
  {
    itr->setName(arguments[ctr]->id.name);
    ctr++;
  }

  context.module->dump();

	context.pushBlock(bblock);

	// This appears to cause allocation of the parameter types? as well as creating
	// additional copys of the parameter with different names
	//for (it = arguments.begin(); it != arguments.end(); it++) {
	//	(**it).codeGen(context);
	//}

	block.codeGen(context);
	ReturnInst::Create(getGlobalContext(), bblock);

	context.popBlock();
	std::cout << "Creating function: " << id.name << std::endl;
	return function;
}
