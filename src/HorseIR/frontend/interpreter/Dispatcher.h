#pragma once

#include <vector>
#include <memory>
#include <string>
#include <sstream>

#include "MethodMETA.h"
#include "ExternalMethod.h"
#include "InternalMethod.h"

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
                                               ast::FunctionType* signatureType) ;
    static void manage(ContainerType& container, MethodMETA<IntermediateType>* method) ;
    static std::string containerToString(const ContainerType& container) ;
} ;

template <typename T>
void VectorDispatcher<T>::manage(ContainerType &container, MethodMETA<T> *method)
{
    auto candidateVector = misc::Collections::filter(
        container.container,
        [&](MethodMETA<T>* o_method) -> bool {
            if (method->getModuleName() != o_method->getModuleName() ||
                method->getMethodName() != o_method->getMethodName()    ){
                return false ;
            } else {
                ast::FunctionType* methodType = method->getMethodType() ;
                ast::FunctionType* o_methodType = o_method->getMethodType() ;
                if (o_methodType->isGeneralizationOf(methodType) ||
                    methodType->isGeneralizationOf(o_methodType)    ) {
                    return false ;
                } else {
                    return true ;
                }
            }
        }) ;
    bool isValidInsert = misc::Collections::pairwiseCheck(
        candidateVector,
        [](MethodMETA<T>* m1, MethodMETA<T>* m2) -> bool {
            ast::ASTNode::MemManagerType tempMem ;
            ast::Type* const m1Type = m1->getMethodType() ;
            ast::Type* const m2Type = m2->getMethodType() ;
            ast::Type* joinedType = nullptr ;
            try {
                joinedType = ast::Type::specificityJoin(m1Type,m2Type,tempMem) ;
            } catch (ast::Type::SpecificityJoinAbortException& except) {
                return false ;
            }
            return !((m1Type->isGeneralizationOf(joinedType) &&
                      m2Type->isGeneralizationOf(joinedType) )) ;
        }) ;
    if (isValidInsert) {
        container.container.emplace_back(method) ;
        container.mem.emplace_back(method) ;
    } else {
        throw std::runtime_error("invalid manage") ;
    }
}

template <typename T>
std::string VectorDispatcher<T>::containerToString(const ContainerType &container)
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
