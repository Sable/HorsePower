#include <string>
#include <cassert>

#include "Type.h"

using WildcardType = horseIR::ast::WildcardType ;
using ASTNodeMemory = horseIR::ast::ASTNodeMemory ;
using Type = horseIR::ast::Type ;

WildcardType::WildcardType(HorseIRParser::TypeCaseWildcardContext* cst, MemoryManager<ASTNode>& mem)
    : Type(cst, mem, Type::TypeClass::Wildcard)
{}

WildcardType::WildcardType(MemoryManager<ASTNode>& mem)
    : Type(mem, Type::TypeClass::Wildcard)
{}

bool WildcardType::isGeneralizationOf(const horseIR::ast::Type *type) const
{
    assert(type != nullptr) ;
    return true ;
}

std::string WildcardType::toString() const
{
    return "?" ;
}

std::string WildcardType::toTreeString() const
{
    return "(WildcardType)" ;
}
