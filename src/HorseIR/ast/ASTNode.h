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
        class ASTVisitor ;

        class ASTNode {
        public:
            class MemManagerType {
            public:
                MemManagerType& manage(ASTNode* ptr) ;
                MemManagerType& release(ASTNode* ptr) ;
            private:
                std::vector<std::unique_ptr<ASTNode>> pool ;
            } ;

            enum class ASTNodeClass {
                NilLiteral, ComplexLiteral, BoolLiteral, CharLiteral, Integer8Literal,
                Integer16Literal, Integer32Literal, Integer64Literal, FP32Literal,
                FP64Literal, SymbolLiteral, TimeMonthLiteral, TimeDateLiteral, TimeDateTimeLiteral,
                TimeMinuteLiteral, TimeSecondLiteral, TimeTimeLiteral, FunctionLiteral, TableLiteral,
                KeyTableLiteral, StringLiteral,

                Identifier,

                ScalarType, WildcardType, ListType, DictionaryType,
                EnumerationType, FunctionType,

                CompilationUnit, Module, Method, LabelStatement, BranchStatement, PhiStatement,
                AssignStatement, ReturnStatement
            } ;
            
            ASTNode () = delete ;
            ASTNode (MemManagerType& mem, const ASTNode::ASTNodeClass type) ;
            ASTNode (ASTNode* p_parentASTNode, const antlr4::tree::ParseTree* cst, MemManagerType& mem, const ASTNode::ASTNodeClass type) ;
            virtual ~ASTNode() = default ;

            virtual std::size_t getNumNodesRecursively() const = 0;
            virtual std::vector<ASTNode*> getChildren() const = 0 ;
            virtual ASTNode* duplicateShallow(ASTNode::MemManagerType& mem) const ; /* TODO */
            virtual ASTNode* duplicateDeep(ASTNode::MemManagerType& mem) const ;    /* TODO */
            ASTNode* getParentASTNode() const ;
            ASTNode& setParentASTNode(ASTNode* p_parentASTNode) ;

            const antlr4::tree::ParseTree* getCST() const ;
            ASTNode::ASTNodeClass getNodeType() const ;

            virtual std::string toString() const = 0 ;
            virtual std::string toTreeString() const = 0 ;

        protected:
            static const std::string INDENT ;
            const antlr4::tree::ParseTree* cst ;
            const ASTNode::ASTNodeClass nodeType ;
            ASTNode* parentASTNode ;

            static std::string CSTNameToString(HorseIRParser::NameContext* nameContext) ;
        } ;
    }
}
