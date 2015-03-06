#include "NewNode.hpp"
#include "FunctionNode.hpp"
llvm::Value* NewNode::codeGen(ICompilerEngine* ctxt)
{
    // figure out the size of the type
    auto mc = new NMethodCall(std::string("malloc"), {new NInteger(ctxt->SizeOfType(Type()))});
    auto malloc_result = mc->codeGen(ctxt);
    delete mc;
    return ctxt->builder.CreateBitCast(malloc_result, ctxt->TypeOf(Type())->getPointerTo());
}
const std::string& NewNode::Type() const
{
    return type;
}
