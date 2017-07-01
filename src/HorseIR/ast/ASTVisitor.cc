#include <vector>

#include "ASTVisitor.h"

using namespace horseIR::ast ;

antlrcpp::Any ASTBaseVisitor::visit(ASTNode *node)
{
    using ASTNodeType = ASTNode::ASTNodeType ;
    const ASTNodeType nodeType = node->getNodeType() ;
    switch (nodeType) {
        case ASTNodeType::NilLiteral: return visitNilLiteral(static_cast<NilLiteral*>(node)) ;
        case ASTNodeType::ComplexLiteral: return visitComplexLiteral(static_cast<ComplexLiteral*>(node)) ;
        case ASTNodeType::BoolLiteral: return visitBoolLiteral(static_cast<BoolLiteral*>(node)) ;
        case ASTNodeType::CharLiteral: return visitCharLiteral(static_cast<CharLiteral*>(node)) ;
        case ASTNodeType::Integer8Literal: return visitInteger8Literal(static_cast<Integer8Literal*>(node)) ;
        case ASTNodeType::Integer16Literal: return visitInteger16Literal(static_cast<Integer16Literal*>(node)) ;
        case ASTNodeType::Integer32Literal: return visitInteger32Literal(static_cast<Integer32Literal*>(node)) ;
        case ASTNodeType::Integer64Literal: return visitInteger64Literal(static_cast<Integer64Literal*>(node)) ;
        case ASTNodeType::FP32Literal: return visitFP32Literal(static_cast<FP32Literal*>(node)) ;
        case ASTNodeType::FP64Literal: return visitFP64Literal(static_cast<FP64Literal*>(node)) ;
        case ASTNodeType::SymbolLiteral: return visitSymbolLiteral(static_cast<SymbolLiteral*>(node)) ;
        case ASTNodeType::TimeMonthLiteral: return visitTimeMonthLiteral(static_cast<TimeMonthLiteral*>(node)) ;
        case ASTNodeType::TimeDateLiteral: return visitTimeDateLiteral(static_cast<TimeDateLiteral*>(node)) ;
        case ASTNodeType::TimeDateTimeLiteral: return visitTimeDateTimeLiteral(static_cast<TimeDateTimeLiteral*>(node));
        case ASTNodeType::TimeMinuteLiteral: return visitTimeMinuteLiteral(static_cast<TimeMinuteLiteral*>(node)) ;
        case ASTNodeType::TimeSecondLiteral: return visitTimeSecondLiteral(static_cast<TimeSecondLiteral*>(node)) ;
        case ASTNodeType::TimeTimeLiteral: return visitTimeTimeLiteral(static_cast<TimeTimeLiteral*>(node)) ;
        case ASTNodeType::FunctionLiteral: return visitFunctionLiteral(static_cast<FunctionLiteral*>(node)) ;
        case ASTNodeType::TableLiteral: return visitTableLiteral(static_cast<TableLiteral*>(node)) ;
        case ASTNodeType::KeyTableLiteral: return visitKeyTableLiteral(static_cast<KeyTableLiteral*>(node)) ;
        case ASTNodeType::StringLiteral: return visitStringLiteral(static_cast<StringLiteral*>(node)) ;

        case ASTNodeType::ScalarType: return visitScalarType(static_cast<ScalarType*>(node)) ;
        case ASTNodeType::WildcardType: return visitWildcardType(static_cast<WildcardType*>(node)) ;
        case ASTNodeType::ListType: return visitListType(static_cast<ListType*>(node)) ;
        case ASTNodeType::DictionaryType: return visitDictionaryType(static_cast<DictionaryType*>(node)) ;
        case ASTNodeType::EnumerationType: return visitEnumerationType(static_cast<EnumerationType*>(node)) ;
        case ASTNodeType::FunctionType: return visitFunctionType(static_cast<FunctionType*>(node)) ;
    }
}

antlrcpp::Any ASTBaseVisitor::visitChildren(ASTNode *node)
{
    using ASTNodeType = ASTNode::ASTNodeType ;
    const ASTNodeType nodeType = node->getNodeType() ;
    switch (nodeType) {
        case ASTNodeType::NilLiteral: {
            ASTNode* typeASTNode = static_cast<NilLiteral*>(node)->getLiteralType() ;
            std::size_t visitCounter {0};
            if (typeASTNode != nullptr) {
                (void) visit(typeASTNode) ;
                ++visitCounter ;
            }
            return antlrcpp::Any(visitCounter) ;
        }
        case ASTNodeType::ComplexLiteral: {
            (void) visit(static_cast<ComplexLiteral*>(node)->getLiteralType()) ;
            return antlrcpp::Any(1) ;
        }
        case ASTNodeType::BoolLiteral: {
            (void) visit(static_cast<BoolLiteral*>(node)->getLiteralType()) ;
            return antlrcpp::Any(1) ;
        }
        case ASTNodeType::CharLiteral: {
            (void) visit(static_cast<CharLiteral*>(node)->getLiteralType()) ;
            return antlrcpp::Any(1) ;
        }
        case ASTNodeType::Integer8Literal: {
            (void) visit(static_cast<Integer8Literal*>(node)->getLiteralType()) ;
            return antlrcpp::Any(1) ;
        }
        case ASTNodeType::Integer16Literal: {
            (void) visit(static_cast<Integer16Literal*>(node)->getLiteralType()) ;
            return antlrcpp::Any(1) ;
        }
        case ASTNodeType::Integer32Literal: {
            (void) visit(static_cast<Integer32Literal*>(node)->getLiteralType()) ;
            return antlrcpp::Any(1) ;
        }
        case ASTNodeType::Integer64Literal: {
            (void) visit(static_cast<Integer64Literal*>(node)->getLiteralType()) ;
            return antlrcpp::Any(1) ;
        }
        case ASTNodeType::FP32Literal: {
            (void) visit(static_cast<FP32Literal*>(node)->getLiteralType()) ;
            return antlrcpp::Any(1) ;
        }
        case ASTNodeType::FP64Literal: {
            (void) visit(static_cast<FP64Literal*>(node)->getLiteralType()) ;
            return antlrcpp::Any(1) ;
        }
        case ASTNodeType::SymbolLiteral: {
            (void) visit(static_cast<SymbolLiteral*>(node)->getLiteralType()) ;
            return antlrcpp::Any(1) ;
        }
        case ASTNodeType::TimeMonthLiteral: {
            (void) visit(static_cast<TimeMonthLiteral*>(node)->getLiteralType()) ;
            return antlrcpp::Any(1) ;
        }
        case ASTNodeType::TimeDateLiteral: {
            (void) visit(static_cast<TimeDateLiteral*>(node)->getLiteralType()) ;
            return antlrcpp::Any(1) ;
        }
        case ASTNodeType::TimeDateTimeLiteral: {
            (void) visit(static_cast<TimeDateTimeLiteral*>(node)->getLiteralType()) ;
            return antlrcpp::Any(1) ;
        }
        case ASTNodeType::TimeMinuteLiteral: {
            (void) visit(static_cast<TimeMinuteLiteral*>(node)->getLiteralType()) ;
            return antlrcpp::Any(1) ;
        }
        case ASTNodeType::TimeSecondLiteral: {
            (void) visit(static_cast<TimeSecondLiteral*>(node)->getLiteralType()) ;
            return antlrcpp::Any(1) ;
        }
        case ASTNodeType::TimeTimeLiteral: {
            (void) visit(static_cast<TimeTimeLiteral*>(node)->getLiteralType()) ;
            return antlrcpp::Any(1) ;
        }
        case ASTNodeType::FunctionLiteral: {
            (void) visit(static_cast<FunctionLiteral*>(node)->getLiteralType()) ;
            return antlrcpp::Any(1) ;
        }
        case ASTNodeType::TableLiteral: {
            (void) visit(static_cast<TableLiteral*>(node)->getLiteralType()) ;
            return antlrcpp::Any(1) ;
        }
        case ASTNodeType::KeyTableLiteral: {
            (void) visit(static_cast<KeyTableLiteral*>(node)->getLiteralType()) ;
            return antlrcpp::Any(1) ;
        }
        case ASTNodeType::StringLiteral: {
            (void) visit(static_cast<StringLiteral*>(node)->getLiteralType()) ;
            return antlrcpp::Any(1) ;
        }

        case ASTNodeType::ScalarType: {
            return antlrcpp::Any(0) ;
        }
        case ASTNodeType::WildcardType: {
            return antlrcpp::Any(0) ;
        }
        case ASTNodeType::ListType: {
            (void) visit(static_cast<ListType*>(node)->getElementType()) ;
            return antlrcpp::Any(1) ;
        }
        case ASTNodeType::DictionaryType: {
            const auto ptr = static_cast<DictionaryType*>(node) ;
            (void) visit(ptr->getKeyType()) ;
            (void) visit(ptr->getValueType()) ;
            return antlrcpp::Any(2) ;
        }
        case ASTNodeType::EnumerationType: {
            (void) visit(static_cast<EnumerationType*>(node)->getElementType()) ;
            return antlrcpp::Any(1) ;
        }
        case ASTNodeType::FunctionType: {
            const auto ptr = static_cast<FunctionType*>(node) ;
            std::vector<Type*> parameters(ptr->getParameterTypes()) ;
            for (auto iter = parameters.cbegin(); iter != parameters.cend(); ++iter) {
                (void) visit(*iter) ;
            }
            (void) visit(ptr->getReturnType()) ;
            return antlrcpp::Any(parameters.size() + 1) ;
        }
    }
}

antlrcpp::Any ASTBaseVisitor::visitNilLiteral(NilLiteral *nilLiteral)
{
    return visitChildren(nilLiteral) ;
}

antlrcpp::Any ASTBaseVisitor::visitComplexLiteral(ComplexLiteral *complexLiteral)
{
    return visitChildren(complexLiteral) ;
}

antlrcpp::Any ASTBaseVisitor::visitBoolLiteral(BoolLiteral *boolLiteral)
{
    return visitChildren(boolLiteral) ;
}

antlrcpp::Any ASTBaseVisitor::visitCharLiteral(CharLiteral *charLiteral)
{
    return visitChildren(charLiteral) ;
}

antlrcpp::Any ASTBaseVisitor::visitInteger8Literal(Integer8Literal *integer8Literal)
{
    return visitChildren(integer8Literal) ;
}

antlrcpp::Any ASTBaseVisitor::visitInteger16Literal(Integer16Literal *integer16Literal)
{
    return visitChildren(integer16Literal) ;
}

antlrcpp::Any ASTBaseVisitor::visitInteger32Literal(Integer32Literal *integer32Literal)
{
    return visitChildren(integer32Literal) ;
}

antlrcpp::Any ASTBaseVisitor::visitInteger64Literal(Integer64Literal *integer64Literal)
{
    return visitChildren(integer64Literal) ;
}

antlrcpp::Any ASTBaseVisitor::visitFP32Literal(FP32Literal *fp32Literal)
{
    return visitChildren(fp32Literal) ;
}

antlrcpp::Any ASTBaseVisitor::visitFP64Literal(FP64Literal *fp64Literal)
{
    return visitChildren(fp64Literal) ;
}

antlrcpp::Any ASTBaseVisitor::visitSymbolLiteral(SymbolLiteral *symbolLiteral)
{
    return visitChildren(symbolLiteral) ;
}

antlrcpp::Any ASTBaseVisitor::visitTimeMonthLiteral(TimeMonthLiteral *timeMonthLiteral)
{
    return visitChildren(timeMonthLiteral) ;
}

antlrcpp::Any ASTBaseVisitor::visitTimeDateLiteral(TimeDateLiteral *timeDateLiteral)
{
    return visitChildren(timeDateLiteral) ;
}

antlrcpp::Any ASTBaseVisitor::visitTimeDateTimeLiteral(TimeDateTimeLiteral *timeDateTimeLiteral)
{
    return visitChildren(timeDateTimeLiteral) ;
}

antlrcpp::Any ASTBaseVisitor::visitTimeMinuteLiteral(TimeMinuteLiteral *timeMinuteLiteral)
{
    return visitChildren(timeMinuteLiteral) ;
}

antlrcpp::Any ASTBaseVisitor::visitTimeSecondLiteral(TimeSecondLiteral *timeSecondLiteral)
{
    return visitChildren(timeSecondLiteral) ;
}

antlrcpp::Any ASTBaseVisitor::visitTimeTimeLiteral(TimeTimeLiteral *timeTimeLiteral)
{
    return visitChildren(timeTimeLiteral) ;
}

antlrcpp::Any ASTBaseVisitor::visitFunctionLiteral(FunctionLiteral *functionLiteral)
{
    return visitChildren(functionLiteral) ;
}

antlrcpp::Any ASTBaseVisitor::visitTableLiteral(TableLiteral *tableLiteral)
{
    return visitChildren(tableLiteral) ;
}

antlrcpp::Any ASTBaseVisitor::visitKeyTableLiteral(KeyTableLiteral *keyTableLiteral)
{
    return visitChildren(keyTableLiteral) ;
}

antlrcpp::Any ASTBaseVisitor::visitStringLiteral(StringLiteral *stringLiteral)
{
    return visitChildren(stringLiteral) ;
}

antlrcpp::Any ASTBaseVisitor::visitScalarType(ScalarType *type)
{
    return visitChildren(type) ;
}

antlrcpp::Any ASTBaseVisitor::visitWildcardType(WildcardType *wildcardType)
{
    return visitChildren(wildcardType) ;
}

antlrcpp::Any ASTBaseVisitor::visitListType(ListType *listType)
{
    return visitChildren(listType) ;
}

antlrcpp::Any ASTBaseVisitor::visitDictionaryType(DictionaryType *dictionaryType)
{
    return visitChildren(dictionaryType) ;
}

antlrcpp::Any ASTBaseVisitor::visitEnumerationType(EnumerationType *enumerationType)
{
    return visitChildren(enumerationType) ;
}

antlrcpp::Any ASTBaseVisitor::visitFunctionType(FunctionType *functionType)
{
    return visitChildren(functionType) ;
}