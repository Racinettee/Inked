#ifndef NEWNODE_HPP_
#define NEWNODE_HPP_
#include "node.h"

class NewNode : public NStatement
{
    std::string type;
public:
    NewNode(const std::string& t): type(t) { }
    const std::string& Type() const;
    llvm::Value* codeGen(ICompilerEngine*);
};
#endif // NEWNODE_HPP_
