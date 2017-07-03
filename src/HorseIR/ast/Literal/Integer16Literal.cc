#include <vector>
#include <sstream>
#include <stdexcept>
#include <limits>
#include "../grammar/HorseIRParser.h"

#include "../Literal.h"

using namespace horseIR::ast ;

Integer16Literal::Integer16Literal(HorseIRParser::LiteralCaseIntegerContext *cst, ASTNode::MemManagerType &mem)
    : IntegerLiteral(cst, mem, ASTNode::ASTNodeType::Integer16Literal, IntegerLiteralClass::Integer16Literal)
{}

Integer16Literal::Integer16Literal(ASTNode::MemManagerType &mem)
    : IntegerLiteral(mem, ASTNode::ASTNodeType::Integer16Literal, IntegerLiteralClass::Integer16Literal),
      value(0)
{}

horseIR::ast::Type* Integer16Literal::getLiteralType() const
{
    return type ;
}

std::int8_t Integer16Literal::getInt8Value() const
{
    if (value > std::numeric_limits<std::int8_t>::max()) throw std::overflow_error("literal casting overflow") ;
    if (value < std::numeric_limits<std::int8_t>::min()) throw std::underflow_error("literal casting underflow") ;
    return static_cast<std::int8_t>(value) ;
}

std::int16_t Integer16Literal::getInt16Value() const
{
    return static_cast<std::int16_t>(value) ;
}

std::int32_t Integer16Literal::getInt32Value() const
{
    return static_cast<std::int32_t>(value) ;
}

std::int64_t Integer16Literal::getInt64Value() const
{
    return static_cast<std::int64_t>(value) ;
}

Integer16Literal::InternalType Integer16Literal::getValue() const
{
    return value ;
}

std::size_t Integer16Literal::getNumNodesRecursively() const
{
    if (type != nullptr) {
        return type->getNumNodesRecursively() + 1 ;
    } else {
        return 1 ;
    }
}

std::vector<ASTNode*> Integer16Literal::getChildren() const
{
    if (type != nullptr) {
        return std::vector<ASTNode*> {type} ;
    } else {
        return std::vector<ASTNode*> {} ;
    }
}

std::string Integer16Literal::toString() const
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

std::string Integer16Literal::toTreeString() const
{
    return "(Integer16Literal " + ((type != nullptr)? type->toTreeString(): "nullptr") + ")" ;
}