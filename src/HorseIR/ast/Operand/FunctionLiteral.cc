#include <vector>
#include <sstream>
#include <cassert>

#include "../AST.h"

using namespace horseIR::ast ;

FunctionLiteral::FunctionLiteral(ASTNode* parent, HorseIRParser::LiteralCaseFunctionContext *cst, ASTNode::MemManagerType &mem)
    : Literal(parent, cst, mem, Literal::LiteralClass::FunctionLiteral, ASTNode::ASTNodeClass::FunctionLiteral),
      type{nullptr}
{
    assert(cst != nullptr) ;

    HorseIRParser::LiteralFunctionContext* context = cst->literalFunction() ;
    const std::string lex = context->value->getText() ;
    value = lex.substr(1, lex.length()) ;
    FunctionType* functionType = new FunctionType(mem) ;

}

FunctionLiteral::FunctionLiteral(ASTNode* parent, HorseIRParser::LiteralFunctionContext *cst, ASTNode::MemManagerType &mem)
    : Literal(parent, cst, mem, Literal::LiteralClass::FunctionLiteral, ASTNode::ASTNodeClass::FunctionLiteral),
      type{nullptr}
{
    assert(cst != nullptr) ;
    const std::string lex = cst->value->getText() ;
    value = lex.substr(1, lex.length()) ;
}

FunctionLiteral::FunctionLiteral(ASTNode::MemManagerType &mem)
    : Literal(mem, Literal::LiteralClass::FunctionLiteral, ASTNode::ASTNodeClass::FunctionLiteral),
      type(nullptr)
{}

horseIR::ast::Type* FunctionLiteral::getLiteralType() const
{
    return type ;
}

FunctionLiteral::InternalType FunctionLiteral::getValue() const
{
    return value ;
}

std::size_t FunctionLiteral::getNumNodesRecursively() const
{
    if (type != nullptr) {
        return type->getNumNodesRecursively() + 1 ;
    } else {
        return 1 ;
    }
}

std::vector<ASTNode*> FunctionLiteral::getChildren() const
{
    if (type != nullptr) {
        return std::vector<ASTNode*> {type} ;
    } else {
        return std::vector<ASTNode*> {} ;
    }
}

std::string FunctionLiteral::toString() const
{
    std::ostringstream stream ;
    stream << '@' << value ;
    if (type != nullptr) {
        stream << " :" << type->toString() ;
    } else {
        stream << " :nullptr" ;
    }
    return stream.str() ;
}

std::string FunctionLiteral::toTreeString() const
{
    return "(FunctionLiteral " + ((type != nullptr)? type->toTreeString(): "nullptr") + ")" ;
}
