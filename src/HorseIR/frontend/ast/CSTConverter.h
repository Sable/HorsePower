#pragma once

#include <unordered_map>
#include <algorithm>
#include <iterator>
#include <vector>
#include <string>
#include "AST.h"

namespace horseIR
{
namespace ast
{

struct CSTConverter {
  using ASTNodeMemory = ASTNode::ASTNodeMemory;

  using TypeContext = HorseIRParser::TypeContext;
  using TypeCasePrimitiveContext = HorseIRParser::TypeCasePrimitiveContext;
  using TypeCaseWildcardContext = HorseIRParser::TypeCaseWildcardContext;
  using TypeCaseListContext = HorseIRParser::TypeCaseListContext;
  using TypeCaseDictContext = HorseIRParser::TypeCaseDictContext;
  using TypeCaseEnumContext = HorseIRParser::TypeCaseEnumContext;
  using TypeCaseFuncContext = HorseIRParser::TypeCaseFuncContext;

  static Type *convert (ASTNodeMemory &mem, TypeContext *type)
  {
    assert (type != nullptr);
    TypeCasePrimitiveContext *primitive = nullptr;
    TypeCaseWildcardContext *wildcard = nullptr;
    TypeCaseListContext *list = nullptr;
    TypeCaseDictContext *dict = nullptr;
    TypeCaseEnumContext *enume = nullptr;
    TypeCaseFuncContext *func = nullptr;

    if ((primitive = dynamic_cast<decltype (primitive)>(type)) != nullptr)
      return convert (mem, primitive);

    if ((wildcard = dynamic_cast<decltype (wildcard)>(type)) != nullptr)
      return convert (mem, wildcard);

    if ((list = dynamic_cast<decltype (list)>(type)) != nullptr)
      return convert (mem, list);

    if ((dict = dynamic_cast<decltype (dict)>(type)) != nullptr)
      return convert (mem, dict);

    if ((enume = dynamic_cast<decltype (enume)>(type)) != nullptr)
      return convert (mem, enume);

    if ((func = dynamic_cast<decltype (func)>(type)) != nullptr)
      return convert (mem, func);

    assert (false);
    return nullptr;

  }

  static WildcardType *
  convert (ASTNodeMemory &mem, TypeCaseWildcardContext *cst)
  {
    assert (cst != nullptr);
    auto wildcardType = new WildcardType (mem, cst);
    return wildcardType;
  }

  static PrimitiveType *
  convert (ASTNodeMemory &mem, TypeCasePrimitiveContext *cst)
  {
    assert (cst != nullptr);
    const std::string cstText = cst->token->getText ();
    using PrimitiveClass = PrimitiveType::PrimitiveClass;
    using ConvertMapT = std::unordered_map<std::string, PrimitiveClass>;
    ConvertMapT convertMap = {
        {"bool", PrimitiveClass::Bool},
        {"char", PrimitiveClass::Character},
        {"i8", PrimitiveClass::Integer8},
        {"i16", PrimitiveClass::Integer16},
        {"i32", PrimitiveClass::Integer32},
        {"i64", PrimitiveClass::Integer64},
        {"fp32", PrimitiveClass::FP32},
        {"fp64", PrimitiveClass::FP64},
        {"complex", PrimitiveClass::Complex},
        {"sym", PrimitiveClass::Symbol},
        {"m", PrimitiveClass::Month}, {"d", PrimitiveClass::Date},
        {"z", PrimitiveClass::DateTime}, {"u", PrimitiveClass::Minute},
        {"v", PrimitiveClass::Second}, {"t", PrimitiveClass::Time},
        {"str", PrimitiveClass::String}, {"table", PrimitiveClass::Table},
        {"ktable", PrimitiveClass::KeyTable}
    };
    auto iter = convertMap.find (cstText);
    assert (iter != convertMap.end ());

    auto primitiveType = new PrimitiveType (mem, cst);
    primitiveType->setPrimitiveClass (iter->second);

    return (primitiveType);
  }

  static ListType *convert (ASTNodeMemory &mem, TypeCaseListContext *cst)
  {
    assert (cst != nullptr);
    const auto typeListContext = cst->typeList ();
    const auto elementTypeContext = typeListContext->element;
    auto listType = new ListType (mem, typeListContext);
    Type *elementType = convert (mem, elementTypeContext);
    elementType->setParentASTNode (listType);
    listType->setElementType (elementType);
    return listType;
  }

  static DictionaryType *convert (ASTNodeMemory &mem, TypeCaseDictContext *cst)
  {
    assert (cst != nullptr);
    const auto typeDictContext = cst->typeDict ();
    const auto keyTypeContext = typeDictContext->key;
    const auto valueTypeContext = typeDictContext->value;
    auto dictionaryType = new DictionaryType (mem, typeDictContext);
    Type *keyType = convert (mem, keyTypeContext);
    keyType->setParentASTNode (dictionaryType);
    dictionaryType->setKeyType (keyType);
    Type *valueType = convert (mem, valueTypeContext);
    valueType->setParentASTNode (dictionaryType);
    dictionaryType->setValueType (valueType);
    return dictionaryType;
  }

  static EnumerationType *convert (ASTNodeMemory &mem, TypeCaseEnumContext *cst)
  {
    assert (cst != nullptr);
    const auto typeEnumCotnext = cst->typeEnum ();
    const auto elementContext = typeEnumCotnext->element;
    auto enumerationType = new EnumerationType (mem);
    Type *elementType = convert (mem, elementContext);
    elementType->setParentASTNode (enumerationType);
    enumerationType->setElementType (elementType);
    return enumerationType;
  }

  using TypeFuncCase0Context = HorseIRParser::TypeFuncCase0Context;
  using TypeFuncCase1Context = HorseIRParser::TypeFuncCase1Context;
  using TypeFuncCase2Context = HorseIRParser::TypeFuncCase2Context;
  using TypeFuncCase3Context = HorseIRParser::TypeFuncCase3Context;

  static FunctionType *convert (ASTNodeMemory &mem, TypeCaseFuncContext *cst)
  {
    assert (cst != nullptr);
    auto typeFunc = cst->typeFunc ();
    TypeFuncCase0Context *case0 = nullptr;
    TypeFuncCase1Context *case1 = nullptr;
    TypeFuncCase2Context *case2 = nullptr;
    TypeFuncCase3Context *case3 = nullptr;

    if ((case0 = dynamic_cast<decltype (case0)>(typeFunc)) != nullptr)
      return convert (mem, case0);

    if ((case1 = dynamic_cast<decltype (case1)>(typeFunc)) != nullptr)
      return convert (mem, case1);

    if ((case2 = dynamic_cast<decltype (case2)>(typeFunc)) != nullptr)
      return convert (mem, case2);

    if ((case3 = dynamic_cast<decltype (case3)>(typeFunc)) != nullptr)
      return convert (mem, case3);

    assert (false);
    return nullptr;
  }

  static FunctionType *convert (ASTNodeMemory &mem, TypeFuncCase0Context *cst)
  {
    assert (cst != nullptr);
    auto functionType = new FunctionType (mem);
    auto returnTypeContext = cst->type ();
    Type *returnType = convert (mem, returnTypeContext);
    returnType->setParentASTNode (functionType);
    functionType->setReturnType (returnType);
    functionType->setIsFlexible (false);
    return functionType;
  }

  static FunctionType *convert (ASTNodeMemory &mem, TypeFuncCase1Context *cst)
  {
    assert (cst != nullptr);
    auto functionType = new FunctionType (mem);
    auto returnTypeContext = cst->type ();
    Type *returnType = convert (mem, returnTypeContext);
    returnType->setParentASTNode (functionType);
    functionType->setReturnType (returnType);
    functionType->setIsFlexible (true);
    return functionType;
  }

  static FunctionType *convert (ASTNodeMemory &mem, TypeFuncCase2Context *cst)
  {
    assert (cst != nullptr);
    auto functionType = new FunctionType (mem);

    const auto types (cst->type ());
    assert (!cst->isEmpty ());
    std::vector<Type *> parameterTypes{};
    std::transform (
        types.cbegin (), std::prev (types.cend ()),
        std::back_inserter (parameterTypes),
        [&] (TypeContext *typeContext) -> Type *
        {
          Type *retType = convert (mem, typeContext);
          retType->setParentASTNode (functionType);
          return retType;
        });

    TypeContext *returnContext = nullptr;
    returnContext = *(std::prev (types.cend ()));
    Type *returnType = convert (mem, returnContext);
    returnType->setParentASTNode (functionType);

    functionType->setParameterTypes (std::move (parameterTypes));
    functionType->setReturnType (returnType);
    functionType->setIsFlexible (false);
    return functionType;
  }

  static FunctionType *convert (ASTNodeMemory &mem, TypeFuncCase3Context *cst)
  {
    assert (cst != nullptr);
    auto functionType = new FunctionType (mem);

    const auto types (cst->type ());
    assert (!cst->isEmpty ());
    std::vector<Type *> parameterTypes{};
    std::transform (
        types.cbegin (), std::prev (types.cend ()),
        std::back_inserter (parameterTypes),
        [&] (TypeContext *typeContext) -> Type *
        {
          Type *retType = convert (mem, typeContext);
          retType->setParentASTNode (functionType);
          return retType;
        });

    TypeContext *returnContext = nullptr;
    returnContext = *(std::prev (types.cend ()));
    Type *returnType = convert (mem, returnContext);
    returnType->setParentASTNode (functionType);

    functionType->setParameterTypes (std::move (parameterTypes));
    functionType->setReturnType (returnType);
    functionType->setIsFlexible (true);
    return functionType;
  }
};

}
}