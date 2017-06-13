#pragma once

#include <cstddef>
#include <vector>
#include <string>
#include <cassert>
#include <memory>

#include "antlr4-runtime.h"
#include "../grammar/HorseIRParser.h"

class ASTNode {
public:
    typedef std::vector<ASTNode*>::const_iterator const_iterator ;
    
    std::size_t getNumChildren() ;
    ASTNode::const_iterator childConstBegin() ;
    ASTNode::const_iterator childConstEnd() ;
    ASTNode* getChildAt(std::size_t pos) ;

    antlr4::tree::ParseTree* getCST() ;
    std::size_t getNumNodes() ;

    virtual constexpr std::string toString() const = 0 ;
    virtual constexpr std::string toTreeString() const = 0 ;
protected:
    antlr4::tree::ParseTree* cst ;
    std::vector<ASTNode*> children ;
} ;

std::size_t ASTNode::getNumChildren() { return children.size() ; }
ASTNode::const_iterator ASTNode::childConstBegin() { return children.cbegin() ; }
ASTNode::const_iterator ASTNode::childConstEnd() { return children.cend() ; }
antlr4::tree::ParseTree* ASTNode::getCST() { return cst ; }
std::size_t ASTNode::getNumNodes()
{
    std::size_t cum = 1 ;
    for (auto ptr = children.cbegin(); ptr != children.cend(); ++ptr) {
        ASTNode* node = *ptr ;
        cum = cum + node->getNumChildren() ;
    }
    return cum ;
}

class ASTNodeMemory {
public:
    void manage(ASTNode* node) ;
private:
    std::vector<std::unique_ptr<ASTNode>> pool ;
} ;
void ASTNodeMemory::manage(ASTNode *node)
{
    for (auto ptr = pool.cbegin(); ptr != pool.cend(); ++ptr) {
        /* ignore duplicate manage requests */
        if (ptr->get() == node) return ;
    }
    pool.emplace_back(node) ;
}
