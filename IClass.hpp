#ifndef I_CLASS_HPP
#define I_CLASS_HPP
#include <string>
class IClass
{
public:
  struct Triple {
    int class_index;
    llvm::Type* type;
    // Value is actually gonna be useless probably
    llvm::Value* value;
    // ----------------
    Triple(int ind, llvm::Type* ty, llvm::Value* val):
      class_index(ind), type(ty), value(val) { }
  };
public:
  virtual const std::string& Name() = 0;
  virtual llvm::StructType* GetType() const = 0;
  virtual void AddMemberVar(const std::string& n, const std::string& t) = 0;
  virtual bool HasMemberVar(const std::string& n) = 0;
  virtual Triple* GetMemberVar(const std::string& n) = 0;
  virtual Triple* GetMemberVar(const int index) = 0;
};
#endif // I_CLASS_HPP
