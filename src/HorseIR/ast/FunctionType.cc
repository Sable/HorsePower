#include <cstddef>
#include <cassert>
#include <string>

#include "Type.h"

using FunctionType = horseIR::ast::FunctionType ;
using ASTNodeMemory = horseIR::ast::ASTNodeMemory ;
using Type = horseIR::ast::Type ;

FunctionType::FunctionType(HorseIRParser::TypeCaseFuncContext* cst, ASTNodeMemory& mem)
{
    assert(cst != nullptr) ;
    this->cst = static_cast<decltype(this->cst)>(cst) ;
    
    HorseIRParser::TypeFunc0Context* type0 = nullptr ;
    HorseIRParser::TypeFunc1Context* type1 = nullptr ;
    HorseIRParser::TypeFunc2Context* type2 = nullptr ;
    HorseIRParser::TypeFunc3Context* type3 = nullptr ;

    auto funcCST = static_cast<HorseIRParser::TypeFuncContext*>(cst->typeFunc()) ;
    if ((type0 = dynamic_cast<decltype(type0)>(funcCST)) != nullptr) {
        flexable = false ;
        returnType = Type::makeTypeASTNode(type0->type(), mem) ;
    } else if ((type1 = dynamic_cast<decltype(type1)>(funcCST)) != nullptr) {
        flexable = true ;
        returnType = Type::makeTypeASTNode(type1->type(), mem) ;
    } else if ((type2 = dynamic_cast<decltype(type2)>(funcCST)) != nullptr) {
        flexable = false ;
        std::vector<HorseIRParser::TypeContext*> types(std::move(type2->type())) ;
        for (auto ptr = types.cbegin(); ptr != types.cend(); ++ptr) {
            auto param = Type::makeTypeASTNode(*ptr, mem) ;
            if (ptr + 1 != types.cend()) {
                parameterTypes.push_back(param) ;
            } else {
                returnType = param ;
            }
        }
    } else if ((type3 = dynamic_cast<decltype(type3)>(funcCST)) != nullptr) {
        flexable = true ;
        std::vector<HorseIRParser::TypeContext*> types(std::move(type3->type())) ;
        for (auto ptr = types.cbegin(); ptr != types.cend(); ++ptr) {
            auto param = Type::makeTypeASTNode(*ptr, mem) ;
            if (ptr + 1 != types.cend()) {
                parameterTypes.push_back(param) ;
            } else {
                returnType = param ;
            }
        }
    } else {
        assert(false) ;
    }

    for (auto ptr = parameterTypes.cbegin(); ptr != parameterTypes.cend(); ++ptr) {
        this->children.push_back(*ptr) ;
    }
    this->children.push_back(returnType) ;
}

Type::TypeClass FunctionType::getTypeClass() const
{
    return Type::TypeClass::Function ;
}

bool FunctionType::isGeneralizationOf(horseIR::ast::Type *type) const
{
    assert(type != nullptr) ;
    if (type->getTypeClass() != Type::TypeClass::Function) return false ;
    auto functionType = static_cast<FunctionType*>(type) ;
    
    bool paramGeneralization = true ;
    if (flexable) {
        if (!(parameterTypes.size() <= functionType->parameterTypes.size())) {
            paramGeneralization = false ;
        }
    } else {
        if (!(parameterTypes.size() == functionType->parameterTypes.size())) {
            paramGeneralization = false ;
        }
    }
    if (!paramGeneralization) return false ;

    for (std::size_t ptr = 0; ptr < parameterTypes.size(); ++ptr) {
        const auto myParam = parameterTypes.at(ptr) ;
        const auto targetParam = parameterTypes.at(ptr) ;
        if (!myParam->isGeneralizationOf(targetParam)) return false ;
    }

    return returnType->isGeneralizationOf(functionType->returnType) ;
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
    ostream << (flexable? ", ..." : "")
            << " :"
            << returnType->toString()
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
            << flexable
            << ")" ;
    return ostream.str() ;
}

constexpr std::size_t FunctionType::getMinNumParameters() const
{
    return parameterTypes.size() ;
}

constexpr bool FunctionType::isFlexable() const
{
    return flexable ;
}
