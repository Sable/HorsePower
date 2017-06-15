#include <string>
#include <cassert>

#include "Type.h"

using WildcardType = horseIR::ast::WildcardType ;
using ASTNodeMemory = horseIR::ast::ASTNodeMemory ;
using Type = horseIR::ast::Type ;

WildcardType::WildcardType(HorseIRParser::TypeCaseWildcardContext* cst, ASTNodeMemory& mem)
{
    (void) mem;
    assert(cst != nullptr) ;
    this->cst = static_cast<decltype(this->cst)>(cst) ;
}

Type::TypeClass WildcardType::getTypeClass() const
{
    return Type::TypeClass::Wildcard ;
}

bool WildcardType::isGeneralizationOf(horseIR::ast::Type *type) const
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
