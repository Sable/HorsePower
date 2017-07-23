#pragma once

#include <cassert>
#include <type_traits>

#include "../misc/Suppiler.h"
#include "AST.h"

namespace horseIR {
    namespace ast {
        template <class T, class Suppiler = horseIR::misc::TrivialSuppiler<T>>
        class ASTVisitor {
        public:
            virtual T visit(ASTNode* ast) ;
        protected:
            T visitChildren(ASTNode* ast) ;

            virtual T visitType(Type* type) ;
            virtual T visitScalarType(ScalarType* scalarType) {
                return visitChildren(scalarType) ;
            }
            virtual T visitWildcardType(WildcardType* wildcardType) {
                return visitChildren(wildcardType) ;
            }
            virtual T visitListType(ListType* listType) {
                return visitChildren(listType) ;
            }
            virtual T visitDictionaryType(DictionaryType* dictionaryType) {
                return visitChildren(dictionaryType) ;
            }
            virtual T visitEnumerationType(EnumerationType* enumerationType) {
                return visitChildren(enumerationType) ;
            }
            virtual T visitFunctionType(FunctionType* functionType) {
                return visitChildren(functionType) ;
            }

            virtual T visitOperand(Operand* operand) ;
            virtual T visitIdentifier(Identifier* identifier) {
                return visitChildren(identifier) ;
            }
            
            virtual T visitLiteral(Literal* literal) ;
            virtual T visitNilLiteral(NilLiteral* nilLiteral) {
                return visitChildren(nilLiteral) ;
            }
            virtual T visitComplexLiteral(ComplexLiteral* complexLiteral) {
                return visitChildren(complexLiteral) ;
            }
            virtual T visitBoolLiteral(BoolLiteral* boolLiteral) {
                return visitChildren(boolLiteral) ;
            }
            virtual T visitCharLiteral(CharLiteral* charLiteral) {
                return visitChildren(charLiteral) ;
            }
            virtual T visitIntegerLiteral(IntegerLiteral* integerLiteral) ;
            virtual T visitInteger8Literal(Integer8Literal* int8Literal) {
                return visitChildren(int8Literal) ;
            }
            virtual T visitInteger16Literal(Integer16Literal* int16Literal) {
                return visitChildren(int16Literal) ;
            }
            virtual T visitInteger32Literal(Integer32Literal* int32Literal) {
                return visitChildren(int32Literal) ;
            }
            virtual T visitInteger64Literal(Integer64Literal* int64Literal) {
                return visitChildren(int64Literal) ;
            }
            virtual T visitSymbolLiteral(SymbolLiteral* symbolLiteral) {
                return visitChildren(symbolLiteral) ;
            }
            virtual T visitFunctionLiteral(FunctionLiteral* functionLiteral) {
                return visitChildren(functionLiteral) ;
            }
            virtual T visitTableLiteral(TableLiteral* tabelLiteral) {
                return visitChildren(tabelLiteral) ;
            }

            virtual T visitStatement(Statement* statement) ;
            virtual T visitReturnStatement(ReturnStatement* returnStmt) {
                return visitChildren(returnStmt) ;
            }
            virtual T visitBranchStatement(BranchStatement* branchStmt) {
                return visitChildren(branchStmt) ;
            }
            virtual T visitAssignStatement(AssignStatement* assignStmt) {
                return visitChildren(assignStmt) ;
            }
            virtual T visitLabelStatement(LabelStatement* labelStmt) {
                return visitChildren(labelStmt) ;
            }
            virtual T visitPhiStatement(PhiStatement* phiStmt) {
                return visitChildren(phiStmt) ;
            }

            virtual T visitMethod(Method* method) {
                return visitChildren(method) ;
            }
            virtual T visitModule(Module* module) {
                return visitChildren(module) ;
            }
            virtual T visitCompilationUnit(CompilationUnit* compilationUnit) {
                return visitChildren(compilationUnit) ;
            }
        } ;

        namespace __implementation {
            template <typename UnaryOperation>
            class ASTNodeUnaryOperationAppiler : protected horseIR::ast::ASTVisitor<void> {
            public:
                ASTNodeUnaryOperationAppiler(const UnaryOperation& p_f) : f(p_f) {}   
                void apply(horseIR::ast::ASTNode* ast) {
                    visit(ast) ;
                }
            protected:
                const UnaryOperation& f ;
                virtual void visit(horseIR::ast::ASTNode* ast) override {
                    (void) f(ast) ;
                    horseIR::ast::ASTVisitor<void>::visit(ast) ;
                    return ;
                }
            } ;
        } 
        
        struct ASTVisitors {
            template <typename UnaryOperation>
            static void applyToEachNode(horseIR::ast::ASTNode* ast, const UnaryOperation& f) {
                using namespace ast::__implementation ;
                ASTNodeUnaryOperationAppiler<UnaryOperation> v(f) ;
                v.apply(ast) ;
                return ;
            }
        } ;
    }
}

template <class T, class Suppiler>
inline T horseIR::ast::ASTVisitor<T, Suppiler>::visit(ASTNode* ast)
{
    assert(ast != nullptr) ;
    using ASTNodeClass = horseIR::ast::ASTNode::ASTNodeClass ;
    switch (ast->getASTNodeClass())
    {
    case ASTNodeClass::ScalarType: return visitType(static_cast<Type*>(ast)) ;
    case ASTNodeClass::WildcardType: return visitType(static_cast<Type*>(ast)) ;
    case ASTNodeClass::ListType: return visitType(static_cast<Type*>(ast)) ;
    case ASTNodeClass::DictionaryType: return visitType(static_cast<Type*>(ast)) ;
    case ASTNodeClass::EnumerationType: return visitType(static_cast<Type*>(ast)) ;
    case ASTNodeClass::FunctionType: return visitType(static_cast<Type*>(ast)) ;

    case ASTNodeClass::Identifier: return visitOperand(static_cast<Operand*>(ast)) ;
    case ASTNodeClass::NilLiteral: return visitOperand(static_cast<Operand*>(ast)) ;
    case ASTNodeClass::ComplexLiteral: return visitOperand(static_cast<Operand*>(ast)) ;
    case ASTNodeClass::BoolLiteral: return visitOperand(static_cast<Operand*>(ast)) ;
    case ASTNodeClass::CharLiteral: return visitOperand(static_cast<Operand*>(ast)) ;
    case ASTNodeClass::Integer8Literal: return visitOperand(static_cast<Operand*>(ast)) ;
    case ASTNodeClass::Integer16Literal: return visitOperand(static_cast<Operand*>(ast)) ;
    case ASTNodeClass::Integer32Literal: return visitOperand(static_cast<Operand*>(ast)) ;
    case ASTNodeClass::Integer64Literal: return visitOperand(static_cast<Operand*>(ast)) ;
    case ASTNodeClass::SymbolLiteral: return visitOperand(static_cast<Operand*>(ast)) ;
    case ASTNodeClass::FunctionLiteral: return visitOperand(static_cast<Operand*>(ast)) ;
    case ASTNodeClass::TableLiteral: return visitOperand(static_cast<Operand*>(ast)) ;

    case ASTNodeClass::CompilationUnit:
        return visitCompilationUnit(static_cast<CompilationUnit*>(ast)) ;
    case ASTNodeClass::Module:
        return visitModule(static_cast<Module*>(ast)) ;
    case ASTNodeClass::Method:
        return visitMethod(static_cast<Method*>(ast)) ;
    case ASTNodeClass::LabelStatement: return visitStatement(static_cast<Statement*>(ast)) ;
    case ASTNodeClass::BranchStatement: return visitStatement(static_cast<Statement*>(ast)) ;
    case ASTNodeClass::PhiStatement: return visitStatement(static_cast<Statement*>(ast)) ;
    case ASTNodeClass::AssignStatement: return visitStatement(static_cast<Statement*>(ast)) ;
    case ASTNodeClass::ReturnStatement: return visitStatement(static_cast<Statement*>(ast)) ;
    }
}

template <class T, class Suppiler>
inline T horseIR::ast::ASTVisitor<T, Suppiler>::visitChildren(ASTNode* ast)
{
    assert(ast != nullptr) ;
    std::vector<ASTNode*> const childNodes = ast->getChildren() ;
    for (auto iter = childNodes.cbegin(); iter != childNodes.cend(); ++iter) {
        (void) visit(*iter) ;
    }
    return Suppiler::supply() ;
}

template <class T, class Suppiler>
inline T horseIR::ast::ASTVisitor<T, Suppiler>::visitType(Type* type)
{
    assert(type != nullptr) ;
    using TypeClass = horseIR::ast::Type::TypeClass ;
    switch (type->getTypeClass()) {
    case TypeClass::Wildcard:
        return visitWildcardType(static_cast<WildcardType*>(type)) ;
    case TypeClass::Scalar:
        return visitScalarType(static_cast<ScalarType*>(type)) ;
    case TypeClass::List:
        return visitListType(static_cast<ListType*>(type)) ;
    case TypeClass::Dictionary:
        return visitDictionaryType(static_cast<DictionaryType*>(type)) ;
    case TypeClass::Enumeration:
        return visitEnumerationType(static_cast<EnumerationType*>(type)) ;
    case TypeClass::Function:
        return visitFunctionType(static_cast<FunctionType*>(type)) ;
    }
}

template <class T, class Suppiler>
inline T horseIR::ast::ASTVisitor<T, Suppiler>::visitStatement(Statement* statement)
{
    assert(statement != nullptr) ;
    using StatementClass = horseIR::ast::Statement::StatementClass ;
    switch (statement->getStatementClass()) {
    case StatementClass::Return:
        return visitReturnStatement(static_cast<ReturnStatement*>(statement)) ;
    case StatementClass::Branch:
        return visitBranchStatement(static_cast<BranchStatement*>(statement)) ;
    case StatementClass::Assign:
        return visitAssignStatement(static_cast<AssignStatement*>(statement)) ;
    case StatementClass::Label:
        return visitLabelStatement(static_cast<LabelStatement*>(statement)) ;
    case StatementClass::Phi:
        return visitPhiStatement(static_cast<PhiStatement*>(statement)) ;
    }
}

template <class T, class Suppiler>
inline T horseIR::ast::ASTVisitor<T, Suppiler>::visitOperand(Operand* operand)
{
    assert(operand != nullptr) ;
    using OperandClass = horseIR::ast::Operand::OperandClass ;
    switch (operand->getOperandClass()) {
    case OperandClass::Identifier:
        return visitIdentifier(static_cast<Identifier*>(operand)) ;
    case OperandClass::Literal:
        return visitLiteral(static_cast<Literal*>(operand)) ;
    }
}

template <class T, class Suppiler>
inline T horseIR::ast::ASTVisitor<T, Suppiler>::visitLiteral(Literal* literal)
{
    assert(literal != nullptr) ;
    using LiteralClass = horseIR::ast::Literal::LiteralClass ;
    switch (literal->getLiteralClass()) {
    case LiteralClass::NilLiteral:
        return visitNilLiteral(static_cast<NilLiteral*>(literal)) ;
    case LiteralClass::ComplexLiteral:
        return visitComplexLiteral(static_cast<ComplexLiteral*>(literal)) ;
    case LiteralClass::BoolLiteral:
        return visitBoolLiteral(static_cast<BoolLiteral*>(literal)) ;
    case LiteralClass::CharLiteral:
        return visitCharLiteral(static_cast<CharLiteral*>(literal)) ;
    case LiteralClass::IntegerLiteral:
        return visitIntegerLiteral(static_cast<IntegerLiteral*>(literal)) ;
    case LiteralClass::SymbolLiteral:
        return visitSymbolLiteral(static_cast<SymbolLiteral*>(literal)) ;
    case LiteralClass::FunctionLiteral:
        return visitFunctionLiteral(static_cast<FunctionLiteral*>(literal)) ;
    case LiteralClass::TableLiteral:
        return visitTableLiteral(static_cast<TableLiteral*>(literal)) ;
    }
}

template <class T, class Suppiler>
inline T horseIR::ast::ASTVisitor<T, Suppiler>::visitIntegerLiteral(IntegerLiteral* integerLiteral)
{
    assert(integerLiteral != nullptr) ;
    using IntegerLiteralClass = horseIR::ast::IntegerLiteral::IntegerLiteralClass ;
    switch (integerLiteral->getIntegerLiteralClass()) {
    case IntegerLiteralClass::Integer8Literal:
        return visitInteger8Literal(static_cast<Integer8Literal*>(integerLiteral)) ;
    case IntegerLiteralClass::Integer16Literal:
        return visitInteger16Literal(static_cast<Integer16Literal*>(integerLiteral)) ;
    case IntegerLiteralClass::Integer32Literal:
        return visitInteger32Literal(static_cast<Integer32Literal*>(integerLiteral)) ;
    case IntegerLiteralClass::Integer64Literal:
        return visitInteger64Literal(static_cast<Integer64Literal*>(integerLiteral)) ;
    }
}
