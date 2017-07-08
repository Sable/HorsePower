#include <vector>
#include <sstream>
#include "../grammar/HorseIRParser.h"

#include "../Operand.h"

using namespace horseIR::ast ;

ComplexLiteral::ComplexLiteral(HorseIRParser::LiteralCaseComplexContext *cst, ASTNode::MemManagerType &mem)
    : Literal(cst, mem, Literal::LiteralClass::ComplexLiteral, ASTNode::ASTNodeClass::ComplexLiteral)
{
    throw std::runtime_error("not yet implement") ;
}

ComplexLiteral::ComplexLiteral(ASTNode::MemManagerType &mem)
    : Literal(mem, Literal::LiteralClass::ComplexLiteral, ASTNode::ASTNodeClass::ComplexLiteral),
      type(nullptr)
{}

horseIR::ast::Type* ComplexLiteral::getLiteralType() const
{
    return type ;
}

ComplexLiteral::InternalType ComplexLiteral::getValue() const
{
    return value ;
}

std::size_t ComplexLiteral::getNumNodesRecursively() const
{
    if (type != nullptr) {
        return type->getNumNodesRecursively() + 1 ;
    } else {
        return 1 ;
    }
}

std::vector<ASTNode*> ComplexLiteral::getChildren() const
{
    if (type != nullptr) {
        return std::vector<ASTNode*> {type} ;
    } else {
        return std::vector<ASTNode*> {} ;
    }
}

std::string ComplexLiteral::toString() const
{
    std::ostringstream stream ;
    stream << value.real()
           << ((value.imag() < 0)? " " : " +")
           << value.imag()
           << 'i' ;
    if (type != nullptr) {
        stream << " :" << type->toString() ;
    } else {
        stream << " :nullptr" ;
    }
    return stream.str() ;
}

std::string ComplexLiteral::toTreeString() const
{
    return "(ComplexLiteral " + ((type != nullptr)? type->toTreeString(): "nullptr") + ")" ;
}