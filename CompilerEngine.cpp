#include "node.h"
#include "CompilerEngine.hpp"
#include "parser.parser.hh"

using namespace std;
using namespace llvm;

CompilerEngine* CompilerEngine::inst;

CompilerEngine::CompilerEngine():
  ICompilerEngine(getGlobalContext()),
  main_module(new Module("main", getGlobalContext()))
{
  if(inst!=nullptr)
    throw;
}
Function* CompilerEngine::CurrentFunction()
{
  return current_function;
}
BasicBlock* CompilerEngine::CurrentBBlock()
{
  return block_stack.top();
}
Module* CompilerEngine::CurrentModule()
{
  return current_module;
}
ValueSymbolTable* CompilerEngine::CurrentValSymTable()
{
  return current_lookup;
}

void CompilerEngine::PushBlock(BasicBlock* bb)
{
  block_stack.push(bb);
  builder.SetInsertPoint(bb);
  current_lookup = block_stack.top()->getValueSymbolTable();
}
void CompilerEngine::PopBlock()
{
  if(!block_stack.empty())
  {
    block_stack.pop();
  }
  if(block_stack.empty())
    return;
  else
  {
    builder.SetInsertPoint(block_stack.top());
    current_lookup = block_stack.top()->getValueSymbolTable();
  }
}
void CompilerEngine::StartGen(NBlock* root)
{
  vector<Type*> argTypes;
	FunctionType* ftype =
    FunctionType::get(
      Type::getVoidTy(getGlobalContext()),
      argTypes,
      false
    );
  main_function = Function::Create(ftype, GlobalValue::InternalLinkage, "main", main_module);
  current_module = main_module;
	BasicBlock *bblock = BasicBlock::Create(getGlobalContext(), "entry", main_function, 0);
	puts("Main function created. Generating code...");
	this->PushBlock(bblock);

	// start generating code from root and the others now
	root->codeGen(this);

	builder.CreateRetVoid();

	this->PopBlock();

	main_module->dump();

	//PassManager pm;
	//pm.run(*main_module);
//	EngineBuilder builder(main_module);
//	std::string error_str;
//  builder.setErrorStr(&error_str);
//  try {
//    auto Jit = builder.create();
//    void* ptrInit = Jit->getPointerToFunction(main_function);
//    puts("running main function");
//    ((void(*)())ptrInit)();
//  }
//  catch(...)
//  {
//    cout << error_str << endl;
//  }
}
/* Executes the AST by running the main function */
GenericValue CompilerEngine::Test() {

  InitializeNativeTarget();

	puts("Running code...");
	EngineBuilder builder(main_module);

	auto ee = builder.create();
	std::string error_str;
  builder.setErrorStr(&error_str);
	vector<GenericValue> noargs;
	cout << error_str << endl;
	GenericValue v = ee->runFunction(main_function, noargs);
	cout << error_str << endl;
	puts("The code was run.");
  return v;
}
