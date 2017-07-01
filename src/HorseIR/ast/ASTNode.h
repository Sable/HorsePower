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
            } ;

            enum class ASTNodeType {
                NilLiteral, ComplexLiteral, BoolLiteral, CharLiteral, Integer8Literal,
                Integer16Literal, Integer32Literal, Integer64Literal, FP32Literal,
                FP64Literal, SymbolLiteral, TimeMonthLiteral, TimeDateLiteral, TimeDateTimeLiteral,
                TimeMinuteLiteral, TimeSecondLiteral, TimeTimeLiteral, FunctionLiteral, TableLiteral,
                KeyTableLiteral, StringLiteral,

                ScalarType, WildcardType, ListType, DictionaryType,
                EnumerationType, FunctionType
            } ;
            
            ASTNode () = delete ;
            ASTNode (MemManagerType& mem, const ASTNode::ASTNodeType type) ;
            ASTNode (const antlr4::tree::ParseTree* cst, MemManagerType& mem, const ASTNode::ASTNodeType type) ;

            virtual std::size_t getNumNodesRecursively() const = 0;
            virtual std::vector<ASTNode*> getChildren() const = 0 ;

            const antlr4::tree::ParseTree* getCST() const ;
            ASTNode::ASTNodeType getNodeType() const ;

            virtual std::string toString() const = 0 ;
            virtual std::string toTreeString() const = 0 ;
        protected:
            const antlr4::tree::ParseTree* cst ;
            const ASTNode::ASTNodeType nodeType ;
        } ;
    }
}
