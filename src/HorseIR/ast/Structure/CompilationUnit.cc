#include <vector>
#include <cassert>
#include <sstream>
#include <string>
#include "../grammar/HorseIRParser.h"

#include "../Structure.h"

using namespace horseIR::ast ;

CompilationUnit::CompilationUnit(HorseIRParser::ProgramContext *cst, ASTNode::MemManagerType &mem)
    : ASTNode(cst, mem, ASTNode::ASTNodeClass::CompilationUnit)
{
    assert(cst != nullptr) ;
    std::vector<HorseIRParser::ProgramContentContext*> contents = cst->programContent() ;
    for (auto iter = contents.cbegin(); iter != contents.cend(); ++iter) {
        if ((*iter)->module() != nullptr) {
            HorseIRParser::ModuleContext* moduleContext = (*iter)->module() ;
            Module* module = new Module(moduleContext, mem) ;
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
