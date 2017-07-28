#pragma once

#include <string>
#include <vector>
#include <sstream>
#include <algorithm>

#include "ASTNode.h"
#include "antlr4-runtime.h"
#include "HorseIRParser.h"

namespace horseIR {
    namespace ast {
        class DictionaryType ;
        class EnumerationType ;
        class FunctionType ;
        class ListType ;
        class ScalarType ;
        class WildcardType ;
        
        class Type : public ASTNode {
        public:
            enum class TypeClass {
                Wildcard, Scalar, List, Dictionary, Enumeration, Function
            } ;
            Type() = delete ;
            Type(ASTNode* parent,
                 antlr4::tree::ParseTree* cst,
                 ASTNode::MemManagerType& mem,
                 const Type::TypeClass& p_typeClass,
                 const ASTNode::ASTNodeClass& type) ;
            Type(ASTNode::MemManagerType& mem,
                 const Type::TypeClass& p_typeClass,
                 const ASTNode::ASTNodeClass& type) ;
            virtual ~Type() override = default ;

            Type::TypeClass getTypeClass() const ;
            
            static Type* makeTypeASTNode(ASTNode* parent, HorseIRParser::TypeContext* cst, ASTNode::MemManagerType& mem) ;
            static Type* makeTypeASTNode(HorseIRParser::TypeContext* cst, ASTNode::MemManagerType& mem) ;
            static std::vector<Type*> makeTypeSignatureASTNodes(ASTNode* parent, HorseIRParser::TypeSignatureListContext* cst, ASTNode::MemManagerType& mem) ;
            static std::vector<Type*> makeTypeSignatureASTNodes(HorseIRParser::TypeSignatureListContext* cst, ASTNode::MemManagerType& mem) ;

            class SpecificityJoinAbortException : public std::runtime_error {
                public:
                    SpecificityJoinAbortException(const Type* p_lhsSite, const Type* p_rhsSite) ;
                    const Type* getLHSSite() const ;
                    const Type* getRHSSite() const ;
                protected:
                    const Type* const lhsSite ;
                    const Type* const rhsSite ;
                } ;
            static Type* specificityJoin(const Type* lhs, const Type* rhs, ASTNode::MemManagerType& mem) ;
            virtual bool isGeneralizationOf(const Type* type) const = 0 ;
        protected:
            struct __specificityJoin {
                static Type* specificityJoin(const Type* lhs, const Type* rhs, ASTNode::MemManagerType& mem) ;
                static Type* joinScalar(const ScalarType* lhs, const Type* rhs, ASTNode::MemManagerType& mem) ;
                static Type* joinWildcard(const WildcardType* lhs, const Type* rhs, ASTNode::MemManagerType& mem) ;
                static Type* joinList(const ListType* lhs, const Type* rhs, ASTNode::MemManagerType& mem) ;
                static Type* joinDictionary(const DictionaryType* lhs, const Type* rhs, ASTNode::MemManagerType& mem) ;
                static Type* joinEnumeration(const EnumerationType* lhs, const Type* rhs, ASTNode::MemManagerType& mem) ;
                static Type* joinFunction(const FunctionType* lhs, const Type* rhs, ASTNode::MemManagerType& mem) ;
            } ;
            const Type::TypeClass typeClass ;
            void __duplicateShallow(const Type* type) ;
            void __duplicateDeep(const Type* type, ASTNode::MemManagerType& mem) ;
        } ;
        
        class ScalarType : public Type {
        public:
            enum class ScalarClass {
                Bool, Char, Integer8, Integer16, Integer32, Integer64, FP32, FP64,
                Complex, Symbol, Month, Date, DateTime, Minute, Second, Time, String,
                Table, KeyTable
            } ;
            
            ScalarType() = delete ;
            ScalarType(ASTNode* parent,
                       HorseIRParser::TypeCaseScalarContext* cst,
                       ASTNode::MemManagerType& mem) ;
            ScalarType(HorseIRParser::TypeCaseScalarContext* cst,
                       ASTNode::MemManagerType& mem) ;
            ScalarType(const ScalarType::ScalarClass& type, ASTNode::MemManagerType& mem) ;
            ScalarType(ASTNode::MemManagerType& mem) ;
            ~ScalarType() override = default ;
            
            virtual std::size_t getNumNodesRecursively() const override ;
            virtual std::vector<ASTNode*> getChildren() const override ;
            virtual std::string toString() const override ;
            virtual std::string toTreeString() const override ;
            virtual ScalarType* duplicateShallow(ASTNode::MemManagerType& mem) const override ;
            virtual ScalarType* duplicateDeep(ASTNode::MemManagerType& mem) const override ;

            virtual bool isGeneralizationOf(const Type* type) const override ;
            
            ScalarType::ScalarClass getScalarClass() const ;
            ScalarType& setScalarClass(const ScalarType::ScalarClass& type) ;
        protected:
            ScalarType::ScalarClass scalarClass ;
            void __duplicateShallow(const ScalarType* scalarType) ;
            void __duplicateDeep(const ScalarType* scalarType, ASTNode::MemManagerType& mem) ;
        } ;

        class WildcardType : public Type {
        public:
            WildcardType() = delete ;
            WildcardType(ASTNode* parent,
                         HorseIRParser::TypeCaseWildcardContext* cst,
                         ASTNode::MemManagerType& mem) ;
            WildcardType(HorseIRParser::TypeCaseWildcardContext* cst,
                         ASTNode::MemManagerType& mem) ;
            WildcardType(ASTNode::MemManagerType& mem) ;
            ~WildcardType() override = default ;

            virtual std::size_t getNumNodesRecursively() const override ;
            virtual std::vector<ASTNode*> getChildren() const override ;
            virtual std::string toString() const override ;
            virtual std::string toTreeString() const override ;
            virtual WildcardType* duplicateShallow(ASTNode::MemManagerType& mem) const override ;
            virtual WildcardType* duplicateDeep(ASTNode::MemManagerType& mem) const override ;

            virtual bool isGeneralizationOf(const Type* type) const override ;
        protected:
            void __duplicateShallow(const WildcardType* wildcardType) ;
            void __duplicateDeep(const WildcardType* wildcardType, ASTNode::MemManagerType& mem) ;                                
        } ;

        class ListType : public Type {
        public:
            ListType() = delete ;
            ListType(ASTNode* parent,
                     HorseIRParser::TypeCaseListContext* cst,
                     ASTNode::MemManagerType& mem) ;
            ListType(HorseIRParser::TypeCaseListContext* cst,
                     ASTNode::MemManagerType& mem) ;
            ListType(ASTNode::MemManagerType& mem) ;
            ~ListType() override = default ;

            virtual std::size_t getNumNodesRecursively() const override ;
            virtual std::vector<ASTNode*> getChildren() const override ;
            virtual std::string toString() const override ;
            virtual std::string toTreeString() const override ;
            virtual ListType* duplicateShallow(ASTNode::MemManagerType& mem) const override ;
            virtual ListType* duplicateDeep(ASTNode::MemManagerType& mem) const override ;

            virtual bool isGeneralizationOf(const Type* type) const override ;
            
            Type* getElementType() const ;
            ListType& setElementType(Type* type) ;
        protected:
            Type* elementType ;
            void __duplicateShallow(const ListType* listType) ;
            void __duplicateDeep(const ListType* listType, ASTNode::MemManagerType& mem) ;
        } ;

        class DictionaryType : public Type {
        public:
            DictionaryType() = delete ;
            DictionaryType(ASTNode* parent,
                           HorseIRParser::TypeCaseDictContext* cst,
                           ASTNode::MemManagerType& mem) ;
            DictionaryType(HorseIRParser::TypeCaseDictContext* cst,
                           ASTNode::MemManagerType& mem) ;
            DictionaryType(ASTNode::MemManagerType& mem) ;
            ~DictionaryType() override = default ;

            virtual std::size_t getNumNodesRecursively() const override ;
            virtual std::vector<ASTNode*> getChildren() const override ;
            virtual std::string toString() const override ;
            virtual std::string toTreeString() const override ;
            virtual DictionaryType* duplicateShallow(ASTNode::MemManagerType& mem) const override ;
            virtual DictionaryType* duplicateDeep(ASTNode::MemManagerType& mem) const override ;

            virtual bool isGeneralizationOf(const Type* type) const override ;
            
            Type* getKeyType() const ;
            Type* getValueType() const ;
            DictionaryType& setKeyType(Type* type) ;
            DictionaryType& setValueType(Type* type) ;
        protected:
            Type* keyType ;
            Type* valueType ;
            void __duplicateShallow(const DictionaryType* dictType) ;
            void __duplicateDeep(const DictionaryType* dictType, ASTNode::MemManagerType& mem) ;
        } ;
        
        class EnumerationType : public Type {
        public:
            EnumerationType() = delete ;
            EnumerationType(ASTNode* parent,
                            HorseIRParser::TypeCaseEnumContext* cst,
                            ASTNode::MemManagerType& mem) ;
            EnumerationType(HorseIRParser::TypeCaseEnumContext* cst,
                            ASTNode::MemManagerType& mem) ;
            EnumerationType(ASTNode::MemManagerType& mem) ;
            ~EnumerationType() override = default ;

            virtual std::size_t getNumNodesRecursively() const override ;
            virtual std::vector<ASTNode*> getChildren() const override ;
            virtual std::string toString() const override ;
            virtual std::string toTreeString() const override ;
            virtual EnumerationType* duplicateShallow(ASTNode::MemManagerType& mem) const override ;
            virtual EnumerationType* duplicateDeep(ASTNode::MemManagerType& mem) const override ;

            virtual bool isGeneralizationOf(const Type* type) const override ;
            
            Type* getElementType() const ;
            EnumerationType& setElementType(Type* type) ;
        protected:
            Type* elementType ;
            void __duplicateShallow(const EnumerationType* enumType) ;
            void __duplicateDeep(const EnumerationType* enumType, ASTNode::MemManagerType& mem) ;
        } ;

        class FunctionType : public Type {
        public:
            FunctionType() = delete ;
            FunctionType(ASTNode* parent, HorseIRParser::TypeCaseFuncContext* cst, ASTNode::MemManagerType& mem) ;
            FunctionType(HorseIRParser::TypeCaseFuncContext* cst, ASTNode::MemManagerType& mem) ;
            FunctionType(ASTNode::MemManagerType& mem) ;
            ~FunctionType() override = default ;

            virtual std::size_t getNumNodesRecursively() const override ;
            virtual std::vector<ASTNode*> getChildren() const override ;
            virtual std::string toString() const override ;
            virtual std::string toTreeString() const override ;
            virtual FunctionType* duplicateShallow(ASTNode::MemManagerType& mem) const override ;
            virtual FunctionType* duplicateDeep(ASTNode::MemManagerType& mem) const override ;

            virtual bool isGeneralizationOf(const Type* type) const override ;
            
            std::vector<Type*> getParameterTypes() const ;
            bool getIsFlexible() const ;
            Type* getReturnType() const ;

            FunctionType& setParameterTypes(const std::vector<Type*>& p_parameterTypes) ;
            FunctionType& setIsFlexible(bool p_flexable) ;
            FunctionType& setReturnType(Type* p_type) ;
        protected:
            std::vector<Type*> parameterTypes ;
            bool flexible ;
            Type* returnType ;
            void __duplicateShallow(const FunctionType* funcType) ;
            void __duplicateDeep(const FunctionType* funcType, ASTNode::MemManagerType& mem) ;
        } ;
    }
}
