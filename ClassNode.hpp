#ifndef CLASS_AST_NODE_HPP
#define CLASS_AST_NODE_HPP
#include "ICompilerEngine.hpp"
#include "IClass.hpp"
#include "node.h"
#include <utility>
class NClass : public NStatement, public IClass {
public:
  NClass(const std::string& n, const std::vector<NStatement*>& b):
    name(n), statements(b) { }

  NClass(const std::string& n, const std::vector<NStatement*>& b, const std::vector<NIdentifier*>& i):
    NClass(n, b)
    {
        for(auto iden : i)
        {
            inheritance_list.push_back(iden->name);
        }
    }
  virtual llvm::Value* codeGen(ICompilerEngine*);
  // --------------------------------------
  llvm::StructType* GetType() const { return type; }
  void AddMemberVar(const std::string& n, const std::string& y);
  bool HasMemberVar(const std::string& n);
  Triple* GetMemberVar(const std::string& n);
  Triple* GetMemberVar(const int index);
  const std::string& Name() { return name; }
private:
  // Here the names are associated to their index by which they may be accessed
  // as well as their llvm type
  std::map<std::string,Triple*> key;
  // Here are the elements that will be passed in when the llvm is generated
  // for this class
  std::vector<llvm::Type*> elements;
  // This points to the list of statements contained in the class body
  std::vector<NStatement*> statements;
  // ------------------------------
  // The inheritance list
  // ------------------------
  std::vector<std::string> inheritance_list;
  std::string name;
  llvm::StructType* type = nullptr;
};
#endif // CLASS_AST_NODE_HPP
