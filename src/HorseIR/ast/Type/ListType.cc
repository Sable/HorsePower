#include <string>
#include <cassert>

#include "../AST.h"

using namespace horseIR::ast ;

ListType::ListType(ASTNode* parent, HorseIRParser::TypeCaseListContext* cst, ASTNode::MemManagerType& mem)
    : Type(parent, cst, mem, Type::TypeClass::List, ASTNode::ASTNodeClass::ListType)
{
    assert(cst != nullptr) ;

    auto listCST = cst->typeList() ;
    elementType = Type::makeTypeASTNode(this, listCST->element, mem) ;
}

ListType::ListType(ASTNode::MemManagerType& mem)
    : Type(mem, Type::TypeClass::List, ASTNode::ASTNodeClass::ListType),
      elementType(nullptr)
{}
      
bool ListType::isGeneralizationOf(const Type *type) const
{
    assert(type != nullptr) ;
    if (type->getTypeClass() != Type::TypeClass::List) return false ;
    auto listType = static_cast<const ListType*>(type) ;
    return elementType->isGeneralizationOf(listType->elementType) ;
}

std::size_t ListType::getNumNodesRecursively() const
{
    if (elementType == nullptr) {
        return 1 ;
    } else {
        return elementType->getNumNodesRecursively() + 1 ;
    }
}

std::vector<ASTNode*> ListType::getChildren() const
{
    return std::vector<ASTNode*>{elementType} ;
}

std::string ListType::toString() const
{
    return "list<" + elementType->toString() + ">" ;
}

std::string ListType::toTreeString() const
{
    return "(ListType " + elementType->toTreeString() + ")" ;
}

Type* ListType::getElementType() const
{
    return elementType ;
}

ListType& ListType::setElementType(const Type *type)
{
    assert(type != nullptr) ;
    elementType = const_cast<Type*>(type) ;
    return *this ;
}
