#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <sstream>

#include "../ast/AST.h"
#include "../misc/Hasher.h"
#include "../misc/Collections.h"

namespace horseIR {
    namespace interpreter {
        template <typename IntermediateType,
                  typename HashCodeType = std::uint32_t,
                  typename Hasher = misc::Hasher<std::basic_string<char>, HashCodeType>>
        class MethodMETA {
        public:
            enum class MethodMETAClass {
                Primitive, UDF
            } ;

            MethodMETA(const std::string& p_moduleName,
                       const std::string & p_methodName,
                       const MethodMETAClass& p_methodMETAClass) ;
            virtual ~MethodMETA() = default ;

            HashCodeType getHashCode() const ;
            std::string getModuleName() const ;
            std::string getMethodName() const ;
            std::vector<ast::Type*> getInputTypes() const ;
            ast::Type* getOutputType() const ;

            virtual std::string toString() const ;
            MethodMETAClass getMethodMETAClass() const ;
        public:
            ast::ASTNode::MemManagerType mem ;
            const std::string methodName ;
            const std::string moduleName ;
            const MethodMETAClass methodMETAClass ;
            std::vector<ast::Type*> inputTypes ;
            ast::Type* outputType ;
            const HashCodeType hashCode ;
        } ;
    }
}

template <typename T, typename V, typename R>
inline horseIR::interpreter::MethodMETA<T, V, R>::MethodMETA (
    const std::string& p_moduleName,
    const std::string& p_methodName,
    const MethodMETAClass& p_methodMETAClass)
    : methodName(p_methodName),
      moduleName(p_moduleName),
      methodMETAClass(p_methodMETAClass),
      hashCode(R::hash(moduleName + "." + methodName))
{}

template <typename T, typename V, typename R>
inline V
horseIR::interpreter::MethodMETA<T, V, R>::getHashCode() const
{
    return hashCode ;
}

template <typename T, typename V, typename R>
inline std::string
horseIR::interpreter::MethodMETA<T, V, R>::getModuleName() const
{
    return moduleName ;
}

template <typename T, typename V, typename R>
inline std::string
horseIR::interpreter::MethodMETA<T, V, R>::getMethodName() const
{
    return methodName ;
}

template <typename T, typename V, typename R>
inline std::vector<horseIR::ast::Type*>
horseIR::interpreter::MethodMETA<T, V, R>::getInputTypes() const
{
    return inputTypes ;
}

template <typename T, typename V, typename R>
inline horseIR::ast::Type*
horseIR::interpreter::MethodMETA<T, V, R>::getOutputType() const
{
    return outputType ;
}

template <typename T, typename V, typename R>
inline std::string
horseIR::interpreter::MethodMETA<T, V, R>::toString() const
{
    std::ostringstream stream ;
    std::ios init(nullptr) ;
    stream << "[HASH:0x" 
           << std::hex << std::setw(sizeof(V) * 2) << std::setfill('0')
           << unsigned(hashCode)
           << "] "
           << moduleName << '.' << methodName ;
    auto inputParamToString = horseIR::misc::Collections::applyAndCollect(
        inputTypes,
        [] (const horseIR::ast::Type* type) -> std::string {
            return type->toString() ;
        }) ;
    stream << '(' ;
    (void) horseIR::misc::Collections::writeToStream(stream, inputParamToString, ", ") ;
    stream << ")->"
           << outputType->toString() ;
    return stream.str() ;
}

template <typename T, typename V, typename R>
inline typename horseIR::interpreter::MethodMETA<T, V, R>::MethodMETAClass
horseIR::interpreter::MethodMETA<T, V, R>::getMethodMETAClass() const
{
    return methodMETAClass ;
}
