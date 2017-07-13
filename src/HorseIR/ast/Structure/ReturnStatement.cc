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

void ReturnStatement::__duplicateShallow(const ReturnStatement *returnStmt)
{
    assert(returnStmt != nullptr) ;
    Statement::__duplicateShallow(returnStmt) ;
    id = returnStmt->id ;
    return ;
}

void ReturnStatement::__duplicateDeep(const ReturnStatement *returnStmt, ASTNode::MemManagerType &mem)
{
    assert(returnStmt != nullptr) ;
    Statement::__duplicateDeep(returnStmt, mem) ;
    Identifier* duplicateID = nullptr ;
    if (returnStmt->id != nullptr) {
        duplicateID = static_cast<Identifier*>(returnStmt->id->duplicateDeep(mem)) ;
        (void) duplicateID->setParentASTNode(this) ;
    }
    id = duplicateID ;
    return ;
}