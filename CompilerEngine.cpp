#include "node.h"
#include "CompilerEngine.hpp"
#include "parser.parser.hh"

using namespace std;
using namespace llvm;
// -------------------------------
CompilerEngine* CompilerEngine::inst;

CompilerEngine::CompilerEngine():
  ICompilerEngine(getGlobalContext()),
  main_module(new Module("main", getGlobalContext()))
{
  if(inst!=nullptr)
    throw;
  type_table["void"] = Type::getVoidTy(getGlobalContext());
  type_table["int"] = Type::getInt32Ty(getGlobalContext());
  type_table["long"]= Type::getInt64Ty(getGlobalContext());
  type_table["double"]=Type::getDoubleTy(getGlobalContext());
  type_table["float"] =Type::getFloatTy(getGlobalContext());
  type_table["cstring"]=Type::getInt8PtrTy(getGlobalContext());
  puts("Basic types registered");
}
ICompilerEngine::Ctxt CompilerEngine::CurrentCtxtTy() const
{
  return ctxt_stak.top();
}
void CompilerEngine::PushContext(Ctxt nctx)
{
  ctxt_stak.push(nctx);
}
void CompilerEngine::PopContext()
{
  ctxt_stak.pop();
}
IClass* CompilerEngine::CurrentClass()
{
  return class_stak.top();
}
void CompilerEngine::PushClass(IClass* cls)
{
  class_stak.push(cls);
  ctxt_stak.push(Ctxt::Class);
}
void CompilerEngine::PopClass()
{
  assert(ctxt_stak.top() == Ctxt::Class);
  ctxt_stak.pop();
  class_stak.pop();
}
void CompilerEngine::AddType(const std::string& s, llvm::Type* t)
{
  type_table[s] = t;
}
/* Returns an LLVM type based on the identifier */
Type* CompilerEngine::TypeOf(const std::string& type)
{
	if(type_table.find(type) != type_table.end()) {
    return type_table[type];
	}
	cout << "Warning: Type of '" << type << "' unrecognized\n";
	return Type::getVoidTy(getGlobalContext());
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
