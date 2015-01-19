#ifndef _ICOMPILER_ENGINE_INTERFACE_HPP_
#define _ICOMPILER_ENGINE_INTERFACE_HPP_
#include <llvm/IR/Module.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/PassManager.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/CallingConv.h>
#include <llvm/Bitcode/ReaderWriter.h>
#include <llvm/Analysis/Verifier.h>
#include <llvm/Assembly/PrintModulePass.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/ExecutionEngine/GenericValue.h>
#include <llvm/ExecutionEngine/JIT.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/IR/ValueSymbolTable.h>
using namespace llvm;

class NBlock;

class ICompilerEngine {
public:
  // The builder is here cause it is one of those things
  // that will probably persist in any offshoot implementation
  IRBuilder<> builder;
public:
  ICompilerEngine(LLVMContext& ctx): builder(ctx) { }

  virtual Function*         CurrentFunction() = 0;
  virtual BasicBlock*       CurrentBBlock () = 0;
  virtual Module*           CurrentModule () = 0;
  virtual ValueSymbolTable* CurrentValSymTable () = 0;

  virtual void PushBlock (BasicBlock* bb) = 0;
  virtual void PopBlock () = 0;
  virtual void StartGen (NBlock* root) = 0;
  virtual GenericValue Test () = 0;

  virtual ~ICompilerEngine(){ }
};
#endif // _ICOMPILER_ENGINE_INTERFACE_HPP_
