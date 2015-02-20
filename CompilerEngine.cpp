#include "node.h"
#include "CompilerEngine.hpp"
#include "parser.parser.hh"

using namespace std;
using namespace llvm;
// -------------------------------
CompilerEngine* CompilerEngine::inst;

CompilerEngine::CompilerEngine():
  ICompilerEngine(getGlobalContext()),
  main_module(new Module("main", getGlobalContext())),
  dat_layout(main_module)
{
  if(inst!=nullptr)
    throw;
  type_table["char"] = Type::getInt8Ty(getGlobalContext());
  type_table["void"] = Type::getVoidTy(getGlobalContext());
  type_table["int"] = Type::getInt32Ty(getGlobalContext());
  type_table["long"]= Type::getInt64Ty(getGlobalContext());
  type_table["double"]=Type::getDoubleTy(getGlobalContext());
  type_table["float"] =Type::getFloatTy(getGlobalContext());
  type_table["cstring"]=Type::getInt8PtrTy(getGlobalContext());
  puts("Basic types registered");
}
size_t CompilerEngine::SizeOfType(const std::string& ty)
{
    auto type = TypeOf(ty);
    // figure out the type of type of type. if its primitive just return the size, if its a struct type
    // then it will have to be interated over
    size_t sizeof_type = 0;
    if(type->isFloatTy())
        sizeof_type = 4;

    else if(type->isDoubleTy())
        sizeof_type = 8;

    else if(type->isIntegerTy())
      sizeof_type = static_cast<llvm::IntegerType*>(type)->getBitWidth();

    else if(type->isStructTy())
    {
        // iterate over all elements and sum them

    }
    return sizeof_type;
}
DataLayout* CompilerEngine::GetDatLayout() { return &dat_layout; }
// May also push or pop current context in implementations
void CompilerEngine::PushFunction(Function* f)
{
  function_stak.push(f);
  PushContext(Ctxt::Function);
}
void CompilerEngine::PopFunction()
{
  assert(CurrentCtxtTy()==Ctxt::Function);
  PopContext();
  function_stak.pop();
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
  return function_stak.top();//current_function;
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
  // Define some functions to use from cstdlib
  llvm::FunctionType* mallocType = FunctionType::get(Type::getInt8PtrTy(getGlobalContext()),
  {
      Type::getInt64Ty(getGlobalContext())
  }, false);
  Function::Create(mallocType, GlobalValue::ExternalLinkage, "malloc", current_module);
  llvm::FunctionType* putsType = FunctionType::get(Type::getInt32Ty(getGlobalContext()),
  {
    Type::getInt8PtrTy(getGlobalContext())
  }, false);
  Function::Create(putsType, GlobalValue::ExternalLinkage, "puts", current_module);
  Function::Create(putsType, GlobalValue::ExternalLinkage, "printf", current_module);

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

	this->PushFunction(main_function);
	this->PushBlock(bblock);

	// start generating code from root and the others now
	root->codeGen(this);

	builder.CreateRetVoid();
	CurrentModule()->dump();

	this->PopBlock();
	this->PopFunction();
  //builder.CreateRetVoid();
	//main_module->dump();
	PrintStacks();
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
void CompilerEngine::PrintStacks()
{
  puts("Remaining function stak:");
  while(!function_stak.empty())
  {
    cout << "   " << function_stak.top();
    function_stak.pop();
  }
  puts("Remaining block stak:");
  while(!block_stack.empty())
  {
    cout << "   " << block_stack.top();
    block_stack.pop();
  }
  puts("Remaining class stack");
  while(!class_stak.empty())
  {
    cout << "   " << class_stak.top();
    class_stak.pop();
  }
  puts("Remaining context stack");
  while(!ctxt_stak.empty())
  {
    cout << "   " << (int)ctxt_stak.top();
    ctxt_stak.pop();
  }
}
