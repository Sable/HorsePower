#include <vector>
#include <sstream>
#include <stdexcept>
#include <limits>

#include "../AST.h"

using namespace horseIR::ast ;

Integer64Literal::Integer64Literal(ASTNode* parent, HorseIRParser::LiteralCaseIntegerContext *cst, ASTNode::MemManagerType &mem)
    : IntegerLiteral(parent, cst, mem, ASTNode::ASTNodeClass::Integer64Literal, IntegerLiteralClass::Integer64Literal)
{
    throw std::runtime_error("not yet implement") ;
}

Integer64Literal::Integer64Literal(ASTNode::MemManagerType &mem)
    : IntegerLiteral(mem, ASTNode::ASTNodeClass::Integer64Literal, IntegerLiteralClass::Integer64Literal)
{}

horseIR::ast::Type* Integer64Literal::getLiteralType() const
{
    return type ;
}

std::int8_t Integer64Literal::getInt8Value() const
{
    if (value > std::numeric_limits<std::int8_t>::max()) throw std::overflow_error("literal casting overflow") ;
    if (value < std::numeric_limits<std::int8_t>::min()) throw std::underflow_error("literal casting underflow") ;
    return static_cast<std::int8_t>(value) ;
}

std::int16_t Integer64Literal::getInt16Value() const
{
    if (value > std::numeric_limits<std::int16_t>::max()) throw std::overflow_error("literal casting overflow") ;
    if (value < std::numeric_limits<std::int16_t>::min()) throw std::underflow_error("literal casting underflow") ;
    return static_cast<std::int16_t>(value) ;
}

std::int32_t Integer64Literal::getInt32Value() const
{
    if (value > std::numeric_limits<std::int32_t>::max()) throw std::overflow_error("literal casting overflow") ;
    if (value < std::numeric_limits<std::int32_t>::min()) throw std::underflow_error("literal casting underflow") ;
    return static_cast<std::int32_t>(value) ;
}

std::int64_t Integer64Literal::getInt64Value() const
{
    return static_cast<std::int64_t>(value);
}

Integer64Literal::InternalType Integer64Literal::getValue() const
{
    return value ;
}

std::size_t Integer64Literal::getNumNodesRecursively() const
{
    if (type != nullptr) {
        return type->getNumNodesRecursively() + 1 ;
    } else {
        return 1 ;
    }
}

std::vector<ASTNode*> Integer64Literal::getChildren() const
{
    if (type != nullptr) {
        return std::vector<ASTNode*> {type} ;
    } else {
        return std::vector<ASTNode*> {} ;
    }
}

std::string Integer64Literal::toString() const
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

std::string Integer64Literal::toTreeString() const
{
    return "(Integer64Literal " + ((type != nullptr)? type->toTreeString(): "nullptr") + ")" ;
}
