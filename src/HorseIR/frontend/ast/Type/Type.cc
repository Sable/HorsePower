#include <cassert>
#include <vector>

#include "../AST.h"
#include "../ASTVisitor.h"
#include "../../misc/Collections.h"

using namespace horseIR::ast ;

Type::Type(ASTNode* parent,
           antlr4::tree::ParseTree* cst,
           ASTNode::MemManagerType& mem,
           const Type::TypeClass& p_typeClass,
           const ASTNode::ASTNodeClass& type)
    : typeClass(p_typeClass),
      ASTNode(parent, cst, mem, type)
{}

Type::Type(ASTNode::MemManagerType& mem,
           const Type::TypeClass& p_typeClass,
           const ASTNode::ASTNodeClass& type)
    : typeClass(p_typeClass),
      ASTNode(mem, type)
{}

Type::TypeClass Type::getTypeClass() const
{
    return typeClass ;
}

Type* Type::makeTypeASTNode(ASTNode* parent, HorseIRParser::TypeContext *cst, ASTNode::MemManagerType& mem)
{
    assert(cst != nullptr) ;
    HorseIRParser::TypeCaseScalarContext* scalar = nullptr ;
    HorseIRParser::TypeCaseWildcardContext* wildcard = nullptr ;
    HorseIRParser::TypeCaseListContext* list = nullptr ;
    HorseIRParser::TypeCaseDictContext* dictionary = nullptr ;
    HorseIRParser::TypeCaseEnumContext* enumeration = nullptr ;
    HorseIRParser::TypeCaseFuncContext* function = nullptr ;
    
    if ((scalar = dynamic_cast<decltype(scalar)>(cst)) != nullptr) {
        ScalarType* ret = new ScalarType(parent, scalar, mem) ;
        return ret ;
    } else if ((wildcard = dynamic_cast<decltype(wildcard)>(cst)) != nullptr) {
        WildcardType* ret = new WildcardType(parent, wildcard, mem) ;
        return ret ;
    } else if ((list = dynamic_cast<decltype(list)>(cst)) != nullptr) {
        ListType* ret = new ListType(parent, list, mem) ;
        return ret ;
    } else if ((dictionary = dynamic_cast<decltype(dictionary)>(cst)) != nullptr) {
        DictionaryType* ret = new DictionaryType(parent, dictionary, mem) ;
        return ret ;
    } else if ((enumeration = dynamic_cast<decltype(enumeration)>(cst)) != nullptr) {
        EnumerationType* ret = new EnumerationType(parent, enumeration, mem) ;
        return ret ;
    } else if ((function = dynamic_cast<decltype(function)>(cst)) != nullptr) {
        FunctionType* ret = new FunctionType(parent, function, mem) ;
        return ret ;
    } else {
        assert(false) ;
        return nullptr ;
    }
}

Type* Type::makeTypeASTNode(HorseIRParser::TypeContext *cst, ASTNode::MemManagerType &mem)
{
    assert(cst != nullptr) ;
    HorseIRParser::TypeCaseScalarContext* scalar = nullptr ;
    HorseIRParser::TypeCaseWildcardContext* wildcard = nullptr ;
    HorseIRParser::TypeCaseListContext* list = nullptr ;
    HorseIRParser::TypeCaseDictContext* dictionary = nullptr ;
    HorseIRParser::TypeCaseEnumContext* enumeration = nullptr ;
    HorseIRParser::TypeCaseFuncContext* function = nullptr ;

    if ((scalar = dynamic_cast<decltype(scalar)>(cst)) != nullptr) {
        ScalarType* ret = new ScalarType(scalar, mem) ;
        return ret ;
    } else if ((wildcard = dynamic_cast<decltype(wildcard)>(cst)) != nullptr) {
        WildcardType* ret = new WildcardType(wildcard, mem) ;
        return ret ;
    } else if ((list = dynamic_cast<decltype(list)>(cst)) != nullptr) {
        ListType* ret = new ListType(list, mem) ;
        return ret ;
    } else if ((dictionary = dynamic_cast<decltype(dictionary)>(cst)) != nullptr) {
        DictionaryType* ret = new DictionaryType(dictionary, mem) ;
        return ret ;
    } else if ((enumeration = dynamic_cast<decltype(enumeration)>(cst)) != nullptr) {
        EnumerationType* ret = new EnumerationType(enumeration, mem) ;
        return ret ;
    } else if ((function = dynamic_cast<decltype(function)>(cst)) != nullptr) {
        FunctionType* ret = new FunctionType(function, mem) ;
        return ret ;
    } else {
        assert(false) ;
        return nullptr ;
    }
}

std::vector<Type*> Type::makeTypeSignatureASTNodes(horseIR::ast::ASTNode *parent,
                                                   HorseIRParser::TypeSignatureListContext *cst,
                                                   ASTNode::MemManagerType &mem)
{
    assert(cst != nullptr) ;
    const std::vector<HorseIRParser::TypeContext*> signatures = cst->type() ;
    return horseIR::misc::Collections::applyAndCollect(
        signatures,
        [&](HorseIRParser::TypeContext* cst) -> Type* {
            return Type::makeTypeASTNode(parent, cst, mem) ;
        }) ;
}

std::vector<Type*> Type::makeTypeSignatureASTNodes(HorseIRParser::TypeSignatureListContext* cst,
                                                   ASTNode::MemManagerType& mem)
{
    assert(cst != nullptr) ;
    const std::vector<HorseIRParser::TypeContext*> signatures = cst->type() ;
    return horseIR::misc::Collections::applyAndCollect(
        signatures,
        [&](HorseIRParser::TypeContext* cst) -> Type* {
            return Type::makeTypeASTNode(cst, mem) ;
        }) ;
}

Type* Type::selectivityMerge(const horseIR::ast::Type *lhs, const horseIR::ast::Type *rhs, ASTNode::MemManagerType &mem)
{
    assert(lhs != nullptr && rhs != nullptr) ;
    Type* retType = Type::__selectivityMerge::selectivityMerge(lhs, rhs, mem) ;
    ASTVisitors::applyToEachNode(
        retType,
        [] (ASTNode* node) -> void {
            const std::vector<ASTNode*> children = node->getChildren() ;
            for (auto iter = children.cbegin(); iter != children.cend(); ++iter) {
                (void) (*iter)->setParentASTNode(node) ;
            }
            (void) node->setCST(nullptr) ;
        }) ;
    (void) retType->setParentASTNode(nullptr) ;
    return retType ;
}

Type* Type::__selectivityMerge::selectivityMerge(const horseIR::ast::Type *lhs, const horseIR::ast::Type *rhs, ASTNode::MemManagerType &mem)
{
    assert(lhs != nullptr && rhs != nullptr) ;
    switch (lhs->getTypeClass()) {
    case TypeClass::Dictionary:
        return __selectivityMerge::mergeDictionary(static_cast<const DictionaryType*>(lhs), rhs, mem) ;
    case TypeClass::Enumeration:
        return __selectivityMerge::mergeEnumeration(static_cast<const EnumerationType*>(lhs), rhs, mem) ;
    case TypeClass::Function:
        return __selectivityMerge::mergeFunction(static_cast<const FunctionType*>(lhs), rhs, mem) ;
    case TypeClass::List:
        return __selectivityMerge::mergeList(static_cast<const ListType*>(lhs), rhs, mem) ;
    case TypeClass::Scalar:
        return __selectivityMerge::mergeScalar(static_cast<const ScalarType*>(lhs), rhs, mem) ;
    case TypeClass::Wildcard:
        return __selectivityMerge::mergeWildcard(static_cast<const WildcardType*>(lhs), rhs, mem) ;
    }
}

Type* Type::__selectivityMerge::mergeScalar(const horseIR::ast::ScalarType *lhs, const horseIR::ast::Type *rhs, ASTNode::MemManagerType &mem)
{
    using SelectivityMergeAbortException = Type::__selectivityMerge::SelectivityMergeAbortException ;
    assert(lhs != nullptr && rhs != nullptr) ;
    switch (rhs->getTypeClass()) {
    case TypeClass::Dictionary:  throw SelectivityMergeAbortException() ;
    case TypeClass::Enumeration: throw SelectivityMergeAbortException() ;
    case TypeClass::Function:    throw SelectivityMergeAbortException() ;
    case TypeClass::List:        throw SelectivityMergeAbortException() ;
    case TypeClass::Scalar: {
        const ScalarType* castedPtr = static_cast<const ScalarType*>(rhs) ;
        if (lhs->getScalarClass() == castedPtr->getScalarClass()) {
            ScalarType* retType = new ScalarType(mem) ;
            (void) retType->setScalarClass(lhs->getScalarClass()) ;
            return retType ;
        } else {
            throw SelectivityMergeAbortException() ;
        }
    }
    case TypeClass::Wildcard: {
        ScalarType* retType = new ScalarType(mem) ;
        (void) retType->setScalarClass(lhs->getScalarClass()) ;
        return retType ;
    }
    }
}

Type* Type::__selectivityMerge::mergeWildcard(const horseIR::ast::WildcardType *lhs, const horseIR::ast::Type *rhs, ASTNode::MemManagerType &mem)
{
    using SelectivityMergeAbortException = Type::__selectivityMerge::SelectivityMergeAbortException ;
    assert(lhs != nullptr && rhs != nullptr) ;
    auto duplicateRHS = rhs->duplicateDeep(mem) ;
    return static_cast<Type*>(duplicateRHS) ;
}

Type* Type::__selectivityMerge::mergeList(const horseIR::ast::ListType *lhs, const horseIR::ast::Type *rhs, ASTNode::MemManagerType &mem)
{
    using SelectivityMergeAbortException = Type::__selectivityMerge::SelectivityMergeAbortException ;
    assert(lhs != nullptr && rhs != nullptr) ;
    switch (rhs->getTypeClass()) {
    case TypeClass::Dictionary:  throw SelectivityMergeAbortException() ;
    case TypeClass::Enumeration: throw SelectivityMergeAbortException() ;
    case TypeClass::Function:    throw SelectivityMergeAbortException() ;
    case TypeClass::List: {
        ListType* retType = new ListType(mem) ;
        const ListType* castedPtr = static_cast<const ListType*>(rhs) ;
        Type* mergedElem = Type::__selectivityMerge::selectivityMerge(
            lhs->getElementType(),
            castedPtr->getElementType(),
            mem) ;
        (void) retType->setElementType(mergedElem) ;
        return retType ;
    }
    case TypeClass::Scalar:     throw SelectivityMergeAbortException() ;
    case TypeClass::Wildcard: {
        ListType* type = lhs->duplicateDeep(mem) ;
        return type ;
    }
    } 
}

Type* Type::__selectivityMerge::mergeDictionary(const horseIR::ast::DictionaryType *lhs, const horseIR::ast::Type *rhs, ASTNode::MemManagerType &mem)
{
    using SelectivityMergeAbortException = Type::__selectivityMerge::SelectivityMergeAbortException ;
    assert(lhs != nullptr && rhs != nullptr) ;
    switch (rhs->getTypeClass()) {
    case TypeClass::Dictionary: {
        DictionaryType* retType = new DictionaryType(mem) ;
        const DictionaryType* castedPtr = static_cast<const DictionaryType*>(rhs) ;
        Type* mergedKey = Type::__selectivityMerge::selectivityMerge(
            lhs->getKeyType(),
            castedPtr->getKeyType(),
            mem) ;
        Type* mergedValue = Type::__selectivityMerge::selectivityMerge(
            lhs->getValueType(),
            castedPtr->getValueType(),
            mem) ;
        (void) retType->setKeyType(mergedKey).setValueType(mergedKey) ;
        return retType ;
    }
    case TypeClass::Enumeration: throw SelectivityMergeAbortException() ;
    case TypeClass::Function:    throw SelectivityMergeAbortException() ;
    case TypeClass::List:        throw SelectivityMergeAbortException() ;
    case TypeClass::Scalar:      throw SelectivityMergeAbortException() ;
    case TypeClass::Wildcard: {
        DictionaryType* dictType = lhs->duplicateDeep(mem) ;
        return dictType ;
    }
    }
}

Type* Type::__selectivityMerge::mergeEnumeration(const horseIR::ast::EnumerationType *lhs, const horseIR::ast::Type *rhs, ASTNode::MemManagerType &mem)
{
    using SelectivityMergeAbortException = Type::__selectivityMerge::SelectivityMergeAbortException ;
    assert(lhs != nullptr && rhs != nullptr) ;
    switch (rhs->getTypeClass()) {
    case TypeClass::Dictionary:  throw SelectivityMergeAbortException() ;
    case TypeClass::Enumeration: {
        EnumerationType* retType = new EnumerationType(mem) ;
        const EnumerationType* castedPtr = static_cast<const EnumerationType*>(rhs) ;
        Type* mergedElement = Type::__selectivityMerge::selectivityMerge(
            lhs->getElementType(),
            castedPtr->getElementType(),
            mem) ;
        (void) retType->setElementType(mergedElement) ;
        return retType ;
    }
    case TypeClass::Function:    throw SelectivityMergeAbortException() ;
    case TypeClass::List:        throw SelectivityMergeAbortException() ;
    case TypeClass::Scalar:      throw SelectivityMergeAbortException() ;
    case TypeClass::Wildcard: {
        EnumerationType* enumType = lhs->duplicateDeep(mem) ;
        return enumType ;
    }
    }
}

Type* Type::__selectivityMerge::mergeFunction(const horseIR::ast::FunctionType *lhs, const horseIR::ast::Type *rhs, ASTNode::MemManagerType &mem)
{
    using SelectivityMergeAbortException = Type::__selectivityMerge::SelectivityMergeAbortException ;
    assert(lhs != nullptr && rhs != nullptr) ;
    switch (rhs->getTypeClass()) {
    case TypeClass::Dictionary:  throw SelectivityMergeAbortException() ;
    case TypeClass::Enumeration: throw SelectivityMergeAbortException() ;
    case TypeClass::Function: {
        const FunctionType* castedPtr = static_cast<const FunctionType*>(rhs) ;
        const std::vector<Type*> lhsParameters = lhs->getParameterTypes() ;
        const std::vector<Type*> rhsParameters = castedPtr->getParameterTypes() ;
        if (lhsParameters.size() < rhsParameters.size()) {
            if (!lhs->getIsFlexible()) throw SelectivityMergeAbortException() ;
            std::vector<Type*> retParameterTypes {} ;
            std::vector<Type*>::size_type iterSize = lhsParameters.size() ;
            for (decltype(iterSize) iter = 0; iter < iterSize; ++iter) {
                Type* mergedParameter = Type::__selectivityMerge::selectivityMerge(
                    lhsParameters[iter],
                    rhsParameters[iter],
                    mem) ;
                retParameterTypes.push_back(mergedParameter) ;
            }
            for (decltype(iterSize) iter = iterSize; iter < rhsParameters.size(); ++iter) {
                auto duplicateParameter = rhsParameters[iter]->duplicateDeep(mem) ;
                retParameterTypes.push_back(static_cast<Type*>(duplicateParameter)) ;
            }
            Type* retReturnType = Type::__selectivityMerge::selectivityMerge(
                lhs->getReturnType(),
                castedPtr->getReturnType(),
                mem) ;

            FunctionType* retType = new FunctionType(mem) ;
            (void) retType
                ->setParameterTypes(retParameterTypes)
                .setReturnType(retReturnType) ;
            return retType ;
        } else if (lhsParameters.size() == rhsParameters.size()) {
            std::vector<Type*> retParameterTypes {} ;
            std::vector<Type*>::size_type iterSize = lhsParameters.size() ;
            for (decltype(iterSize) iter = 0; iter < iterSize; ++iter) {
                Type* mergedParameter = Type::__selectivityMerge::selectivityMerge(
                    lhsParameters[iter],
                    rhsParameters[iter],
                    mem) ;
                retParameterTypes.push_back(mergedParameter) ;
            }
            Type* retReturnType = Type::__selectivityMerge::selectivityMerge(
                lhs->getReturnType(),
                castedPtr->getReturnType(),
                mem) ;

            FunctionType* retType = new FunctionType(mem) ;
            (void) retType
                ->setParameterTypes(retParameterTypes)
                .setReturnType(retReturnType) ;
            return retType ;
        } else /* lhsParameters.size() > rhsParameters.size() */ {
            if (!castedPtr->getIsFlexible()) throw SelectivityMergeAbortException() ;
            std::vector<Type*> retParameterTypes {} ;
            for (auto iter = 0; iter < rhsParameters.size(); ++iter) {
                Type* mergedParameter = Type::__selectivityMerge::selectivityMerge(
                    lhsParameters[iter],
                    rhsParameters[iter],
                    mem) ;
                retParameterTypes.push_back(mergedParameter) ;
            }
            for (auto iter = rhsParameters.size(); iter < lhsParameters.size(); ++iter) {
                auto duplicateParameter = lhsParameters[iter]->duplicateDeep(mem) ;
                retParameterTypes.push_back(static_cast<Type*>(duplicateParameter)) ;
            }
            Type* retReturnType = Type::__selectivityMerge::selectivityMerge(
                lhs->getReturnType(),
                castedPtr->getReturnType(),
                mem) ;

            FunctionType* retType = new FunctionType(mem) ;
            (void) retType
                ->setParameterTypes(retParameterTypes)
                .setReturnType(retReturnType) ;
            return retType ;
        }
    }
    case TypeClass::List:        throw SelectivityMergeAbortException() ;
    case TypeClass::Scalar:      throw SelectivityMergeAbortException() ;
    case TypeClass::Wildcard: {
        FunctionType* funcType = lhs->duplicateDeep(mem) ;
        return funcType ;
    }
    }
}

void Type::__duplicateShallow(const Type* type)
{
    assert(type != nullptr) ;
    return ;
}

void Type::__duplicateDeep(const Type* type, ASTNode::MemManagerType& mem)
{
    assert(type != nullptr) ;
    (void) mem ;
    return ;
}
