

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

class NClass : public NStatement {
public:
  NBlock* block;
  std::string name;
  NClass(const std::string& n, NBlock* b):name(n), block(b) { }
  virtual llvm::Value* codeGen(ICompilerEngine*);
};

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

class NMethodCall : public NExpression {
public:
    const NIdentifier& id;
    ExpressionList arguments;
    NMethodCall(const NIdentifier& id, ExpressionList& arguments) :
        id(id), arguments(arguments) { }
    NMethodCall(const NIdentifier& id) : id(id) { }
    virtual llvm::Value* codeGen(ICompilerEngine* context);
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

// NVariableReference stores an llvm value ptr to directly reference a variable
class NVariableReference : public NStatement {
public:
  const NIdentifier& name;
  llvm::Value* value = nullptr;

  NVariableReference(const NIdentifier& name, llvm::Value* value):name(name), value(value){ }

  virtual llvm::Value* codeGen(ICompilerEngine* ctx);
};

class NFunctionPrototype : public NStatement {
public:
  const NIdentifier& type;
  const NIdentifier& id;
  VariableList arguments;
  NFunctionPrototype(const NIdentifier& type, const NIdentifier& id, const VariableList& arguments):
    type(type), id(id), arguments(arguments) { }
  virtual llvm::Value* codeGen(ICompilerEngine* context);
};

class NFunctionDeclaration : public NFunctionPrototype {
public:
  NBlock& block;
  NFunctionDeclaration(const NIdentifier& type, const NIdentifier& id,
          const VariableList& arguments, NBlock& block) :
    NFunctionPrototype(type, id, arguments), block(block) { }
  virtual llvm::Value* codeGen(ICompilerEngine* context);
};
