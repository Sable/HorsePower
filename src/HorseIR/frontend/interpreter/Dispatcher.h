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
    static void manage(const ContainerType& container, MethodMETA<IntermediateType>* method) ;
    static std::string containerToString(const ContainerType& container) ;
} ;

template <typename T>
void VectorDispatcher<T>::manage(const ContainerType &container, MethodMETA<T> *method)
{
    auto candidate = misc::Collections::filter(
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
    
}

template <typename T>
std::string VectorDispatcher<T>::containerToString(const ContainerType &container)
{
    std::ostringstream stream ;
    stream << '[' ;
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
