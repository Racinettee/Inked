#ifndef _COMPILER_ENGINE_CONTEXT_HPP_
#define _COMPILER_ENGINE_CONTEXT_HPP_
#include <stack>

#include "ICompilerEngine.hpp"

class CompilerEngine : public ICompilerEngine{
public:
  std::stack<BasicBlock*> block_stack;
  Function*               current_function;
  Module*                 main_module;
  Module*                 current_module;
  ValueSymbolTable*       current_lookup;
  Function*               main_function;
  // ------------------------------------------
  static CompilerEngine* inst;
  CompilerEngine();

  Function*         CurrentFunction();
  BasicBlock*       CurrentBBlock();
  Module*           CurrentModule();
  ValueSymbolTable* CurrentValSymTable();

  GenericValue Test();
  void StartGen(NBlock* root);
  void PushBlock(BasicBlock* bb);
  void PopBlock();
};
#endif // _COMPILER_ENGINE_CONTEXT_HPP_
