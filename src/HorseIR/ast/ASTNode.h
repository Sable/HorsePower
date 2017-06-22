#pragma once

#include <cstddef>
#include <vector>
#include <string>
#include <cassert>
#include <memory>
#include <algorithm>

#include "antlr4-runtime.h"
#include "../grammar/HorseIRParser.h"

namespace horseIR {
    namespace ast {
        template <typename T>
        class MemoryManager {
        public:
            virtual MemoryManager<T>& manage(T* ptr) = 0 ;
            virtual MemoryManager<T>& release(T* ptr) = 0 ;
        } ;
        
        class ASTNode {
        public:
            ASTNode () = delete ;
            ASTNode (MemoryManager<ASTNode>& mem) ;
            ASTNode (const antlr4::tree::ParseTree* cst, MemoryManager<ASTNode>& mem) ;
            
            typedef std::vector<ASTNode*>::const_iterator const_iterator ;
            
            std::size_t getNumChildren() ;
            ASTNode::const_iterator childConstBegin() ;
            ASTNode::const_iterator childConstEnd() ;
            ASTNode* getChildAt(std::size_t pos) ;

            const antlr4::tree::ParseTree* getCST() ;
            std::size_t getNumNodesRecursively() ;

            virtual std::string toString() const = 0 ;
            virtual std::string toTreeString() const = 0 ;
        protected:
            const antlr4::tree::ParseTree* cst ;
            std::vector<ASTNode*> children ;
        } ;

        ASTNode::ASTNode(MemoryManager<ASTNode>& mem) : ASTNode(nullptr, mem) {} 
        ASTNode::ASTNode(const antlr4::tree::ParseTree* pTree, MemoryManager<ASTNode>& mem)
            : cst{pTree} {
            mem.manage(this) ;
        }
        std::size_t ASTNode::getNumChildren() { return children.size() ; }
        ASTNode::const_iterator ASTNode::childConstBegin() { return children.cbegin() ; }
        ASTNode::const_iterator ASTNode::childConstEnd() { return children.cend() ; }
        const antlr4::tree::ParseTree* ASTNode::getCST() { return cst ; }
        std::size_t ASTNode::getNumNodesRecursively()
        {
            std::size_t cum = 1 ;
            for (auto ptr = children.cbegin(); ptr != children.cend(); ++ptr) {
                ASTNode* node = *ptr ;
                cum = cum + node->getNumChildren() ;
            }
            return cum ;
        }

        class ASTNodeMemory : MemoryManager<ASTNode> {
        public:
            virtual ASTNodeMemory& manage(ASTNode* ptr) override ;
            virtual ASTNodeMemory& release(ASTNode* ptr) override ;
        private:
            std::vector<std::unique_ptr<ASTNode>> pool ;
        } ;

        ASTNodeMemory& ASTNodeMemory::manage(ASTNode* ptr)
        {
            auto p = std::find_if(pool.cbegin(), pool.cend(),
                                  [=] (std::unique_ptr<ASTNode>& p_search) -> bool {
                                      return p_search.get() == ptr ;
                                  }) ;
            if (p != pool.cend()) {
                /* duplicate ignore */
            } else {
                pool.emplace_back(ptr) ;
            }
            return *this ;
        }

        ASTNodeMemory& ASTNodeMemory::release(ASTNode* ptr)
        {
            pool.erase(std::remove_if(pool.begin(), pool.end(),
                                      [=] (std::unique_ptr<ASTNode>& p_search) -> bool {
                                          return p_search.get() == ptr ;
                                      }));
            return *this ;
        }
    }
}
