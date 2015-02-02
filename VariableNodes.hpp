#ifndef VARIABLE_AST_NODE_HPP
#define VARIABLE_AST_NODE_HPP

class NVariableDeclaration : public NStatement {
public:
  const NIdentifier& type;
  NIdentifier& id;
  NExpression *assignmentExpr;
  NVariableDeclaration(const NIdentifier& type, NIdentifier& id) :
      type(type), id(id) { }
  NVariableDeclaration(const NIdentifier& type, NIdentifier& id, NExpression *assignmentExpr) :
      type(type), id(id), assignmentExpr(assignmentExpr) { }
  virtual llvm::Value* codeGen(ICompilerEngine* context);
};

#endif // VARIABLE_AST_NODE_HPP
