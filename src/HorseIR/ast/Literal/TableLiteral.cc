#include <vector>
#include <sstream>
#include <cassert>
#include "../grammar/HorseIRParser.h"

#include "../Literal.h"

using namespace horseIR::ast ;

TableLiteral::TableLiteral(HorseIRParser::LiteralCaseTableContext *cst, ASTNode::MemManagerType &mem)
    : Literal(cst, mem, Literal::LiteralClass::TableLiteral, ASTNode::ASTNodeClass::TableLiteral),
      type(new ScalarType(ScalarType::ScalarClass::Table, mem))

{
    assert(cst != nullptr) ;
    HorseIRParser::LiteralTableContext* context = cst->literalTable() ;
    value = context->value->getText() ;
}

TableLiteral::TableLiteral(ASTNode::MemManagerType &mem)
    : Literal(mem, Literal::LiteralClass::TableLiteral, ASTNode::ASTNodeClass::TableLiteral),
      type(new ScalarType(ScalarType::ScalarClass::Table, mem)),
      value()
{}

horseIR::ast::Type* TableLiteral::getLiteralType() const
{
    return type ;
}

TableLiteral::InternalType TableLiteral::getValue() const
{
    return value ;
}

std::size_t TableLiteral::getNumNodesRecursively() const
{
    if (type != nullptr) {
        return type->getNumNodesRecursively() + 1 ;
    } else {
        return 1 ;
    }
}

std::vector<ASTNode*> TableLiteral::getChildren() const
{
    if (type != nullptr) {
        return std::vector<ASTNode*> {type} ;
    } else {
        return std::vector<ASTNode*> {} ;
    }
}

std::string TableLiteral::toString() const
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

std::string TableLiteral::toTreeString() const
{
    return "(TableLiteral " + ((type != nullptr)? type->toTreeString(): "nullptr") + ")" ;
}