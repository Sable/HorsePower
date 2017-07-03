#include "ASTNode.h"

using namespace horseIR::ast ;

ASTNode::ASTNode(ASTNode::MemManagerType& mem, ASTNode::ASTNodeType type)
    : ASTNode(nullptr, mem, type)
{}

ASTNode::ASTNode(const antlr4::tree::ParseTree* pTree, ASTNode::MemManagerType& mem, ASTNode::ASTNodeType type)
    : cst(pTree),
      nodeType(type) 
{
    mem.manage(this) ;
}

const antlr4::tree::ParseTree* ASTNode::getCST() const
{
    return cst ;
}

ASTNode::ASTNodeType ASTNode::getNodeType() const
{
    return nodeType ;
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
        std::cout << "node successfully allocated with address: 0x"
                  << std::hex
                  << (std::intptr_t) ptr << std::endl ;
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
