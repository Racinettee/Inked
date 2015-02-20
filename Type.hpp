#ifndef _TYPE_HPP_
#define _TYPE_HPP_
#include "ICompilerEngine.hpp"
class Type
{
    std::string name;
    llvm::Type* type;
public:
    const std::string& Name()
    {
        return name;
    }
    const llvm::Type* Type()
    {
        return type;
    }
    std::size_t SizeBytes() const
    {
        // figure out the type of type of type. if its primitive just return the size, if its a struct type
        // then it will have to be interated over
        unsigned std::size_t sizeof_type = 0;
        if(type->isFloatTy())
            sizeof_type = 4;
            // 32 bits
        else if(type->isDoubleTy())
            // 64 bits
            sizeof_type = 8;
        else if(type->isIntegerTy())
        {
            // variable width that type can report
            sizeof_type = dynamic_cast<llvm::IntegerType>(type)->getBitWidth();
        }
        else if(type->isStructTy())
        {
            // iterate over all elements and sum them
        }
    }
};
#endif // _TYPE_HPP_
