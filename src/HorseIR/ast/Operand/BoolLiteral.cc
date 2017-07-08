#include <vector>
#include <sstream>
#include "../grammar/HorseIRParser.h"

#include "../Operand.h"

using namespace horseIR::ast ;

BoolLiteral::BoolLiteral(HorseIRParser::LiteralCaseBoolContext *cst, ASTNode::MemManagerType &mem)
    : Literal(cst, mem, Literal::LiteralClass::BoolLiteral, ASTNode::ASTNodeClass::BoolLiteral)
{
    throw std::runtime_error("not yet implement") ;
}

BoolLiteral::BoolLiteral(ASTNode::MemManagerType &mem)
    : Literal(mem, Literal::LiteralClass::BoolLiteral, ASTNode::ASTNodeClass::BoolLiteral),
      type(nullptr)
{}

horseIR::ast::Type* BoolLiteral::getLiteralType() const
{
    return type ;
}

BoolLiteral::InternalType  BoolLiteral::getValue() const
{
    return value ;
}

bool BoolLiteral::getBoolValue() const
{
    return static_cast<bool>(value);
}

std::size_t BoolLiteral::getNumNodesRecursively() const
{
    if (type != nullptr) {
        return type->getNumNodesRecursively() + 1;
    } else {
        return 1 ;
    }
}

std::vector<ASTNode*> BoolLiteral::getChildren() const
{
    if (type != nullptr) {
        return std::vector<ASTNode*> {type} ;
    } else {
        return std::vector<ASTNode*> {} ;
    }
}

std::string BoolLiteral::toString() const
{
    std::ostringstream stream ;
    stream << value ;
    if (type != nullptr) {
        stream << " :" << type->toString() ;
    } else {
        stream << " :nullptr" ;
    }
    return stream.str() ;
}

std::string BoolLiteral::toTreeString() const
{
    return "(BoolLiteral " + ((type != nullptr)? type->toTreeString(): "nullptr") + ")" ;
}
