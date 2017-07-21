#include "ASTNode.h"

using namespace horseIR::ast ;

const std::string ASTNode::INDENT = "    " ;

ASTNode::ASTNode(ASTNode::MemManagerType& mem, ASTNode::ASTNodeClass type)
    : ASTNode(nullptr, nullptr, mem, type)
{}

ASTNode::ASTNode(ASTNode* p_parentASTNode, const antlr4::tree::ParseTree* pTree, ASTNode::MemManagerType& mem, ASTNode::ASTNodeClass type)
    : cst{pTree},
      nodeType{type} ,
      parentASTNode{p_parentASTNode}
{
    mem.manage(this) ;
}

ASTNode::ASTNodeClass ASTNode::getASTNodeClass() const
{
    return nodeType ;
}

ASTNode* ASTNode::getParentASTNode() const
{
    return parentASTNode ;
}

ASTNode& ASTNode::setParentASTNode(ASTNode *p_parentASTNode)
{
    parentASTNode = p_parentASTNode ;
    return *this ;
}

const antlr4::tree::ParseTree* ASTNode::getCST() const
{
    return cst ;
}

ASTNode::ASTNodeClass ASTNode::getNodeType() const
{
    return nodeType ;
}

std::string ASTNode::CSTNameToString(HorseIRParser::NameContext *nameContext)
{
    assert(nameContext != nullptr) ;
    HorseIRParser::NameIdContext* nameIdContext = nullptr ;
    HorseIRParser::NameKeyContext* nameKeyContext = nullptr ;
    if ((nameIdContext = dynamic_cast<decltype(nameIdContext)>(nameContext)) != nullptr) {
        return nameIdContext->ID()->getText() ;
    } else if ((nameKeyContext = dynamic_cast<decltype(nameKeyContext)>(nameContext)) != nullptr) {
        return nameKeyContext->idKey->getText() ;
    } else {
        assert(false);
        return std::string() ;
    }
}

/* TODO */
ASTNode* ASTNode::duplicateShallow(ASTNode::MemManagerType &mem) const
{
    return nullptr ;
}

/* TODO */
ASTNode* ASTNode::duplicateDeep(ASTNode::MemManagerType &mem) const
{
    return nullptr ;
}

ASTNode::MemManagerType& ASTNode::MemManagerType::manage(ASTNode* ptr)
{
    auto p = std::find_if(pool.begin(), pool.end(),
                          [&] (std::unique_ptr<ASTNode>& p_search) -> bool {
                              return p_search.get() == ptr ;
                          }) ;
    if (p != pool.cend()) {
        /* duplicate ignore */
    } else {
        pool.emplace_back(ptr) ;
        /*std::cout << "node successfully allocated with address: 0x"
                  << std::hex
                  << (std::intptr_t) ptr << std::endl ;
*/
    }
    return *this ;
}

ASTNode::MemManagerType& ASTNode::MemManagerType::release(ASTNode* ptr)
{
    pool.erase(std::remove_if(pool.begin(), pool.end(),
                              [&] (std::unique_ptr<ASTNode>& p_search) -> bool {
                                  return p_search.get() == ptr ;
                              }));
    return *this ;
}

void ASTNode::__duplicateShallow(const ASTNode *astNode)
{
    assert(astNode != nullptr) ;
    cst = astNode->cst ;
    nodeType = astNode->nodeType ;
    parentASTNode = nullptr ;
    return ;
}

void ASTNode::__duplicateDeep(const ASTNode *astNode, ASTNode::MemManagerType& mem)
{
    assert(astNode != nullptr) ;
    (void) mem ;
    cst = astNode->cst ;
    nodeType = astNode->nodeType ;
    parentASTNode = nullptr ;
    return ;
}
