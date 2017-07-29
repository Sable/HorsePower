#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <sstream>

#include "../ast/AST.h"
#include "../misc/Collections.h"

namespace horseIR {
    namespace interpreter {
        template <typename IntermediateType>
        class MethodMETA {
        public:
            enum class MethodMETAClass {
                Internal, External
            } ;

            MethodMETA(const std::string& p_moduleName,
                       const std::string & p_methodName,
                       const MethodMETAClass& p_methodMETAClass) ;
            virtual ~MethodMETA() = default ;

            std::string getModuleName() const ;
            std::string getMethodName() const ;
            ast::FunctionType* getMethodType() const ;

            virtual std::string toString() const ;
            MethodMETAClass getMethodMETAClass() const ;
        public:
            ast::ASTNode::MemManagerType mem ;
            const std::string methodName ;
            const std::string moduleName ;
            const MethodMETAClass methodMETAClass ;
            ast::FunctionType* methodType ;
        } ;
    }
}

template <typename T>
inline horseIR::interpreter::MethodMETA<T>::MethodMETA (
    const std::string& p_moduleName,
    const std::string& p_methodName,
    const MethodMETAClass& p_methodMETAClass)
    : methodName(p_methodName),
      moduleName(p_moduleName),
      methodMETAClass(p_methodMETAClass)
{}

template <typename T>
inline std::string
horseIR::interpreter::MethodMETA<T>::getModuleName() const
{
    return moduleName ;
}

template <typename T>
inline std::string
horseIR::interpreter::MethodMETA<T>::getMethodName() const
{
    return methodName ;
}

template <typename T>
inline horseIR::ast::FunctionType*
horseIR::interpreter::MethodMETA<T>::getMethodType() const
{
    return methodType ;
}

template <typename T>
inline std::string
horseIR::interpreter::MethodMETA<T>::toString() const
{
    std::ostringstream stream ;
    stream << moduleName << '.' << methodName
           << '('
           << methodType->toString()
           << ')' ;
    return stream.str() ;
}

template <typename T>
inline typename horseIR::interpreter::MethodMETA<T>::MethodMETAClass
horseIR::interpreter::MethodMETA<T>::getMethodMETAClass() const
{
    return methodMETAClass ;
}
