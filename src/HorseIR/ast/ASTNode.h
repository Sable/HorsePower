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
        class ASTNode {
        public:
            class MemManagerType {
            public:
                MemManagerType& manage(ASTNode* ptr) ;
                MemManagerType& release(ASTNode* ptr) ;
            private:
                std::vector<std::unique_ptr<ASTNode>> pool ;
            };
            
            ASTNode () = delete ;
            ASTNode (MemManagerType& mem) ;
            ASTNode (const antlr4::tree::ParseTree* cst, MemManagerType& mem) ;
            
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
    }
}
