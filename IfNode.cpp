#include "IfNode.hpp"
using namespace std;
IfNode::IfNode(NExpression* cond, const StatementList& then, ElseNode* els)
: condition(cond), body(then), els(els)
{
}
llvm::Value* IfNode::codeGen(ICompilerEngine* ctxt)
{
  auto evaluation = condition->codeGen(ctxt);
  //
  auto& builder = ctxt->builder;
  //
  auto functn = ctxt->CurrentFunction();
  //
  BasicBlock* ElseBB = nullptr;
  //
  BasicBlock* ThenBB = BasicBlock::Create(getGlobalContext(), "then", functn);
  ElseBB = (els != nullptr ? BasicBlock::Create(getGlobalContext(), "else", functn) : nullptr);
  BasicBlock* ContBB = BasicBlock::Create(getGlobalContext(), "ifnd", functn);
  //
  builder.CreateCondBr(evaluation, ThenBB, ElseBB != nullptr? ElseBB : ContBB);
  //
  // Set Then Block
  ctxt->PushBlock(ThenBB);
  //
  // Generate if body
  llvm::Value* then_val = nullptr;

  for(auto stat : body)
    then_val = stat->codeGen(ctxt);
//
  builder.CreateBr(ContBB); //ContBB);

  ctxt->PopBlock();

  if(els != nullptr)
  {
    ctxt->PushBlock(ElseBB);
    els->codeGen(ctxt);
    builder.CreateBr(ContBB);
    ctxt->PopBlock();
  }
  // Pop again and set the insert point to continue
  ctxt->PopBlock();
  ctxt->PushBlock(ContBB);

  return nullptr;
}

ElseNode::ElseNode(const StatementList& then, IfNode* ifnode)
: body(then), if_ext(ifnode)
{
}
llvm::Value* ElseNode::codeGen(ICompilerEngine* ctxt)
{
  for(auto inst : body)
    inst->codeGen(ctxt);
  return nullptr;
}
