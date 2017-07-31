#pragma once

#include <vector>
#include <memory>
#include <string>
#include <sstream>

#include "MethodMETA.h"
#include "ExternalMethod.h"
#include "InternalMethod.h"
#include "Exception.h"

#include "antlr4-runtime.h"
#include "../misc/Collections.h"

namespace horseIR {
namespace interpreter {

template <typename IntermediateType>
struct VectorDispatcher {
    typedef struct {
        std::vector<MethodMETA<IntermediateType>*> container ;
        std::vector<std::unique_ptr<MethodMETA<IntermediateType>>> mem ;
    } ContainerType ;
    static MethodMETA<IntermediateType>* fetch(const ContainerType& container,
                                               const std::string& moduleName,
                                               const std::string& methodName,
                                               const std::string& signatureString) ;
    static MethodMETA<IntermediateType>* fetch(const ContainerType& container,
                                               const std::string& moduleName,
                                               const std::string& methodName,
                                               const std::vector<ast::Type*>& signatureType) ;
    static void manage(ContainerType& container, MethodMETA<IntermediateType>* method) ;
    static std::string containerToString(const ContainerType& container) ;
} ;

template <typename T>
inline MethodMETA<T>* VectorDispatcher<T>::fetch(const ContainerType &container,
                                                 const std::string &moduleName,
                                                 const std::string &methodName,
                                                 const std::string &signatureString)
{
    antlr4::ANTLRInputStream inStream(signatureString.c_str()) ;
    HorseIRLexer lexer(&inStream) ;
    antlr4::CommonTokenStream tokenStream(&lexer) ;
    HorseIRParser parser(&tokenStream) ;

    lexer.removeErrorListeners() ;
    parser.removeErrorListeners() ;
    InvalidSignatureString::SignatureStringErrorListener errorListener(&signatureString) ;
    lexer.addErrorListener(&errorListener) ;
    parser.addErrorListener(&errorListener) ;

    HorseIRParser::DispatcherInTypeListContext* inParamContext = parser.dispatcherInTypeList() ;
    const std::vector<HorseIRParser::TypeContext*> inParamTypeCTXs (inParamContext->type()) ;
    ast::ASTNode::MemManagerType mem ;
    const std::vector<ast::Type*> inParamTypes = misc::Collections::map(
        inParamTypeCTXs,
        [&](HorseIRParser::TypeContext* typeContext) -> ast::Type* {
            return ast::Type::makeTypeASTNode(typeContext, mem) ;
        }) ;
    ast::FunctionType* fetchType = new ast::FunctionType(mem) ;
    (void) fetchType
        ->setReturnType(new ast::WildcardType(mem))
        .setParameterTypes(inParamTypes)
        .setIsFlexible(false) ;

    auto candidates = container.container ;
    candidates = misc::Collections::filter(candidates, [&](MethodMETA<T>* m) -> bool {
            if (moduleName != m->getModuleName()) return false ;
            if (methodName != m->getMethodName()) return false ;
            return true ;
        }) ;
    candidates = misc::Collections::filter(candidates, [&](MethodMETA<T>* m) -> bool {
            ast::Type* mType = m->getDispatchType() ;
            return mType->isGeneralizationOf(fetchType) ;
        }) ;
    if (candidates.empty()) return nullptr ;
    MethodMETA<T>* retMethod = nullptr ;
    for (auto iterA = candidates.cbegin(); iterA != candidates.cend(); ++iterA) {
        bool isMostSpecific = true ;
        for (auto iterB = candidates.cbegin(); iterB != candidates.cend(); ++iterB) {
            if (*iterA == *iterB) continue ;
            ast::Type* aType = (*iterA)->getDispatchType() ;
            ast::Type* bType = (*iterB)->getDispatchType() ;
            if (aType->isGeneralizationOf(bType)) {
                isMostSpecific = false ;
                break ;
            }
        }
        if (isMostSpecific) {
            assert(retMethod == nullptr) ;
            retMethod = (*iterA) ;
        }
    }
    assert(retMethod != nullptr) ;
    return retMethod ;
}

template <typename T>
inline void VectorDispatcher<T>::manage(ContainerType &container, MethodMETA<T> *method)
{
    auto candidateVector = container.container ;
    candidateVector = misc::Collections::filter(candidateVector, [&](MethodMETA<T>* m) -> bool {
            if (method->getModuleName() != m->getModuleName()) return false ;
            if (method->getMethodName() != m->getMethodName()) return false ;
            return true ;
        }) ;
    misc::Collections::apply(candidateVector, [&](MethodMETA<T>* m) -> void {
            ast::FunctionType* const methodType = method->getDispatchType() ;
            ast::FunctionType* const o_methodType = m->getDispatchType() ;
            if (methodType->isSameAs(o_methodType)) {
                throw OverloadDuplicateException<T>(method, container.container) ;
            }
        }) ;
    candidateVector = misc::Collections::filter(candidateVector, [&](MethodMETA<T>* m) -> bool {
            ast::Type* const methodType = method->getDispatchType() ;
            ast::Type* const mType = m->getDispatchType() ;
            const bool methodIsGeneralizationOf = methodType->isGeneralizationOf(mType) ;
            const bool mIsGeneralizationOf = mType->isGeneralizationOf(methodType) ;
            return !(methodIsGeneralizationOf || mIsGeneralizationOf) ;
        }) ;
    misc::Collections::apply(candidateVector, [&](MethodMETA<T>* m) -> void {
            if (ast::Type::hasOverlap(m->getDispatchType(), method->getDispatchType())) {
                throw OverloadOverlapException<T>(method, container.container) ;
            } 
        }) ;

    container.container.emplace_back(method) ;
    container.mem.emplace_back(method) ;
}

template <typename T>
inline std::string VectorDispatcher<T>::containerToString(const ContainerType &container)
{
    std::ostringstream stream ;
    stream << "[\n  " ;
    auto segmentVector = misc::Collections::map(
        container.container,
        [](MethodMETA<T>* ptr) -> std::string {
            return ptr->toString() ;
        }) ;
    misc::Collections::writeToStream(stream, segmentVector, ",\n  ", "\n]") ;
    return stream.str() ;
}

}
}
