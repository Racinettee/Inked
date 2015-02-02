#ifndef INKED_AST_NODE_H
#define INKED_AST_NODE_H

#include <iostream>
#include <vector>
#include <llvm/IR/Value.h>

class ICompilerEngine;
class NStatement;
class NExpression;
class NVariableDeclaration;

typedef std::vector<NStatement*> StatementList;
typedef std::vector<NExpression*> ExpressionList;
typedef std::vector<NVariableDeclaration*> VariableList;

class Node {
public:
  Node(){ }
  virtual ~Node() {}
  virtual llvm::Value* codeGen(ICompilerEngine* context) { return nullptr; }
};

class NExpression : public Node {
};

class NStatement : public NExpression {
};
class NBlock;

class NInteger : public NExpression {
public:
  long long value;
  NInteger(long long value) : value(value) { }
  virtual llvm::Value* codeGen(ICompilerEngine*);
};

class NDouble : public NExpression {
public:
  double value;
  NDouble(double value) : value(value) { }
  virtual llvm::Value* codeGen(ICompilerEngine*);
};

class NString : public NExpression {
public:
	std::string value;
	NString(const std::string& v):value(v.substr(1, v.size()-2)) { }
	virtual llvm::Value* codeGen(ICompilerEngine*);
};

class NIdentifier : public NExpression {
public:
    std::string name;
    NIdentifier(const std::string& name) : name(name) { }
    virtual llvm::Value* codeGen(ICompilerEngine*);
};

class NBinaryOperator : public NExpression {
public:
    int op;
    NExpression& lhs;
    NExpression& rhs;
    NBinaryOperator(NExpression& lhs, int op, NExpression& rhs) :
        lhs(lhs), rhs(rhs), op(op) { }
    virtual llvm::Value* codeGen(ICompilerEngine* context);
};

class NAssignment : public NExpression {
public:
  NIdentifier& lhs;
  NExpression& rhs;
  NAssignment(NIdentifier& lhs, NExpression& rhs) :
        lhs(lhs), rhs(rhs) { }
  virtual llvm::Value* codeGen(ICompilerEngine* context);
};

class NBlock : public NExpression {
public:
  StatementList statements;
  virtual llvm::Value* codeGen(ICompilerEngine* context);
};

class NExpressionStatement : public NStatement {
public:
  //NExpression& expression;
  Node* expression;
  NExpressionStatement(Node* expression)://NExpression& expression) :
      expression(expression) { }
  virtual llvm::Value* codeGen(ICompilerEngine* context);
};

#include "VariableNodes.hpp"
#include "ClassNode.hpp"
#include "FunctionNode.hpp"

#endif // INKED_AST_NODE_H
