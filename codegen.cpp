#include "node.h"
#include "ICompilerEngine.hpp"
#include "parser.parser.hh"

using namespace std;
using namespace llvm;

/* -- Code Generation -- */

Value* NInteger::codeGen(ICompilerEngine* context)
{
	return ConstantInt::get(Type::getInt64Ty(getGlobalContext()), value, true);
}

Value* NDouble::codeGen(ICompilerEngine* context)
{
	return ConstantFP::get(Type::getDoubleTy(getGlobalContext()), value);
}

Value* NString::codeGen(ICompilerEngine* context)
{
	return context->builder.CreateGlobalStringPtr(value);
}

Value* NIdentifier::codeGen(ICompilerEngine* context)
{
	std::cout << "Creating identifier reference: " << name << std::endl;
  auto current_block = context->CurrentBBlock();
	auto sym_table = context->CurrentValSymTable();
	auto val = sym_table->lookup(name);

  if(val == nullptr){
    std::cerr << "undeclared variable " << name << std::endl;
    return NULL;
  }
  return context->builder.CreateLoad(val);
}

Value* NBinaryOperator::codeGen(ICompilerEngine* context)
{
	std::cout << "Creating binary operation " << op << std::endl;
	Instruction::BinaryOps instr;
	switch (op) {
		case TPLUS: 	instr = Instruction::Add; goto math;
		case TMINUS: 	instr = Instruction::Sub; goto math;
		case TMUL: 		instr = Instruction::Mul; goto math;
		case TDIV: 		instr = Instruction::SDiv; goto math;

		/* TODO comparison */
	}

	return NULL;
math:
	return BinaryOperator::Create(instr, lhs.codeGen(context),
		rhs.codeGen(context), "", context->CurrentBBlock());
}

Value* NAssignment::codeGen(ICompilerEngine* context)
{
	//std::cout << "Creating assignment for " << lhs.name << std::endl;
	auto current_block = context->CurrentBBlock();
	auto sym_table = current_block->getValueSymbolTable();
	auto val = sym_table->lookup(lhs.name);
	if(val == nullptr){
    auto paren = current_block->getParent();
    sym_table = &paren->getValueSymbolTable();
    val = sym_table->lookup(lhs.name);
  }
  if(val == nullptr){
    std::cerr << "undeclared variable " << lhs.name << std::endl;
    return NULL;
  }
	return new StoreInst(rhs.codeGen(context),
      context->CurrentValSymTable()->lookup(lhs.name), false, context->CurrentBBlock());
}

Value* NBlock::codeGen(ICompilerEngine* context)
{
	StatementList::const_iterator it;
	Value *last = NULL;
	for (it = statements.begin(); it != statements.end(); it++) {
		std::cout <<
      "Generating code for " << typeid(**it).name() << std::endl;
		last = (**it).codeGen(context);
	}
	std::cout << "Creating block" << std::endl;
	return last;
}

Value* NExpressionStatement::codeGen(ICompilerEngine* context)
{
	std::cout << "Generating code for " << typeid(expression).name() << std::endl;
	return expression->codeGen(context);
}
