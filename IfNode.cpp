#include "IfNode.hpp"
using namespace std;
IfNode::IfNode(NExpression* cond, const StatementList& then, ElseNode* els)
: condition(cond), body(then), els(els)
{
}
llvm::Value* IfNode::codeGen(ICompilerEngine* ctxt)
{
  puts("At: ifnode codegen, ");

  cout << body.size() << " nested statements." << endl;

  auto evaluation = condition->codeGen(ctxt);
//
  auto& builder = ctxt->builder;
//  // Convert condition to a bool by comparing equal to 0.0.
  //evaluation = builder.CreateFCmpONE(
      //evaluation, ConstantFP::get(getGlobalContext(), APFloat(0.0)), "ifcond");
//
  BasicBlock *ThenBB =
      BasicBlock::Create(getGlobalContext(), "then", ctxt->CurrentFunction());
  BasicBlock *ElseBB = BasicBlock::Create(getGlobalContext(), "continue", ctxt->CurrentFunction());
  //BasicBlock *MergeBB = BasicBlock::Create(getGlobalContext(), "ifcont");
//
  builder.CreateCondBr(evaluation, ThenBB, ElseBB);
//
//  // Set Then Block
  ctxt->PushBlock(ThenBB);
//
//  // Generate if body
  llvm::Value* then_val = nullptr;

  for(auto stat : body)
    then_val = stat->codeGen(ctxt);
//
  builder.CreateBr(ElseBB);
//  // Have yet to understand what this one does
  //builder.CreateBr(MergeBB);
//  ThenBB = builder.GetInsertBlock();
//
  ctxt->PopBlock();
  // Pop again and set the insert point to continue
  ctxt->PopBlock();
  ctxt->PushBlock(ElseBB);
  //ctxt->CurrentModule()->dump();
  //ctxt->CurrentFunction()->getBasicBlockList().push_back(ElseBB);
  //ctxt->CurrentFunction()->getBasicBlockList().push_back(MergeBB);
  //ctxt->PushBlock(MergeBB);

  //auto pn = builder.CreatePHI(Type::getVoidTy(getGlobalContext()), 1, "iftmp");

  //pn->addIncoming(then_val, ThenBB);
  //auto mod = ctxt->CurrentModule();
  //mod->dump();


  //return pn;
  return nullptr;
}

ElseNode::ElseNode(const StatementList& then, IfNode* ifnode)
: body(then), if_ext(ifnode)
{
}
llvm::Value* ElseNode::codeGen(ICompilerEngine* ctxt)
{
  return nullptr;
}
