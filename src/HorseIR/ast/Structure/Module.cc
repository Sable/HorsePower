#include <vector>
#include <cassert>
#include <sstream>
#include <string>
#include <algorithm>

#include "../AST.h"

using namespace horseIR::ast ;

Module::Module(ASTNode* parent, HorseIRParser::ModuleContext *cst, ASTNode::MemManagerType &mem)
    : ASTNode(parent, cst, mem, ASTNode::ASTNodeClass::Module)
{
    assert(cst != nullptr) ;
    moduleName = ASTNode::CSTNameToString(cst->name()) ;
    const std::vector<HorseIRParser::ContentContext*> contents = cst->content() ;
    for (auto iter = contents.cbegin(); iter != contents.cend(); ++iter) {
        if ((*iter)->method() != nullptr) {
            HorseIRParser::MethodContext* methodContext = (*iter)->method() ;
            Method* method = new Method(this, methodContext, mem) ;
            methods.push_back(method) ;
            continue ;
        } else if ((*iter)->globalVar() != nullptr) {
            HorseIRParser::GlobalVarContext* globalVarContext = (*iter)->globalVar() ;
            Identifier* variable = new Identifier(this, globalVarContext->name(), mem) ;
            (void) variable->setPackageName(moduleName) ;
            HorseIRParser::TypeContext* varTypeContext = globalVarContext->type() ;
            Type* varType = Type::makeTypeASTNode(this, varTypeContext, mem) ;
            globalVariables.emplace_back(variable, varType) ;
            continue ;
        } else if ((*iter)->importModule() != nullptr) {
            HorseIRParser::ImportModuleContext* importModuleContext = (*iter)->importModule() ;
            HorseIRParser::ImportIDContext* importIDContext = nullptr ;
            HorseIRParser::ImportCIDContext* importCIDContext = nullptr ;
            if ((importIDContext = dynamic_cast<decltype(importIDContext)>(importModuleContext)) != nullptr) {
                std::string importModuleName = importIDContext->COMPOUND_ID()->getText() ;
                importedModules.push_back(std::move(importModuleName)) ;
            } else if ((importCIDContext = dynamic_cast<decltype(importCIDContext)>(importModuleContext)) != nullptr) {
                std::string importModuleName = importCIDContext->IMPORT_COMPOUND_ID()->getText() ;
                importedModules.push_back(std::move(importModuleName)) ;
            } else {
                assert(false) ;
            }
            continue ;
        } else {
            assert(false) ;
            continue ;
        }
    }
}

Module::Module(ASTNode::MemManagerType &mem)
    : ASTNode(mem, ASTNode::ASTNodeClass::Module)
{}

std::string Module::getModuleName() const
{
    return moduleName ;
}

std::vector<std::pair<Identifier*, Type*>> Module::getGlobalVariables() const
{
    return globalVariables ;
}

std::vector<Method*> Module::getMethods() const
{
    return methods ;
}

Method* Module::getMethod(std::size_t index) const
{
    return (index >= methods.size())? nullptr : methods[index] ;
}

Method* Module::getMethod(const std::string &methodName) const
{
    auto ptr = std::find_if(methods.begin(), methods.end(), [&](Method* method) -> bool {
            return method->getMethodName() == methodName ;
        }) ;
    return (ptr == methods.end())? nullptr : *ptr ;
}

std::size_t Module::getNumNodesRecursively() const
{
    std::size_t count = 1 ;
    for (auto iter = globalVariables.cbegin(); iter != globalVariables.cend(); ++iter) {
        count = count + iter->second->getNumNodesRecursively() ;
    }
    for (auto iter = methods.cbegin(); iter != methods.cend(); ++iter) {
        count = count + (*iter)->getNumNodesRecursively() ;
    }
    return count ;
}

std::vector<ASTNode*> Module::getChildren() const
{
    std::vector<ASTNode*> retVector ;
    for (auto iter = globalVariables.cbegin(); iter != globalVariables.cend(); ++iter) {
        retVector.push_back(static_cast<ASTNode*>(iter->second)) ;
    }
    for (auto iter = methods.cbegin(); iter != methods.cend(); ++iter) {
        retVector.push_back(static_cast<ASTNode*>(*iter)) ;
    }
    return retVector ;
}

std::string Module::toString() const
{
    std::ostringstream stream ;
    stream << "module " << moduleName << " {" << std::endl ;
    for (auto iter = importedModules.cbegin(); iter != importedModules.cend(); ++iter) {
        stream << ASTNode::INDENT << "import " << *iter << " ;" << std::endl;
    }
    for (auto iter = globalVariables.cbegin(); iter != globalVariables.cend(); ++iter) {
        stream << ASTNode::INDENT << "def "
               << iter->first->getIDName() << " :"
               << iter->second->toString() << " ;" << std::endl ;
    }
    for (auto iter = methods.cbegin(); iter != methods.cend(); ++iter) {
        std::string line ;
        std::istringstream istream((*iter)->toString()) ;
        while (std::getline(istream, line, '\n')) {
            stream << ASTNode::INDENT << line << std::endl ;
        }
    }
    stream << '}' ;
    return stream.str() ;
}

std::string Module::toTreeString() const
{
    std::ostringstream stream ;
    stream << "(Module name:" << moduleName ;
    for (auto iter = methods.cend(); iter != methods.cend(); ++iter) {
        if (iter + 1 != methods.cend()) {
            stream << (*iter)->toTreeString() << ' ' ;
        } else {
            stream << (*iter)->toTreeString() ;
        }
    }
    stream << ')' ;
    return stream.str() ;
}
