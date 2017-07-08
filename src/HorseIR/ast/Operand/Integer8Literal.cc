#include <vector>
#include <sstream>
#include "../grammar/HorseIRParser.h"

#include "../Operand.h"

using namespace horseIR::ast ;

Integer8Literal::Integer8Literal(HorseIRParser::LiteralCaseIntegerContext *cst, ASTNode::MemManagerType &mem)
    : IntegerLiteral(cst, mem, ASTNode::ASTNodeClass::Integer8Literal, IntegerLiteralClass::Integer8Literal)
{
    throw std::runtime_error("not yet implement") ;
}

Integer8Literal::Integer8Literal(ASTNode::MemManagerType &mem)
    : IntegerLiteral(mem, ASTNode::ASTNodeClass::Integer8Literal, IntegerLiteralClass::Integer8Literal)
{}

horseIR::ast::Type* Integer8Literal::getLiteralType() const
{
    return type ;
}

std::int8_t Integer8Literal::getInt8Value() const
{
    return static_cast<std::int8_t>(value) ;
}

std::int16_t Integer8Literal::getInt16Value() const
{
    return static_cast<std::int16_t>(value) ;
}

std::int32_t Integer8Literal::getInt32Value() const
{
    return static_cast<std::int32_t>(value) ;
}

std::int64_t Integer8Literal::getInt64Value() const
{
    return static_cast<std::int64_t>(value) ;
}

Integer8Literal::InternalType Integer8Literal::getValue() const
{
    return value ;
}

std::size_t Integer8Literal::getNumNodesRecursively() const
{
    if (type != nullptr) {
        return type->getNumNodesRecursively() + 1 ;
    } else {
        return 1 ;
    }
}

std::vector<ASTNode*> Integer8Literal::getChildren() const
{
    if (type != nullptr) {
        return std::vector<ASTNode*> {type} ;
    } else {
        return std::vector<ASTNode*> {} ;
    }
}

std::string Integer8Literal::toString() const
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

std::string Integer8Literal::toTreeString() const
{
    return "(Integer8Literal " + ((type != nullptr)? type->toTreeString(): "nullptr") + ")" ;
}