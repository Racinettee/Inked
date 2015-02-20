#include "NewNode.hpp"

llvm::Value* NewNode::codeGen(ICompilerEngine* ctxt)
{
    // figure out the size of the type

    auto mc = new NMethodCall("malloc", {});
}
