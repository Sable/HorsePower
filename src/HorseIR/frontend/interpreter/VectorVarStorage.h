#pragma once

#include <vector>
#include <tuple>
#include <algorithm>
#include <cassert>
#include <sstream>
#include <limits>
#include <string>

#include "antlr4-runtime.h"
#include "../ast/AST.h"
#include "../ast/ASTVisitor.h"
#include "../misc/Collections.h"
#include "Exception.h"

namespace horseIR {
namespace interpreter {

template <typename IntermediateType>
struct VectorVarStorage {
    typedef struct {
        typedef std::tuple<std::string, IntermediateType, ast::Type*> UnitType ;
        typedef enum {
            Name = 0, Value = 1, Type = 2
        } UnitTypeSelector ;
        std::vector<UnitType> storageContainer ;
        ast::ASTNode::MemManagerType mem ;
    } ContainerType ;

    static IntermediateType write(ContainerType& c, const std::string& name, const IntermediateType& v) ;
    static IntermediateType read(const ContainerType& c, const std::string& name) ;
    static void alloc(ContainerType& c, const std::string& name, ast::Type* type) ;
    static void alloc(ContainerType& c, const std::string& name, const std::string& type) ;
    static ast::Type* queryType(const ContainerType& c, const std::string& name) ;
    static std::string containerToString(const ContainerType& c) ;

    typedef typename std::vector<typename ContainerType::UnitType>::const_iterator ConstIteratorType ;
    static ConstIteratorType cbegin(const ContainerType& c) ;
    static ConstIteratorType cend(const ContainerType& c) ;
} ;

template <typename T>
inline T VectorVarStorage<T>::write(ContainerType& c, const std::string& name, const T& v)
{
    auto varIter = std::find_if(
        c.storageContainer.begin(),
        c.storageContainer.end(),
        [&] (const typename ContainerType::UnitType& unitStorage) -> bool {
            return std::get<ContainerType::UnitTypeSelector::Name>(unitStorage) == name ;
        }) ;
    if (varIter == c.storageContainer.end()) assert(false) ; /* TODO: handle exception */
    T swappedValue = v ;
    std::swap(swappedValue, std::get<ContainerType::UnitTypeSelector::Value>(*varIter)) ;
    return swappedValue ;
}

template <typename T>
inline T VectorVarStorage<T>::read(const ContainerType& c, const std::string& name)
{
    auto varIter = std::find_if(
        c.storageContainer.cbegin(),
        c.storageContainer.cend(),
        [&] (const typename ContainerType::UnitType& unitStorage) -> bool {
            return std::get<ContainerType::UnitTypeSelector::Name>(unitStorage) == name ;
        }) ;
    if (varIter == c.storageContainer.cend()) assert(false) ; /* TODO: handle exception */
    return std::get<ContainerType::UnitTypeSelector::Value>(*varIter) ;
}

template <typename T>
inline void VectorVarStorage<T>::alloc(ContainerType &c, const std::string &name, ast::Type *type)
{
    auto duplicateIter = std::find_if(
        c.storageContainer.cbegin(),
        c.storageContainer.cend(),
        [&] (const typename ContainerType::UnitType& unitStorage) -> bool {
            return std::get<ContainerType::UnitTypeSelector::Name>(unitStorage) == name ;
        }) ;
    if (duplicateIter != c.storageContainer.cend()) assert(false) ; /* TODO: handle exception */

    std::string duplicateName = name ;
    ast::Type* duplicateType = static_cast<ast::Type*>(type->duplicateDeep(c.mem)) ;
    ast::ASTVisitors::applyToEachNode(duplicateType, [](ast::ASTNode* node) -> void {
            (void) node->setCST(nullptr) ;
        }) ;
    (void) duplicateType->setParentASTNode(nullptr) ;
    
    auto newEntry = std::make_tuple(std::move(duplicateName), nullptr, std::move(duplicateType)) ;
    c.storageContainer.emplace_back(std::move(newEntry)) ;
    return ;
}

template <typename T>
inline void VectorVarStorage<T>::alloc(ContainerType &c, const std::string &name, const std::string &type)
{
    auto duplicateIter = std::find_if(
        c.storageContainer.cbegin(),
        c.storageContainer.cend(),
        [&](const typename ContainerType::UnitType& unitStorage) -> bool {
            return std::get<ContainerType::UnitTypeSelector::Name>(unitStorage) == name ;
        }) ;
    if (duplicateIter != c.storageContainer.cend()) assert(false) ; /* TODO: handle exception */
    
    antlr4::ANTLRInputStream inStream(type.c_str()) ;
    HorseIRLexer lexer(&inStream) ;
    antlr4::CommonTokenStream tokenStream(&lexer) ;
    HorseIRParser parser(&tokenStream) ;

    std::string duplicateName = name ;
    lexer.removeErrorListeners() ;
    parser.removeParseListeners() ;
    InvalidSignatureStringException::SignatureStringErrorListener errorListener(&type) ;
    lexer.addErrorListener(&errorListener) ;
    parser.addErrorListener(&errorListener) ;
    HorseIRParser::TypeContext* typeContext = parser.type() ;
    ast::Type* astType = ast::Type::makeTypeASTNode(typeContext, c.mem) ;
    ast::ASTVisitors::applyToEachNode(astType, [](ast::ASTNode* node) -> void {
            (void) node->setCST(nullptr) ;
        }) ;
    (void) astType->setParentASTNode(nullptr) ;

    auto newEntry = std::make_tuple(std::move(duplicateName), nullptr, std::move(astType)) ;
    c.storageContainer.emplace_back(std::move(newEntry)) ;
    return ;
}

template <typename T>
inline ast::Type* VectorVarStorage<T>::queryType(const ContainerType &c, const std::string &name)
{
    auto searchIter = std::find_if(
        c.storageContainer.cbeing(),
        c.storageContainer.cend(),
        [&](const typename ContainerType::UnitType& unitStorage) -> bool {
            return std::get<ContainerType::UnitTypeSelector::Name>(unitStorage) == name ;
        }) ;
    if (searchIter == c.storageContainer.cend()) assert(false) ; /* TODO: handle exception */
    return std::get<ContainerType::UnitTypeSelector::Type>(*searchIter) ;
}

template <typename T>
inline std::string VectorVarStorage<T>::containerToString(const ContainerType &c)
{
    std::ostringstream stream ;
    stream << '[' << '\n' ;
    std::vector<std::string> nameVector = misc::Collections::map(
        c.storageContainer,
        [&](const typename ContainerType::UnitType& storageUnit) -> std::string {
            return std::get<ContainerType::UnitTypeSelector::Name>(storageUnit) ;
        }) ;
    std::vector<std::string> typeVector = misc::Collections::map(
        c.storageContainer,
        [&](const typename ContainerType::UnitType& storageUnit) -> std::string {
            ast::Type* type = std::get<ContainerType::UnitTypeSelector::Type>(storageUnit) ;
            assert(type != nullptr) ;
            return type->toString() ;
        }) ;
    std::size_t maxNameLength = std::numeric_limits<std::size_t>::min() ;
    for (auto iter = nameVector.cbegin(); iter != nameVector.cend(); ++iter) {
        maxNameLength = (iter->length() > maxNameLength)? iter->length() : maxNameLength ;
    }
    assert(nameVector.size() == typeVector.size()) ;
    std::ios initSetting(NULL) ;
    initSetting.copyfmt(stream) ;
    const std::size_t iterSize = nameVector.size() ;
    for (std::size_t iter = 0; iter < iterSize; ++iter) {
        stream << "    "
               << std::setfill(' ') << std::setw(maxNameLength + 1) << std::left
               << nameVector[iter] ;
        stream.copyfmt(initSetting) ;
        stream << ':'
               << typeVector[iter] ;
        if (iter + 1 < iterSize) {
            stream << ' ' << ',' << '\n' ;
        } else {
            stream << '\n' ;
        }
    }
    stream << ']' ;
    return stream.str() ;
}

template <typename T>
inline typename VectorVarStorage<T>::ConstIteratorType VectorVarStorage<T>::cbegin(const ContainerType &c)
{
    return c.storageContainer.cbegin() ;
}

template <typename T>
inline typename VectorVarStorage<T>::ConstIteratorType VectorVarStorage<T>::cend(const ContainerType &c)
{
    return c.storageContainer.cend() ;
}

}
}
