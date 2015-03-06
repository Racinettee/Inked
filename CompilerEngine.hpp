#ifndef _COMPILER_ENGINE_CONTEXT_HPP_
#define _COMPILER_ENGINE_CONTEXT_HPP_
#include <stack>

#include "ICompilerEngine.hpp"

// Conforms to ICompilerEngine
class CompilerEngine : public ICompilerEngine{
public:
  // ------------------------------------------
  static CompilerEngine* inst;
  CompilerEngine();

  DataLayout*       GetDatLayout();

  Type*             TypeOf(const std::string&);

  Function*         CurrentFunction();
  // -----------------------
  void              PushFunction(Function*);
  void              PopFunction();
  // -------------------
  BasicBlock*       CurrentBBlock();
  Module*           CurrentModule();
  ValueSymbolTable* CurrentValSymTable();

  GenericValue Test();
  void StartGen(NBlock* root);
  void PushBlock(BasicBlock* bb);
  void PopBlock();
  // -----------------------
  Ctxt CurrentCtxtTy() const;
  void PushContext(Ctxt);
  void PopContext();
  // ------------------------------------
  IClass* CurrentClass();
  // ---------------------------------
  void PushClass(IClass*);
  void PopClass();
  // --------------------
  void AddType(const std::string&, llvm::Type*);
  // ---------------------
  void PrintStacks();
  // Returns size in bytes
  size_t SizeOfType(const std::string&);
private:
  std::stack<IClass*> class_stak;
  std::stack<Function*> function_stak;
  std::map<std::string, Type*> type_table;
  std::stack<BasicBlock*> block_stack;
  std::stack<Ctxt> ctxt_stak;
  // ---------------------------------
  //Function*               current_function;
  Module*               main_module;
  Module*               current_module;
  ValueSymbolTable*     current_lookup;
  Function*             main_function;
  DataLayout            dat_layout;
private:
    void add_c_std_fnct();
};
#endif // _COMPILER_ENGINE_CONTEXT_HPP_
