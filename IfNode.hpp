#ifndef _IF_NODE_HPP_
#define _IF_NODE_HPP_
#include "node.h"
class ICompilerEngine;
class ElseNode;
class IfNode: public NStatement
{
  NExpression* condition;
  StatementList body;
  ElseNode* els;
public:
  IfNode(NExpression* cond, const StatementList& then, ElseNode* els = nullptr);
  virtual llvm::Value* codeGen(ICompilerEngine* ctxt);
  ~IfNode()
  {
    delete condition;
  }
};
class ElseNode: public NStatement
{
  StatementList body;
  IfNode* if_ext;
public:
  ElseNode(const StatementList& then, IfNode* ifnode = nullptr);
  virtual llvm::Value* codeGen(ICompilerEngine* ctxt);
  ~ElseNode()
  {
    delete if_ext;
  }
};
#endif // _IF_NODE_HPP_
