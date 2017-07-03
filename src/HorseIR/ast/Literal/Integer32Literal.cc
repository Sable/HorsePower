#include <vector>
#include <sstream>
#include <stdexcept>
#include <limits>
#include "../grammar/HorseIRParser.h"

#include "../Literal.h"

using namespace horseIR::ast ;

Integer32Literal::Integer32Literal(HorseIRParser::LiteralCaseIntegerContext *cst, ASTNode::MemManagerType &mem)
    : IntegerLiteral(cst, mem, ASTNode::ASTNodeType::Integer32Literal, IntegerLiteralClass::Integer32Literal)
{
    throw std::runtime_error("not yet implement") ;
}

Integer32Literal::Integer32Literal(ASTNode::MemManagerType &mem)
    : IntegerLiteral(mem, ASTNode::ASTNodeType::Integer32Literal, IntegerLiteralClass::Integer32Literal),
      value(0)
{}

horseIR::ast::Type* Integer32Literal::getLiteralType() const
{
    return type ;
}

std::int8_t Integer32Literal::getInt8Value() const
{
    if (value > std::numeric_limits<std::int8_t>::max()) throw std::overflow_error("literal casting overflow") ;
    if (value < std::numeric_limits<std::int8_t>::min()) throw std::underflow_error("literal casting underflow") ;
    return static_cast<std::int8_t>(value) ;
}

std::int16_t Integer32Literal::getInt16Value() const
{
    if (value > std::numeric_limits<std::int16_t>::max()) throw std::overflow_error("literal casting overflow") ;
    if (value < std::numeric_limits<std::int16_t>::min()) throw std::underflow_error("literal casting underflow") ;
    return static_cast<std::int16_t>(value) ;
}

std::int32_t Integer32Literal::getInt32Value() const
{
    return static_cast<std::int32_t>(value) ;
}

std::int64_t Integer64Literal::getInt64Value() const
{
    return static_cast<std::int64_t>(value) ;
}

Integer32Literal::InternalType Integer32Literal::getValue() const
{
    return value ;
}

std::size_t Integer32Literal::getNumNodesRecursively() const
{
    if (type != nullptr) {
        return type->getNumNodesRecursively() + 1 ;
    } else {
        return 1 ;
    }
}

std::vector<ASTNode*> Integer32Literal::getChildren() const
{
    if (type != nullptr) {
        return std::vector<ASTNode*> {type} ;
    } else {
        return std::vector<ASTNode*> {} ;
    }
}

std::string Integer32Literal::toString() const
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

std::string Integer32Literal::toTreeString() const
{
    return "(Integer32Literal " + ((type != nullptr)? type->toTreeString(): "nullptr") + ")" ;
}