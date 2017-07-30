#include <cstddef>
#include <cassert>
#include <string>
#include <vector>

#include "../AST.h"
#include "../../misc/Collections.h"

using namespace horseIR::ast ;

FunctionType::FunctionType(ASTNode* parent, HorseIRParser::TypeCaseFuncContext* cst, ASTNode::MemManagerType& mem)
    : Type(parent, cst, mem, Type::TypeClass::Function, ASTNode::ASTNodeClass::FunctionType)
{
    assert(cst != nullptr) ;
    
    HorseIRParser::TypeFunc0Context* type0 = nullptr ;
    HorseIRParser::TypeFunc1Context* type1 = nullptr ;
    HorseIRParser::TypeFunc2Context* type2 = nullptr ;
    HorseIRParser::TypeFunc3Context* type3 = nullptr ;

    auto funcCST = cst->typeFunc() ;
    if ((type0 = dynamic_cast<decltype(type0)>(funcCST)) != nullptr) {
        flexible = false ;
        returnType = Type::makeTypeASTNode(this, type0->type(), mem) ;
    } else if ((type1 = dynamic_cast<decltype(type1)>(funcCST)) != nullptr) {
        flexible = true ;
        returnType = Type::makeTypeASTNode(this, type1->type(), mem) ;
    } else if ((type2 = dynamic_cast<decltype(type2)>(funcCST)) != nullptr) {
        flexible = false ;
        std::vector<HorseIRParser::TypeContext*> types(std::move(type2->type())) ;
        for (auto ptr = types.cbegin(); ptr != types.cend(); ++ptr) {
            auto param = Type::makeTypeASTNode(this, *ptr, mem) ;
            if (ptr + 1 != types.cend()) {
                parameterTypes.push_back(param) ;
            } else {
                returnType = param ;
            }
        }
    } else if ((type3 = dynamic_cast<decltype(type3)>(funcCST)) != nullptr) {
        flexible = true ;
        std::vector<HorseIRParser::TypeContext*> types(std::move(type3->type())) ;
        for (auto ptr = types.cbegin(); ptr != types.cend(); ++ptr) {
            auto param = Type::makeTypeASTNode(this, *ptr, mem) ;
            if (ptr + 1 != types.cend()) {
                parameterTypes.push_back(param) ;
            } else {
                returnType = param ;
            }
        }
    } else {
        assert(false) ;
    }
}

FunctionType::FunctionType(HorseIRParser::TypeCaseFuncContext* cst, ASTNode::MemManagerType& mem)
    : FunctionType(nullptr, cst, mem) 
{}

FunctionType::FunctionType(ASTNode::MemManagerType& mem)
    : Type(mem, Type::TypeClass::Function, ASTNode::ASTNodeClass::FunctionType),
      flexible(false),
      returnType(nullptr)
{}

FunctionType::FunctionType(HorseIRParser::TypeFuncContext* cst, ASTNode::MemManagerType& mem)
    : Type(nullptr, cst, mem, Type::TypeClass::Function, ASTNode::ASTNodeClass::FunctionType)
{
    assert(cst != nullptr) ;

    HorseIRParser::TypeFunc0Context* type0 = nullptr ;
    HorseIRParser::TypeFunc1Context* type1 = nullptr ;
    HorseIRParser::TypeFunc2Context* type2 = nullptr ;
    HorseIRParser::TypeFunc3Context* type3 = nullptr ;

    if ((type0 = dynamic_cast<decltype(type0)>(cst)) != nullptr) {
        flexible = false ;
        returnType = Type::makeTypeASTNode(this, type0->type(), mem) ;
    } else if ((type1 = dynamic_cast<decltype(type1)>(cst)) != nullptr) {
        flexible = true ;
        returnType = Type::makeTypeASTNode(this, type1->type(), mem) ;
    } else if ((type2 = dynamic_cast<decltype(type2)>(cst)) != nullptr) {
        flexible = false ;
        const std::vector<HorseIRParser::TypeContext*> types(std::move(type2->type())) ;
        for (auto ptr = types.cbegin(); ptr != types.cend(); ++ptr) {
            auto param = Type::makeTypeASTNode(this, *ptr, mem) ;
            if (ptr + 1 != types.cend()) {
                parameterTypes.push_back(param) ;
            } else {
                returnType = param ;
            }
        }
    } else if ((type3 = dynamic_cast<decltype(type3)>(cst)) != nullptr) {
        flexible = true ;
        const std::vector<HorseIRParser::TypeContext*> types(std::move(type3->type())) ;
        for (auto ptr = types.cbegin(); ptr != types.cend(); ++ptr) {
            auto param = Type::makeTypeASTNode(this, *ptr, mem) ;
            if (ptr + 1 != types.cend()) {
                parameterTypes.push_back(param) ;
            } else {
                returnType = param ;
            }
        }
    } else {
        assert(false) ;
    }
}

std::size_t FunctionType::getNumNodesRecursively() const
{
    std::size_t count = 1 ;
    count += (returnType == nullptr)? 0: returnType->getNumNodesRecursively() ;
    for (auto ptr = parameterTypes.cbegin(); ptr != parameterTypes.cend(); ++ptr) {
        if (*ptr != nullptr) {
            count += (*ptr)->getNumNodesRecursively() ;
        }
    }
    return count ;
}

std::vector<ASTNode*> FunctionType::getChildren() const
{
    std::vector<ASTNode*> retVector ;
    for (auto ptr = parameterTypes.cbegin(); ptr != parameterTypes.cend(); ++ptr) {
        if (*ptr != nullptr) {
            ASTNode* node_ptr = *ptr ;
            retVector.push_back(node_ptr) ;
        }
    }
    if (returnType != nullptr) {
        ASTNode* ret_ptr = returnType ;
        retVector.push_back(ret_ptr) ;
    }
    return retVector ;
}

std::string FunctionType::toString() const
{
    std::ostringstream ostream ;
    ostream << "func<" ;
    for (auto ptr = parameterTypes.cbegin(); ptr != parameterTypes.cend(); ++ptr) {
        Type* paramType = *ptr ;
        ostream << paramType->toString()
                << ((ptr + 1 == parameterTypes.cend())? "" : ", ") ;
    }
    if (parameterTypes.size() != 0 && flexible) ostream << ", " ;
    ostream << (flexible? "..." : "")
            << " :"
            << ((returnType!= nullptr)? returnType->toString() : "nullptr")
            << ">" ;
    return ostream.str() ;
}

std::string FunctionType::toTreeString() const
{
    std::ostringstream ostream ;
    ostream << std::boolalpha
            << "(FunctionType" ;
    for (auto ptr = parameterTypes.cbegin(); ptr != parameterTypes.cend(); ++ptr) {
        Type* paramType = *ptr ;
        ostream << " "
                << paramType->toTreeString() ;
    }
    ostream << " "
            << returnType->toTreeString()
            << " "
            << flexible
            << ")" ;
    return ostream.str() ;
}

FunctionType* FunctionType::duplicateShallow(ASTNode::MemManagerType &mem) const
{
    FunctionType* functionType = new FunctionType(mem) ;
    functionType->__duplicateShallow(this) ;
    return functionType ;
}

FunctionType* FunctionType::duplicateDeep(ASTNode::MemManagerType &mem) const
{
    FunctionType* functionType = new FunctionType(mem) ;
    functionType->__duplicateDeep(this, mem) ;
    return functionType ;
}

bool FunctionType::isGeneralizationOf(const horseIR::ast::Type *type) const
{
    assert(type != nullptr) ;
    if (type->getTypeClass() != Type::TypeClass::Function) return false ;
    auto castedPtr = static_cast<const FunctionType*>(type) ;
    const std::vector<Type*> o_parameterTypes = castedPtr->getParameterTypes() ;
    using ContainerSizeType = decltype(o_parameterTypes)::size_type ;
    if (flexible) {
        if (parameterTypes.size() <= o_parameterTypes.size()) {
            const ContainerSizeType iterSize = parameterTypes.size() ;
            for (ContainerSizeType iter = 0; iter < iterSize; ++iter) {
                Type* const paramPtr = parameterTypes[iter] ;
                Type* const o_paramPtr = o_parameterTypes[iter] ;
                if (!paramPtr->isGeneralizationOf(o_paramPtr)) return false ;
            }
            return true ;
        } else /* parameterTypes.size() > o_parameterTypes.size() */ {
            if (!castedPtr->getIsFlexible()) return false ;
            const ContainerSizeType iterSize = o_parameterTypes.size() ;
            for (ContainerSizeType iter = 0; iter < iterSize; ++iter) {
                Type* const paramPtr = parameterTypes[iter] ;
                Type* const o_paramPtr = o_parameterTypes[iter] ;
                if (!paramPtr->isGeneralizationOf(o_paramPtr)) return false ;
            }
            return true ;
        }
    } else /* !flexible */ {
        if (parameterTypes.size() < o_parameterTypes.size()) {
            return false ;
        } else if (parameterTypes.size() == o_parameterTypes.size()) {
            const ContainerSizeType iterSize = parameterTypes.size() ;
            for (ContainerSizeType iter = 0; iter < iterSize; ++iter) {
                Type* const paramPtr = parameterTypes[iter] ;
                Type* const o_paramPtr = o_parameterTypes[iter] ;
                if (!paramPtr->isGeneralizationOf(o_paramPtr)) return false ;
            }
            return true ;
        }else /* parameterTypes.size > o_parameterTypes.size() */ {
            if (!castedPtr->getIsFlexible()) return false ;
            const ContainerSizeType iterSize = o_parameterTypes.size() ;
            for (ContainerSizeType iter = 0; iter < iterSize; ++iter) {
                Type* const paramPtr = parameterTypes[iter] ;
                Type* const o_paramPtr = o_parameterTypes[iter] ;
                if (!paramPtr->isGeneralizationOf(o_paramPtr)) return false ;
            }
            return true ;
        }
    }
}

bool FunctionType::isSameAs(const horseIR::ast::Type *type) const
{
    assert(type != nullptr) ;
    if (type->getTypeClass() != Type::TypeClass::Function) return false ;
    auto castedPtr = static_cast<const FunctionType*>(type) ;
    bool parameterTypeIsSame = true ;
    if (parameterTypes.size() != castedPtr->parameterTypes.size()) {
        parameterTypeIsSame = false ;
    } else {
        auto iterSize = parameterTypes.size() ;
        for (decltype(iterSize) iter = 0; iter < iterSize; ++iter) {
            Type* const parameterType = parameterTypes[iter] ;
            Type* const o_parameterType = castedPtr->parameterTypes[iter] ;
            if (!parameterType->isSameAs(o_parameterType)) {
                parameterTypeIsSame = false ;
                break ;
            }
        }
    }
    const bool flexibleIsSame = (flexible == castedPtr->flexible) ;
    const bool returnTypeIsSame = returnType->isSameAs(castedPtr->returnType) ;
    return parameterTypeIsSame && flexibleIsSame && returnTypeIsSame ;
}

std::vector<Type*> FunctionType::getParameterTypes() const
{
    return parameterTypes ;
}

bool FunctionType::getIsFlexible() const
{
    return flexible ;
}

Type* FunctionType::getReturnType() const
{
    return returnType ;
}

FunctionType& FunctionType::setParameterTypes(const std::vector<Type *>& p_parameterTypes)
{
    parameterTypes = p_parameterTypes ;
    return *this ;
}

FunctionType& FunctionType::setIsFlexible(bool p_flexable)
{
    flexible = p_flexable ;
    return *this ;
}

FunctionType& FunctionType::setReturnType(horseIR::ast::Type *p_type)
{
    returnType = p_type ;
    return *this ;
}

void FunctionType::__duplicateShallow(const FunctionType* funcType)
{
    assert(funcType != nullptr) ;
    parameterTypes = funcType->parameterTypes ;
    flexible = funcType->flexible ;
    returnType = funcType->returnType ;
    return ;
}

void FunctionType::__duplicateDeep(const FunctionType* funcType, ASTNode::MemManagerType& mem)
{
    assert(funcType != nullptr) ;
    auto duplicateParamTypes = horseIR::misc::Collections::map(
        funcType->parameterTypes,
        [&] (Type* type) -> Type* {
            assert(type != nullptr) ;
            Type* duplicateType = static_cast<Type*>(type->duplicateDeep(mem)) ;
            (void) duplicateType->setParentASTNode(this) ;
            return duplicateType ;
        }) ;
    parameterTypes = std::move(duplicateParamTypes) ;
    flexible = funcType->flexible ;
    Type* duplicateReturnType = nullptr ;
    if (funcType->returnType != nullptr) {
        duplicateReturnType = static_cast<Type*>(funcType->returnType->duplicateDeep(mem)) ;
        (void) duplicateReturnType->setParentASTNode(this) ;
    }
    returnType = duplicateReturnType ;
    return ;
}
