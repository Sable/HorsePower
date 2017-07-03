#include <vector>
#include <sstream>
#include "../grammar/HorseIRParser.h"

#include "../Literal.h"

using namespace horseIR::ast ;

CharLiteral::CharLiteral(HorseIRParser::LiteralCaseCharContext *cst, ASTNode::MemManagerType &mem)
    : Literal(cst, mem, Literal::LiteralClass::CharLiteral, ASTNode::ASTNodeType::CharLiteral)
{
    throw std::runtime_error("not yet implement") ;
}

CharLiteral::CharLiteral(ASTNode::MemManagerType &mem)
    : Literal(mem, Literal::LiteralClass::CharLiteral, ASTNode::ASTNodeType::CharLiteral)
{}

horseIR::ast::Type* CharLiteral::getLiteralType() const
{
    return type ;
}

CharLiteral::InternalType CharLiteral::getValue() const
{
    return value ;
}

std::size_t CharLiteral::getNumNodesRecursively() const
{
    if (type != nullptr) {
        return type->getNumNodesRecursively() + 1 ;
    } else {
        return 1 ;
    }
}

std::vector<ASTNode*> CharLiteral::getChildren() const
{
    if (type != nullptr) {
        return std::vector<ASTNode*> {type} ;
    } else {
        return std::vector<ASTNode*> {} ;
    }
}

std::string CharLiteral::toString() const
{
    std::ostringstream stream ;
    stream << '\'' << value << '\'' ;
    if (type != nullptr) {
        stream << " :" << type->toString() ;
    } else {
        stream << " :" << "nullptr" ;
    }
    return stream.str() ;
}

std::string CharLiteral::toTreeString() const
{
    return "(CharLiteral " + ((type != nullptr)? type->toTreeString(): "nullptr") + ")" ;
}