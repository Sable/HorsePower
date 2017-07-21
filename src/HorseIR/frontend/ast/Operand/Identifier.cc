#include <vector>
#include <string>
#include <sstream>

#include "../AST.h"

using namespace horseIR::ast ;

const std::string Identifier::LOCAL_NAME = "<local>" ;

Identifier::Identifier(ASTNode* parent, HorseIRParser::GeneralNameContext *cst, ASTNode::MemManagerType &mem)
    : Operand(parent, cst, mem, ASTNode::ASTNodeClass::Identifier, OperandClass::Identifier)
{
    assert(cst != nullptr) ;
    if (cst->name() != nullptr) {
        packageName = Identifier::LOCAL_NAME ;
        idName = ASTNode::CSTNameToString(cst->name()) ;
    } else if (cst->compoundName() != nullptr) {
        HorseIRParser::CompoundNameContext* context = cst->compoundName() ;
        std::istringstream compoundNameStream (context->COMPOUND_ID()->getText()) ;
        std::vector<std::string> nameTokens ;
        std::string tokenBuffer ;
        while(std::getline(compoundNameStream, tokenBuffer, '.')) {
            nameTokens.push_back(std::move(tokenBuffer)) ;
        }
        assert(nameTokens.size() == 2) ;
        packageName = std::move(nameTokens[0]) ;
        idName = std::move(nameTokens[1]) ;
    } else {
        assert(false) ;
    }
}

Identifier::Identifier(ASTNode* parent, HorseIRParser::NameContext *cst, ASTNode::MemManagerType &mem)
    : Operand(parent, cst, mem, ASTNode::ASTNodeClass::Identifier, OperandClass::Identifier)
{
    assert(cst != nullptr) ;
    packageName = Identifier::LOCAL_NAME ;
    idName = ASTNode::CSTNameToString(cst) ;
}

Identifier::Identifier(ASTNode::MemManagerType &mem)
    : Operand(mem, ASTNode::ASTNodeClass::Identifier, OperandClass::Identifier)
{}

std::string Identifier::getFullName() const
{
    return packageName + "." + idName ;
}

std::string Identifier::getIDName() const
{
    return idName ;
}

std::string Identifier::getPackageName() const
{
    return packageName ;
}

Identifier& Identifier::setIDName(const std::string &p_idName)
{
    idName = p_idName ;
    return *this ;
}

Identifier& Identifier::setPackageName(const std::string &p_packageName)
{
    packageName = p_packageName ;
    return *this ;
}

std::size_t Identifier::getNumNodesRecursively() const
{
    return 1 ;
}

std::vector<ASTNode*> Identifier::getChildren() const
{
    return std::vector<ASTNode*>{} ;
}

std::string Identifier::toString() const
{
    if (packageName == Identifier::LOCAL_NAME) {
        return idName ;
    } else {
        return getFullName() ;
    }
}

std::string Identifier::toTreeString() const
{
    return "(Identifier)" ;
}
