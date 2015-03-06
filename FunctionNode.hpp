#ifndef FUNCTION_AST_NODE_HPP
#define FUNCTION_AST_NODE_HPP

class NMethodCall : public NExpression {
public:
    const NIdentifier& id;
    ExpressionList arguments;
    NMethodCall(const NIdentifier& id, const ExpressionList& arguments) :
        id(id), arguments(arguments) { }
    NMethodCall(const NIdentifier& id) : id(id) { }
    virtual llvm::Value* codeGen(ICompilerEngine* context);
};

class NFunctionPrototype : public NStatement {
public:
  std::string type;
  NIdentifier id;
  VariableList arguments;
  NFunctionPrototype(const std::string& ty, NIdentifier& id, VariableList& args):
    type(ty), id(id), arguments(args) { }
  // -------------------------------------
  NFunctionPrototype(NIdentifier& ty, NIdentifier& id, VariableList& args):
    NFunctionPrototype(ty.name, id, args) { }
  virtual llvm::Value* codeGen(ICompilerEngine* context);
};

class NFunctionDeclaration : public NFunctionPrototype {
public:
  NBlock& block;
  NFunctionDeclaration(const std::string& ty, NIdentifier id, VariableList& args, NBlock& block):
    NFunctionPrototype(ty, id, args), block(block) { }
  // -------------------------------------
  NFunctionDeclaration(NIdentifier& ty, NIdentifier& id, VariableList& args, NBlock& block) :
    NFunctionDeclaration(ty.name, id, args, block) { }
  virtual llvm::Value* codeGen(ICompilerEngine* context);
};

#endif // FUNCTION_AST_NODE_HPP
