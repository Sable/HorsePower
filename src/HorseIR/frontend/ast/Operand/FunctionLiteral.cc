#include <vector>
#include <sstream>
#include <cassert>
#include <string>

#include "../AST.h"

using namespace horseIR::ast ;

const std::string FunctionLiteral::UNDEFINED_PACKAGE = "[unknown]" ;

FunctionLiteral::FunctionLiteral(ASTNode* parent, HorseIRParser::LiteralCaseFunctionContext *cst, ASTNode::MemManagerType &mem)
    : Literal(parent, cst, mem, Literal::LiteralClass::FunctionLiteral, ASTNode::ASTNodeClass::FunctionLiteral),
      type{nullptr}
{
    assert(cst != nullptr) ;

    HorseIRParser::LiteralFunctionContext* context = cst->literalFunction() ;
    std::string const lex = context->value->getText() ;
    std::string const rawName = lex.substr(1, lex.length()) ;
    std::string::size_type const dotPos = rawName.find(".") ;
    if (dotPos == std::string::npos) {
        std::string moduleName = UNDEFINED_PACKAGE ;
        value = std::make_pair(std::move(moduleName), std::move(rawName)) ;
    } else {
        std::string moduleName = rawName.substr(0, dotPos) ;
        std::string methodName = rawName.substr(dotPos + 1, rawName.size()) ;
        value = std::make_pair(std::move(moduleName), std::move(methodName)) ;
    }
}

FunctionLiteral::FunctionLiteral(ASTNode* parent, HorseIRParser::LiteralFunctionContext *cst, ASTNode::MemManagerType &mem)
    : Literal(parent, cst, mem, Literal::LiteralClass::FunctionLiteral, ASTNode::ASTNodeClass::FunctionLiteral),
      type{nullptr}
{
    assert(cst != nullptr) ;
    std::string const lex = cst->value->getText() ;
    std::string const rawName = lex.substr(1, lex.length()) ;
    std::string::size_type const dotPos = rawName.find(".") ;
    if (dotPos == std::string::npos) {
        std::string moduleName = UNDEFINED_PACKAGE ;
        value = std::make_pair(std::move(moduleName), std::move(rawName)) ;
    } else {
        std::string moduleName = rawName.substr(0, dotPos) ;
        std::string methodName = rawName.substr(dotPos + 1, rawName.size()) ;
        value = std::make_pair(std::move(moduleName), std::move(methodName)) ;
    }
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

FunctionLiteral::InternalType::first_type FunctionLiteral::getModuleName() const
{
    return value.first ;
}

FunctionLiteral::InternalType::second_type FunctionLiteral::getFunctionName() const
{
    return value.second ;
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
    stream << '@' ;
    if (value.first == UNDEFINED_PACKAGE) {
        stream << value.second ;
    } else {
        stream << value.first << '.' << value.second ;
    }
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
