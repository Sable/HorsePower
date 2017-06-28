#pragma once

#include <string>
#include <complex>
#include <cstdint>

#include "ASTNode.h"
#include "Type.h"
#include "antlr4-runtime.h"
#include "../grammar/HorseIRParser.h"

namespace horseIR {
    namespace ast {
        class Literal : public ASTNode {
        public:
            enum class LiteralClass {

            } ;
            Literal() = delete ;
            Literal(antlr4::tree::ParseTree* cst, ASTNode::MemManagerType& mem, Literal::LiteralClass, ASTNode::ASTNodeType type) ;
            Literal(ASTNode::MemManagerType& mem, Literal::LiteralClass p_literalClass, ASTNode::ASTNodeType type) ;

            constexpr Literal::LiteralClass getLiteralClass() const ;
            virtual horseIR::ast::Type* getLiteralType() const = 0;

            static Literal* makeLiteralASTNode(HorseIRParser::LiteralContext* cst, ASTNode::MemManagerType& mem) ;
        protected:
            const Literal::LiteralClass literalClass ;
        } ;

        class NilLiteral : public Literal {
        public:
            typedef std::nullptr_t InternalType ;
            
            NilLiteral() = delete ;
            NilLiteral(HorseIRParser::LiteralNilContext* cst, ASTNode::MemManagerType& mem) ;
            NilLiteral(ASTNode::MemManagerType& mem) ;

            virtual horseIR::ast::Type* getLiteralType() const override ;
            InternalType getValue() const ;

            virtual std::size_t getNumNodesRecursively() const override ;
            virtual std::vector<ASTNode*> getChildren() const override ;
            virtual std::string toString() const override ;
            virtual std::string toTreeString() const override ;
        protected:
            Type* type ;
        } ;

        class ComplexLiteral : public Literal {
        public:
            typedef std::complex<double> InternalType ;

            ComplexLiteral() = delete ;
            ComplexLiteral(HorseIRParser::LiteralComplexContext* cst, ASTNode::MemManagerType& mem) ;
            ComplexLiteral(HorseIRParser::LiteralComplexCase0Context* cst, ASTNode::MemManagerType& mem) ;
            ComplexLiteral(HorseIRParser::LiteralComplexCase1Context* cst, ASTNode::MemManagerType& mem) ;
            ComplexLiteral(HorseIRParser::LiteralComplexCase2Context* cst, ASTNode::MemManagerType& mem) ;
            ComplexLiteral(HorseIRParser::LiteralComplexCase3Context* cst, ASTNode::MemManagerType& mem) ;
            ComplexLiteral(ASTNode::MemManagerType& mem) ;

            virtual horseIR::ast::Type* getLiteralType() const override ;
            InternalType getValue() const ;

            virtual std::size_t getNumNodesRecursively() const override ;
            virtual std::vector<ASTNode*> getChildren() const override ;
            virtual std::string toString() const override ;
            virtual std::string toTreeString() const override ;
        } ;

        class BoolLiteral : public Literal {
        public:
            typedef std::int_least32_t InternalType ;

            BoolLiteral() = delete ;
            BoolLiteral(HorseIRParser::LiteralBoolContext* cst, ASTNode::MemManagerType& mem) ;
            BoolLiteral(ASTNode::MemManagerType& mem) ;

            virtual horseIR::ast::Type* getLiteralType() const override ;
            InternalType getValue() const ;
            bool getBoolValue() const ;

            virtual std::size_t getNumNodesRecursively() const override ;
            virtual std::vector<ASTNode*> getChildren() const override ;
            virtual std::string toString() const override ;
            virtual std::string toTreeString() const override ;
        } ;

        class CharLiteral : public Literal {
        public:
            typedef char InternalType ;

            CharLiteral() = delete ;
            CharLiteral(HorseIRParser::LiteralCharContext* cst, ASTNode::MemManagerType& mem) ;
            CharLiteral(ASTNode::MemManagerType& mem) ;

            virtual horseIR::ast::Type* getLiteralType() const override ;
            InternalType getValue() const ;

            virtual std::size_t getNumNodesRecursively() const override ;
            virtual std::vector<ASTNode*> getChildren() const override ;
            virtual std::string toString() const override ;
            virtual std::string toTreeString() const override ;
        } ;

        class IntegerLiteral : public Literal {
        public:
            enum class IntegerLiteralClass {
                Integer8Literal, Integer16Literal, Integer32Literal, Integer64Literal
            } ;

            IntegerLiteral() = delete ;
            IntegerLiteral(HorseIRParser::LiteralIntegerContext* cst,
                           ASTNode::MemManagerType& mem,
                           ASTNode::ASTNodeType type,
                           IntegerLiteral::IntegerLiteralClass p_integerLiteralClass) ;
            IntegerLiteral(ASTNode::MemManagerType& mem,
                           ASTNode::ASTNodeType type,
                           IntegerLiteral::IntegerLiteralClass p_integerLiteralClass) ;
            
            virtual std::int_least8_t  getInt8Value()  const = 0 ;
            virtual std::int_least16_t getInt16Value() const = 0 ;
            virtual std::int_least32_t getInt32Value() const = 0 ;
            virtual std::int_least64_t getInt64Value() const = 0 ;

            static IntegerLiteral* makeIntegerLiteralASTNode(HorseIRParser::LiteralIntegerContext* cst, ASTNode::MemManagerType& mem) ;
        protected:
            IntegerLiteralClass integerLiteralClass ;
        } ;

        class Integer8Literal : public IntegerLiteral {
        public:
            typedef std::int8_t InternalType ;
            
            Integer8Literal() = delete ;
            Integer8Literal(HorseIRParser::LiteralIntegerContext* cst,
                            ASTNode::MemManagerType& mem,
                            ASTNode::ASTNodeType type) ;
            Integer8Literal(ASTNode::MemManagerType& mem, ASTNode::ASTNodeType type) ;

            virtual std::int_least8_t  getInt8Value()  const override ;
            virtual std::int_least16_t getInt16Value() const override ;
            virtual std::int_least32_t getInt32Value() const override ;
            virtual std::int_least64_t getInt64Value() const override ;
            InternalType getValue() const ;

            virtual std::size_t getNumNodesRecursively() const override ;
            virtual std::vector<ASTNode*> getChildren() const override ;
            virtual std::string toString() const override ;
            virtual std::string toTreeString() const override ;
        } ;

        class Integer16Literal : public IntegerLiteral {

        } ;
    }
}
