#include <string>
#include <cassert>

#include "../AST.h"

using namespace horseIR::ast ;

ListType::ListType(ASTNode* parent,
                   HorseIRParser::TypeCaseListContext* cst,
                   ASTNode::MemManagerType& mem)
    : Type(parent, cst, mem, Type::TypeClass::List, ASTNode::ASTNodeClass::ListType)
{
    assert(cst != nullptr) ;

    auto listCST = cst->typeList() ;
    elementType = Type::makeTypeASTNode(this, listCST->element, mem) ;
}

ListType::ListType(HorseIRParser::TypeCaseListContext* cst,
                   ASTNode::MemManagerType& mem)
    : ListType(nullptr, cst, mem) 
{}

ListType::ListType(ASTNode::MemManagerType& mem)
    : Type(mem, Type::TypeClass::List, ASTNode::ASTNodeClass::ListType),
      elementType(nullptr)
{}

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

ListType* ListType::duplicateShallow(ASTNode::MemManagerType &mem) const
{
    ListType* listType = new ListType(mem) ;
    listType->__duplicateShallow(this) ;
    return listType ;
}

ListType* ListType::duplicateDeep(ASTNode::MemManagerType &mem) const
{
    ListType* listType = new ListType(mem) ;
    listType->__duplicateDeep(this, mem) ;
    return listType ;
}

bool ListType::isGeneralizationOf(const horseIR::ast::Type *type) const
{
    assert(type != nullptr) ;
    if (type->getTypeClass() != Type::TypeClass::List) return false ;
    auto castedPtr = static_cast<const ListType*>(type) ;
    return elementType->isGeneralizationOf(castedPtr->getElementType()) ;
}

Type* ListType::getElementType() const
{
    return elementType ;
}

ListType& ListType::setElementType(Type *type)
{
    assert(type != nullptr) ;
    elementType = type ;
    return *this ;
}

void ListType::__duplicateShallow(const ListType* listType)
{
    assert(listType != nullptr) ;
    elementType = listType->elementType ;
    return ;
}

void ListType::__duplicateDeep(const ListType* listType, ASTNode::MemManagerType& mem)
{
    assert(listType != nullptr) ;
    Type* duplicateElementType = nullptr ;
    if (listType->elementType != nullptr) {
        duplicateElementType = static_cast<Type*>(listType->elementType->duplicateDeep(mem)) ;
        (void) duplicateElementType->setParentASTNode(this) ;
    }
    elementType = duplicateElementType ;
    return ;
}
