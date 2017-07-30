#pragma once

#include <stdexcept>
#include <string>
#include <sstream>

#include "MethodMETA.h"
#include "../ast/AST.h"
#include "../misc/Collections.h"

namespace horseIR {
namespace interpreter {

class InterpreterException : public std::exception {
public:
    enum class InterpreterExceptionClass {
        Compiletime, Runtime
    } ;
    InterpreterException(const InterpreterExceptionClass& p_interpreterExceptionClass)
        : std::exception(),
        interpreterExceptionClass(p_interpreterExceptionClass)
        {} ;
    virtual std::string toString() const noexcept = 0 ;
protected:
    const InterpreterExceptionClass interpreterExceptionClass ;
} ;

class CompiletimeException : public InterpreterException {
public:
    enum class CompiletimeExceptionClass {
        OverloadOverlap, OverloadDuplicate
    } ;
    CompiletimeException(const CompiletimeExceptionClass& p_compiletimeExceptionClass)
        : InterpreterException(InterpreterExceptionClass::Compiletime),
          compiletimeExceptionClass(p_compiletimeExceptionClass)
        {} ;
protected:
    const CompiletimeExceptionClass compiletimeExceptionClass ;
} ;

class RuntimeException : public InterpreterException {} ;

template <typename T>
class OverloadOverlapException : public CompiletimeException {
public:
    OverloadOverlapException(MethodMETA<T>* p_candidateMethod,
                             const std::vector<MethodMETA<T>*>& p_existedOverload)
        : CompiletimeException(CompiletimeExceptionClass::OverloadOverlap),
          candidateMethod(p_candidateMethod),
          existedOverloadOptions(p_existedOverload)
        {}
    const char* what() const noexcept override {
        return "Overload Type Matching Overlap" ;
    }
    MethodMETA<T>* getSiteCandidate() const {
        return candidateMethod ;
    }
    virtual std::string toString() const noexcept override ;
protected:
    MethodMETA<T>* const candidateMethod ;
    std::vector<MethodMETA<T>*> const existedOverloadOptions ;
} ;

template <typename T>
inline std::string OverloadOverlapException<T>::toString() const noexcept  {
    std::ostringstream stream ;
    stream << "Overloading may result in ambiguity, with candidate method: "
           << candidateMethod->toString()
           << ", and existing overloading options: ["
           << std::endl ;
    auto candidates = existedOverloadOptions ;
    candidates = misc::Collections::filter(candidates, [&](MethodMETA<T>* m) -> bool {
            return (m->getModuleName() == candidateMethod->getModuleName() &&
                    m->getMethodName() == candidateMethod->getMethodName()   ) ;
        }) ;
    candidates = misc::Collections::filter(candidates, [&](MethodMETA<T>* m) -> bool {
            ast::Type* const candidateType = candidateMethod->getDispatchType() ;
            ast::Type* const mType = m->getDispatchType() ;
            const bool candidateGeneralized = candidateType->isGeneralizationOf(mType) ;
            const bool mGeneralized = mType->isGeneralizationOf(candidateType) ;
            return !(candidateGeneralized || mGeneralized) ;
        }) ;
    for (auto iter = candidates.cbegin(); iter != candidates.cend(); ++iter) {
        ast::Type* const candidateType = candidateMethod->getDispatchType() ;
        ast::Type* const iterType = (*iter)->getDispatchType() ;
        const bool overlap = ast::Type::hasOverlap(candidateType, iterType) ;
        stream << (overlap? "  * " : "    ") << (*iter)->toString()
               << ((iter + 1 == candidates.cend())? "" : " ,")
               << std::endl ;
    }
    stream << "]" ;
    return stream.str() ;
}

template <typename T>
class OverloadDuplicateException : public CompiletimeException {
public:
    OverloadDuplicateException(MethodMETA<T>* p_candidateMethod,
                               const std::vector<MethodMETA<T>*>& p_existedOverload)
        : CompiletimeException(CompiletimeExceptionClass::OverloadDuplicate),
          candidateMethod(p_candidateMethod),
          existedOverloadOptions(p_existedOverload)
        {}
    const char* what() const noexcept override {
        return "Overload Type Matching Duplicate" ;
    }
    virtual std::string toString() const noexcept override ;
protected:
    MethodMETA<T>* const candidateMethod ;
    std::vector<MethodMETA<T>*> const existedOverloadOptions ;
} ;

template <typename T>
inline std::string OverloadDuplicateException<T>::toString() const noexcept
{
    std::ostringstream stream ;
    stream << "Overloading may result in duplicate, with candidate method: "
           << candidateMethod->toString()
           << ", and existing overloading options: ["
           << std::endl ;
    auto candidates = existedOverloadOptions ;
    candidates = misc::Collections::filter(candidates, [&](MethodMETA<T>* m) -> bool {
            return (m->getModuleName() == candidateMethod->getModuleName() &&
                    m->getMethodName() == candidateMethod->getMethodName()   ) ;
        }) ;
    for (auto iter = candidates.cbegin(); iter != candidates.cend(); ++iter) {
        ast::Type* const candidateType = candidateMethod->getDispatchType() ;
        ast::Type* const iterType = (*iter)->getDispatchType() ;
        const bool duplicate = candidateType->isSameAs(iterType) ;
        stream << (duplicate? "  * " : "    ") << (*iter)->toString()
               << ((iter + 1 == candidates.cend())? "" : " ,")
               << std::endl ;
    }
    stream << "]" ;
    return stream.str() ;
}

}
}
