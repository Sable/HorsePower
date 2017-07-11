#include <vector>
#include <cassert>
#include <string>

#include "../AST.h"

using namespace horseIR::ast ;

ReturnStatement::ReturnStatement(ASTNode* parent, HorseIRParser::StmtCoreContext *cst, ASTNode::MemManagerType &mem)
    : Statement(parent, cst, mem, ASTNode::ASTNodeClass::ReturnStatement, StatementClass::Return)
{
    assert(cst != nullptr) ;
    assert(cst->statementCore() != nullptr) ;
    auto returnContext = dynamic_cast<HorseIRParser::StmtReturnContext*>(cst->statementCore()) ;
    assert(returnContext != nullptr) ;

    id = new Identifier(this, returnContext->name(), mem) ;
}

ReturnStatement::ReturnStatement(ASTNode::MemManagerType &mem)
    : Statement(mem, ASTNode::ASTNodeClass::ReturnStatement, StatementClass::Return),
      id{nullptr}
{}

std::size_t ReturnStatement::getNumNodesRecursively() const
{
    return (id != nullptr)? id->getNumNodesRecursively() + 1 : 1 ;
}

std::vector<ASTNode*> ReturnStatement::getChildren() const
{
    return (id != nullptr)? std::vector<ASTNode*>{id} : std::vector<ASTNode*>{} ;
}

std::string ReturnStatement::toString() const
{
    return "return " + id->toString() + " ;" ;
}

std::string ReturnStatement::toTreeString() const
{
    return "(ReturnStatement)" ;
}

ReturnStatement* ReturnStatement::duplicateShallow(ASTNode::MemManagerType &mem) const
{
    ReturnStatement* replica = new ReturnStatement(mem) ;
    replica->id = id ;
    return replica ;
}

ReturnStatement* ReturnStatement::duplicateDeep(ASTNode::MemManagerType &mem) const
{
    ReturnStatement* replica = new ReturnStatement(mem) ;
    Identifier* duplicateIdentifier = nullptr ;
    if (id != nullptr) {
        duplicateIdentifier = static_cast<Identifier*>(id->duplicateDeep(mem)) ;
        (void) duplicateIdentifier->setParentASTNode(replica) ;
    }
    replica->id = duplicateIdentifier ;
    return replica ;
}
