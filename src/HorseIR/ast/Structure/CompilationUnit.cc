#include <vector>
#include <cassert>
#include <sstream>
#include <string>
#include <algorithm>

#include "../AST.h"

using namespace horseIR::ast ;

CompilationUnit::CompilationUnit(HorseIRParser::ProgramContext *cst, ASTNode::MemManagerType &mem)
    : ASTNode(nullptr, cst, mem, ASTNode::ASTNodeClass::CompilationUnit)
{
    assert(cst != nullptr) ;
    std::vector<HorseIRParser::ProgramContentContext*> contents = cst->programContent() ;
    for (auto iter = contents.cbegin(); iter != contents.cend(); ++iter) {
        if ((*iter)->module() != nullptr) {
            HorseIRParser::ModuleContext* moduleContext = (*iter)->module() ;
            Module* module = new Module(this, moduleContext, mem) ;
            modules.push_back(module) ;
            continue ;
        } else if ((*iter)->content() != nullptr) {
            // TODO
            continue ;
        } else {
            assert(false) ;
            continue ;
        }
    }
}

CompilationUnit::CompilationUnit(ASTNode::MemManagerType &mem)
    : ASTNode(mem, ASTNode::ASTNodeClass::CompilationUnit)
{

}

CompilationUnit&& CompilationUnit::merge(CompilationUnit &&obj)
{
    // TODO
    return std::move(*this) ;
}

std::vector<Module*> CompilationUnit::getModules() const
{
    return modules ;
}

Module* CompilationUnit::getModule(std::size_t index) const
{
    return (index < modules.size())? modules[index] : nullptr ;
}

Module* CompilationUnit::getModule(const std::string &moduleName) const
{
    auto ptr = std::find_if(modules.begin(), modules.end(), [&](Module* module) {
            return module->getModuleName() == moduleName ;
        }) ;
    return (ptr == modules.end())? nullptr : *ptr ;
}

std::size_t CompilationUnit::getNumNodesRecursively() const
{
    std::size_t count = 1 ;
    for (auto moduleIter = modules.cbegin(); moduleIter != modules.cend(); ++moduleIter) {
        count = count + (*moduleIter)->getNumNodesRecursively() ;
    }
    return count ;
}

std::vector<ASTNode*> CompilationUnit::getChildren() const
{
    std::vector<ASTNode*> retVector ;
    for (auto iter = modules.cbegin(); iter != modules.cend(); ++iter) {
        retVector.push_back(static_cast<ASTNode*>(*iter)) ;
    }
    return retVector ;
}

std::string CompilationUnit::toString() const
{
    std::ostringstream stream ;
    for (auto iter = modules.cbegin(); iter != modules.cend(); ++iter) {
        stream << (*iter)->toString() << std::endl << std::endl ;
    }
    return stream.str() ;
}

std::string CompilationUnit::toTreeString() const
{
    std::ostringstream stream ;
    stream << "(CompilationUnit " ;
    for (auto iter = modules.cbegin(); iter != modules.cend(); ++iter) {
        stream << (*iter)->toTreeString() ;
    }
    stream << ")" ;
    return stream.str() ;
}

void CompilationUnit::__duplicateShallow(const CompilationUnit* compilationUnit)
{
    assert(compilationUnit != nullptr) ;
    ASTNode::__duplicateShallow(compilationUnit) ;
    modules = compilationUnit->modules ;
    return ;
}

void CompilationUnit::__duplicateDeep(const CompilationUnit* compilationUnit, ASTNode::MemManagerType& mem)
{
    assert(compilationUnit != nullptr) ;
    ASTNode::__duplicateDeep(compilationUnit, mem) ;
    decltype(modules) duplicateModules {} ;
    for (auto iter = compilationUnit->modules.cbegin(); iter != compilationUnit->modules.cend(); ++iter) {
        assert(*iter != nullptr) ;
        Module* duplicateModule = static_cast<Module*>((*iter)->duplicateDeep(mem)) ;
        (void) duplicateModule->setParentASTNode(this) ;
        duplicateModules.push_back(duplicateModule) ;
    }
    modules = std::move(duplicateModules) ;
    return ;
}
