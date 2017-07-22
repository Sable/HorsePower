#include <cassert>
#include <vector>

#include "../AST.h"
#include "../../misc/Collections.h"

using namespace horseIR::ast ;

Type::Type(ASTNode* parent, const antlr4::tree::ParseTree* cst, ASTNode::MemManagerType& mem, Type::TypeClass p_typeClass, ASTNode::ASTNodeClass type)
    : typeClass(p_typeClass),
      ASTNode(parent, cst, mem, type)
{}

Type::Type(ASTNode::MemManagerType& mem, Type::TypeClass p_typeClass, ASTNode::ASTNodeClass type)
    : typeClass(p_typeClass),
      ASTNode(mem, type)
{}

Type::TypeClass Type::getTypeClass() const
{
    return typeClass ;
}

Type* Type::makeTypeASTNode(ASTNode* parent, HorseIRParser::TypeContext *cst, ASTNode::MemManagerType& mem)
{
    assert(cst != nullptr) ;
    HorseIRParser::TypeCaseScalarContext* scalar = nullptr ;
    HorseIRParser::TypeCaseWildcardContext* wildcard = nullptr ;
    HorseIRParser::TypeCaseListContext* list = nullptr ;
    HorseIRParser::TypeCaseDictContext* dictionary = nullptr ;
    HorseIRParser::TypeCaseEnumContext* enumeration = nullptr ;
    HorseIRParser::TypeCaseFuncContext* function = nullptr ;
    
    if ((scalar = dynamic_cast<decltype(scalar)>(cst)) != nullptr) {
        ScalarType* ret = new ScalarType(parent, scalar, mem) ;
        return ret ;
    } else if ((wildcard = dynamic_cast<decltype(wildcard)>(cst)) != nullptr) {
        WildcardType* ret = new WildcardType(parent, wildcard, mem) ;
        return ret ;
    } else if ((list = dynamic_cast<decltype(list)>(cst)) != nullptr) {
        ListType* ret = new ListType(parent, list, mem) ;
        return ret ;
    } else if ((dictionary = dynamic_cast<decltype(dictionary)>(cst)) != nullptr) {
        DictionaryType* ret = new DictionaryType(parent, dictionary, mem) ;
        return ret ;
    } else if ((enumeration = dynamic_cast<decltype(enumeration)>(cst)) != nullptr) {
        EnumerationType* ret = new EnumerationType(parent, enumeration, mem) ;
        return ret ;
    } else if ((function = dynamic_cast<decltype(function)>(cst)) != nullptr) {
        FunctionType* ret = new FunctionType(parent, function, mem) ;
        return ret ;
    } else {
        assert(false) ;
        return nullptr ;
    }
}

Type* Type::makeTypeASTNode(HorseIRParser::TypeContext *cst, ASTNode::MemManagerType &mem)
{
    assert(cst != nullptr) ;
    HorseIRParser::TypeCaseScalarContext* scalar = nullptr ;
    HorseIRParser::TypeCaseWildcardContext* wildcard = nullptr ;
    HorseIRParser::TypeCaseListContext* list = nullptr ;
    HorseIRParser::TypeCaseDictContext* dictionary = nullptr ;
    HorseIRParser::TypeCaseEnumContext* enumeration = nullptr ;
    HorseIRParser::TypeCaseFuncContext* function = nullptr ;

    if ((scalar = dynamic_cast<decltype(scalar)>(cst)) != nullptr) {
        ScalarType* ret = new ScalarType(scalar, mem) ;
        return ret ;
    } else if ((wildcard = dynamic_cast<decltype(wildcard)>(cst)) != nullptr) {
        WildcardType* ret = new WildcardType(wildcard, mem) ;
        return ret ;
    } else if ((list = dynamic_cast<decltype(list)>(cst)) != nullptr) {
        ListType* ret = new ListType(list, mem) ;
        return ret ;
    } else if ((dictionary = dynamic_cast<decltype(dictionary)>(cst)) != nullptr) {
        DictionaryType* ret = new DictionaryType(dictionary, mem) ;
        return ret ;
    } else if ((enumeration = dynamic_cast<decltype(enumeration)>(cst)) != nullptr) {
        EnumerationType* ret = new EnumerationType(enumeration, mem) ;
        return ret ;
    } else if ((function = dynamic_cast<decltype(function)>(cst)) != nullptr) {
        FunctionType* ret = new FunctionType(function, mem) ;
        return ret ;
    } else {
        assert(false) ;
        return nullptr ;
    }
}

std::vector<Type*> Type::makeTypeSignatureASTNodes(horseIR::ast::ASTNode *parent,
                                                   HorseIRParser::TypeSignatureListContext *cst,
                                                   ASTNode::MemManagerType &mem)
{
    assert(cst != nullptr) ;
    const std::vector<HorseIRParser::TypeContext*> signatures = cst->type() ;
    return horseIR::misc::Collections::applyAndCollect(
        signatures,
        [&](HorseIRParser::TypeContext* cst) -> Type* {
            return Type::makeTypeASTNode(parent, cst, mem) ;
        }) ;
}

std::vector<Type*> Type::makeTypeSignatureASTNodes(HorseIRParser::TypeSignatureListContext* cst,
                                                   ASTNode::MemManagerType& mem)
{
    assert(cst != nullptr) ;
    const std::vector<HorseIRParser::TypeContext*> signatures = cst->type() ;
    return horseIR::misc::Collections::applyAndCollect(
        signatures,
        [&](HorseIRParser::TypeContext* cst) -> Type* {
            return Type::makeTypeASTNode(cst, mem) ;
        }) ;
}
