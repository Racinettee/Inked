#include "ICompilerEngine.hpp"
#include "ClassNode.hpp"

Value* NClass::codeGen(ICompilerEngine* ctxt)
{
  printf("Class def node!!! %s\n", name.c_str());
  // Push this so that variable declarations can check to see
  // if they are declared inside this class
  ctxt->PushClass(this);
  //std::vector<Type*> elements({Type::getInt32Ty(getGlobalContext()), Type::getInt64Ty(getGlobalContext())});
  // Create the llvm struct type and then add it to the engines pool of classes
  for(auto stmt : statements)
  {
    // The variables among this list of statements
    // should be able to tell that their in a class def
    // and add themselves to this class
    stmt->codeGen(ctxt);
  }

  auto str = StructType::create(getGlobalContext(), elements,name);
  ctxt->AddType(Name(),str);
  ctxt->PopClass();
  return nullptr;//return ctxt->CurrentModule()->getOrInsertGlobal (name, str);
}

void NClass::AddMemberVar(const std::string& n, const std::string& t)
{
  if(HasMemberVar(n))
  {
    printf("Warning! Variable: %s redeclaration ignored!\n",n.c_str());
    return;
  }
  auto ty = ICompilerEngine::engine->TypeOf(t);
  elements.push_back(ty);
  int index = elements.size()-1;
  auto trip = new Triple(index, ty, nullptr);
  key[n] = trip;
}
bool NClass::HasMemberVar(const std::string& n)
{
  if(key.find(n) != key.end())
    return true;
  return false;
}
IClass::Triple* NClass::GetMemberVar(const std::string& n)
{
  if(HasMemberVar(n))
    return key[n];
  return nullptr;
}
IClass::Triple* NClass::GetMemberVar(const int index)
{
  for(auto v = key.begin(); v != key.end(); ++v)
    if(v->second->class_index == index)
      return v->second;
  return nullptr;
}
