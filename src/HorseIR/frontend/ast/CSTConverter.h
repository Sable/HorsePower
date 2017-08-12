#pragma once

#include <limits>
#include "AST.h"

namespace horseIR
{
namespace ast
{

struct CSTConverter {
  class CSTConverterException : public std::logic_error {
   public:
    using CSTType = antlr4::tree::ParseTree;

    explicit CSTConverterException (const CSTType *exceptionSite)
        : logic_error ("CSTConverterException"), site (exceptionSite)
    {}

    const antlr4::tree::ParseTree *getExceptionSite () const
    { return site; }

   protected:
    const antlr4::tree::ParseTree *site = nullptr;
  };

  using ASTNodeMemory = ASTNode::ASTNodeMemory;

  using LiteralBool = HorseIRParser::LiteralBoolContext;
  using LiteralBoolCase0 = HorseIRParser::LiteralBoolCase0Context;
  using LiteralBoolCase1 = HorseIRParser::LiteralBoolCase1Context;

  static BoolLiteral *convert (ASTNodeMemory &mem, LiteralBool *literalBool)
  {
    assert (literalBool != nullptr);
    LiteralBoolCase0 *case0 = nullptr;
    LiteralBoolCase1 *case1 = nullptr;

    if ((case0 = dynamic_cast<decltype (case0)>(literalBool)) != nullptr)
      return convert (mem, case0);
    if ((case1 = dynamic_cast<decltype (case1)>(literalBool)) != nullptr)
      return convert (mem, case1);

    throw CSTConverterException (literalBool);
  }

  static BoolLiteral *
  convert (ASTNodeMemory &mem, LiteralBoolCase0 *literalBool)
  {
    assert (literalBool != nullptr);
    auto boolValueNContext = literalBool->boolValueN ();
    std::vector<BoolLiteral::ElementType> valueVector{};
    assert (boolValueNContext->value != nullptr);
    const std::string stringValue = boolValueNContext->value->getText ();
    if (stringValue != "null")
      {
        const std::string valueString = boolValueNContext->getText ();
        if (valueString == "0")
          { valueVector.emplace_back (false); }
        else if (valueString == "1")
          { valueVector.emplace_back (true); }
        else
          { throw CSTConverterException (literalBool); }
      }
    else
      { valueVector.emplace_back (nullptr); }
    auto boolLiteral = new BoolLiteral (mem, literalBool);
    boolLiteral->setValue (std::move (valueVector));
    return boolLiteral;
  }

  static BoolLiteral *
  convert (ASTNodeMemory &mem, LiteralBoolCase1 *literalBool)
  {
    assert (literalBool != nullptr);
    using BoolValueN = HorseIRParser::BoolValueNContext;
    const std::vector<BoolValueN *> boolValueNs = literalBool->boolValueN ();
    std::vector<BoolLiteral::ElementType> valueVector{};
    std::transform (
        boolValueNs.cbegin (), boolValueNs.cend (),
        std::back_inserter (valueVector),
        [=] (const BoolValueN *context) -> BoolLiteral::ElementType
        {
          using ElementType = BoolLiteral::ElementType;
          assert (context->value != nullptr);
          const std::string stringValue = context->value->getText ();
          if (stringValue != "null")
            {
              const std::string valueString = context->value->getText ();
              if (valueString == "0") return ElementType (false);
              if (valueString == "1") return ElementType (true);
              throw CSTConverterException (literalBool);
            }
          else
            { return ElementType (nullptr); }
        });
    auto boolLiteral = new BoolLiteral (mem, literalBool);
    boolLiteral->setValue (std::move (valueVector));
    return boolLiteral;
  }

  using LiteralCharContext = HorseIRParser::LiteralCharContext;
  using LiteralCharCase0Context = HorseIRParser::LiteralCharCase0Context;
  using LiteralCharCase1Context = HorseIRParser::LiteralCharCase1Context;
  using LiteralCharCase2Context = HorseIRParser::LiteralCharCase2Context;
  using LiteralCharCase3Context = HorseIRParser::LiteralCharCase3Context;
  using LiteralCharCase4Context = HorseIRParser::LiteralCharCase4Context;
  using LiteralCharCase5Context = HorseIRParser::LiteralCharCase5Context;

  static CharLiteral *
  convert (ASTNodeMemory &mem, LiteralCharContext *literalCharContext)
  {
    assert (literalCharContext != nullptr);
    LiteralCharCase0Context *case0 = nullptr;
    LiteralCharCase1Context *case1 = nullptr;
    LiteralCharCase2Context *case2 = nullptr;
    LiteralCharCase3Context *case3 = nullptr;
    LiteralCharCase4Context *case4 = nullptr;
    LiteralCharCase5Context *case5 = nullptr;

    if ((case0 = dynamic_cast<decltype (case0)>(literalCharContext)) != nullptr)
      return convert (mem, case0);
    if ((case1 = dynamic_cast<decltype (case1)>(literalCharContext)) != nullptr)
      return convert (mem, case1);
    if ((case2 = dynamic_cast<decltype (case2)>(literalCharContext)) != nullptr)
      return convert (mem, case2);
    if ((case3 = dynamic_cast<decltype (case3)>(literalCharContext)) != nullptr)
      return convert (mem, case3);
    if ((case4 = dynamic_cast<decltype (case4)>(literalCharContext)) != nullptr)
      return convert (mem, case4);
    if ((case5 = dynamic_cast<decltype (case5)>(literalCharContext)) != nullptr)
      return convert (mem, case5);

    throw CSTConverterException (literalCharContext);
  }

  static std::vector<std::uint8_t>
  convertEscapedChar (const std::basic_string<char> &rawString)
  {
    std::vector<std::uint8_t> returnVector{};
    for (auto iter = rawString.cbegin (); iter != rawString.cend (); ++iter)
      {
        if (*iter != '\\')
          { returnVector.push_back (static_cast<std::uint8_t>(*iter)); }
        else
          {
            std::advance (iter, 1);
            if (*iter == 'a')
              { returnVector.push_back (0x07); }
            else if (*iter == 'b')
              { returnVector.push_back (0x08); }
            else if (*iter == 'f')
              { returnVector.push_back (0x0C); }
            else if (*iter == 'n')
              { returnVector.push_back (0x0A); }
            else if (*iter == 'r')
              { returnVector.push_back (0x0D); }
            else if (*iter == 't')
              { returnVector.push_back (0x09); }
            else if (*iter == 'v')
              { returnVector.push_back (0x0B); }
            else if (*iter == '\\')
              { returnVector.push_back (0x5C); }
            else if (*iter == '\'')
              { returnVector.push_back (0x27); }
            else if (*iter == '\"')
              { returnVector.push_back (0x22); }
            else if (*iter == '?')
              { returnVector.push_back (0x3F); }
            else if (*iter == 'x')
              {
                std::string convertBuffer;
                auto bufferInserter = std::back_inserter (convertBuffer);
                *bufferInserter = *(++iter);
                *bufferInserter = *(++iter);
                auto ret = std::stoi (convertBuffer, nullptr, 16);
                returnVector.push_back (static_cast<std::uint8_t >(ret));
              }
            else
              {
                std::string convertBuffer;
                auto bufferInserter = std::back_inserter (convertBuffer);
                *bufferInserter = *(iter);
                *bufferInserter = *(++iter);
                *bufferInserter = *(++iter);
                auto ret = std::stoi (convertBuffer, nullptr, 8);
                returnVector.push_back (static_cast<std::uint8_t>(ret));
              }
          }
      }
    return returnVector;
  }

  static bool verifyEscapeChar (const std::string &rawString)
  {
    for (auto iter = rawString.cbegin (); iter != rawString.cend (); ++iter)
      {
        if (*iter != '\\') continue;

        std::advance (iter, 1);
        if (*iter == 'a') continue;
        if (*iter == 'b') continue;
        if (*iter == 'f') continue;
        if (*iter == 'n') continue;
        if (*iter == 'r') continue;
        if (*iter == 't') continue;
        if (*iter == 'v') continue;
        if (*iter == '\\') continue;
        if (*iter == '\'') continue;
        if (*iter == '\"') continue;
        if (*iter == '?') continue;
        if (*iter == 'x')
          {
            std::string convertBuffer;
            auto bufferInserter = std::back_inserter (convertBuffer);
            *bufferInserter = *(++iter);
            *bufferInserter = *(++iter);
            auto ret = std::stoi (convertBuffer, nullptr, 16);
            if (ret >= std::numeric_limits<std::uint8_t>::max ()) return false;
            continue;
          }
        else
          {
            std::string convertBuffer;
            auto bufferInserter = std::back_inserter (convertBuffer);
            *bufferInserter = *(iter);
            *bufferInserter = *(++iter);
            *bufferInserter = *(++iter);
            auto ret = std::stoi (convertBuffer, nullptr, 8);
            if (ret >= std::numeric_limits<std::uint8_t>::max ()) return false;
            continue;
          }
      }
    return true;
  }

  static CharLiteral::ElementType
  convert (HorseIRParser::CharValueContext *charValue)
  {
    assert (charValue != nullptr);
    std::string rawString = charValue->value->getText ();
    rawString = rawString.substr (1, rawString.length () - 2);
    if (!verifyEscapeChar (rawString)) throw CSTConverterException (charValue);
    const std::vector<std::uint8_t> converted = convertEscapedChar (rawString);
    assert (converted.size () == 1);
    return CharLiteral::ElementType (converted[0]);
  }

  static CharLiteral *
  convert (ASTNodeMemory &mem, LiteralCharCase0Context *literalCharContext)
  {
    assert (literalCharContext != nullptr);
    std::vector<CharLiteral::ElementType> valueVector{};
    valueVector.emplace_back (convert (literalCharContext->charValue ()));
    auto charLiteral = new CharLiteral (mem, literalCharContext);
    charLiteral->setValue (std::move (valueVector));
    return charLiteral;
  }

  static CharLiteral *
  convert (ASTNodeMemory &mem, LiteralCharCase1Context *literalCharContext)
  {
    assert (literalCharContext != nullptr);
    const auto tokenPtr = literalCharContext->LITERAL_CHAR_VECTOR ();
    std::string rawString = tokenPtr->getText ();
    rawString = rawString.substr (1, rawString.size () - 2);
    if (!verifyEscapeChar (rawString))
      throw CSTConverterException (literalCharContext);
    std::vector<CharLiteral::ElementType> valueVector{};
    const std::vector<std::uint8_t> converted = convertEscapedChar (rawString);
    std::transform (
        converted.cbegin (), converted.cend (),
        std::back_inserter (valueVector),
        [] (const std::uint8_t value) -> CharLiteral::ElementType
        { return CharLiteral::ElementType (value); });
    auto charLiteral = new CharLiteral (mem, literalCharContext);
    charLiteral->setValue (std::move (valueVector));
    return charLiteral;
  }

  static CharLiteral *
  convert (ASTNodeMemory &mem, LiteralCharCase2Context *literalCharContext)
  {
    assert (literalCharContext != nullptr);
    auto charLiteral = new CharLiteral (mem, literalCharContext);
    return charLiteral;
  }

  static CharLiteral *
  convert (ASTNodeMemory &mem, LiteralCharCase3Context *literalCharContext)
  {
    assert (literalCharContext != nullptr);
    using ParseTree = antlr4::tree::ParseTree;
    using CharValueContext = HorseIRParser::CharValueContext;
    const std::vector<ParseTree *> rawChildren = literalCharContext->children;
    std::vector<ParseTree *> children{};
    children.reserve (rawChildren.size ());
    std::copy_if (
        rawChildren.cbegin (), rawChildren.cend (),
        std::back_inserter (children),
        [] (ParseTree *parseTree) -> bool
        {
          if (dynamic_cast<CharValueContext *>(parseTree) != nullptr)
            { return true; }
          return parseTree->getText () == "null";
        });
    std::vector<CharLiteral::ElementType> valueVector{};
    valueVector.reserve (children.size ());
    std::transform (
        children.cbegin (), children.cend (), std::back_inserter (valueVector),
        [] (ParseTree *parseTree) -> CharLiteral::ElementType
        {
          CharValueContext *value = nullptr;
          if ((value = dynamic_cast<decltype (value)>(parseTree)) != nullptr)
            { return convert (value); }
          else
            { return CharLiteral::ElementType (nullptr); }
        });
    auto charLiteral = new CharLiteral (mem, literalCharContext);
    charLiteral->setValue (std::move (valueVector));
    return charLiteral;
  }

  static CharLiteral *
  convert (ASTNodeMemory &mem, LiteralCharCase4Context *literalCharContext)
  {
    assert (literalCharContext != nullptr);
    using ParseTree = antlr4::tree::ParseTree;
    const std::vector<ParseTree *> rawChildren = literalCharContext->children;
    std::size_t nullElementCount = 0;
    for (auto iter = rawChildren.cbegin (); iter != rawChildren.cend (); ++iter)
      { if ((*iter)->getText () == "null") nullElementCount += 1; }
    std::vector<CharLiteral::ElementType> valueVector{};
    valueVector.reserve (nullElementCount);
    for (std::size_t iter = 0; iter < nullElementCount; ++iter)
      { valueVector.emplace_back (nullptr); }
    auto charLiteral = new CharLiteral (mem, literalCharContext);
    charLiteral->setValue (std::move (valueVector));
    return charLiteral;
  }

  static CharLiteral *
  convert (ASTNodeMemory &mem, LiteralCharCase5Context *literalCharContext)
  {
    assert (literalCharContext != nullptr);
    std::vector<CharLiteral::ElementType> valueVector{};
    valueVector.emplace_back (nullptr);
    auto charLiteral = new CharLiteral (mem, literalCharContext);
    charLiteral->setValue (std::move (valueVector));
    return charLiteral;
  }

  using LiteralStringContext = HorseIRParser::LiteralStringContext;
  using LiteralStringCase0Context = HorseIRParser::LiteralStringCase0Context;
  using LiteralStringCase1Context = HorseIRParser::LiteralStringCase1Context;
  using LiteralStringCase2Context = HorseIRParser::LiteralStringCase2Context;
  using LiteralStringCase3Context = HorseIRParser::LiteralStringCase3Context;
  using LiteralStringCase4Context = HorseIRParser::LiteralStringCase4Context;

  static StringLiteral *
  convert (ASTNodeMemory &mem, LiteralStringContext *stringContext)
  {
    assert (stringContext != nullptr);
    LiteralStringCase0Context *case0 = nullptr;
    LiteralStringCase1Context *case1 = nullptr;
    LiteralStringCase2Context *case2 = nullptr;
    LiteralStringCase3Context *case3 = nullptr;
    LiteralStringCase4Context *case4 = nullptr;

    if ((case0 = dynamic_cast<decltype (case0)>(stringContext)) != nullptr)
      return convert (mem, case0);
    if ((case1 = dynamic_cast<decltype (case1)>(stringContext)) != nullptr)
      return convert (mem, case1);
    if ((case2 = dynamic_cast<decltype (case2)>(stringContext)) != nullptr)
      return convert (mem, case2);
    if ((case3 = dynamic_cast<decltype (case3)>(stringContext)) != nullptr)
      return convert (mem, case3);
    if ((case4 = dynamic_cast<decltype (case4)>(stringContext)) != nullptr)
      return convert (mem, case4);

    throw CSTConverterException (stringContext);
  }

  static StringLiteral::ElementType
  convert (HorseIRParser::StringValueContext *stringValueContext)
  {
    assert (stringValueContext != nullptr);
    std::string rawString = stringValueContext->LITERAL_STRING ()->getText ();
    rawString = rawString.substr (1, rawString.length () - 2);
    if (!verifyEscapeChar (rawString))
      throw CSTConverterException (stringValueContext);
    const std::vector<std::uint8_t> converted = convertEscapedChar (rawString);
    return StringLiteral::ElementType (std::move (converted));
  }

  static StringLiteral *
  convert (ASTNodeMemory &mem, LiteralStringCase0Context *stringContext)
  {
    assert (stringContext != nullptr);
    std::vector<StringLiteral::ElementType> valueVector{};
    valueVector.emplace_back (convert (stringContext->stringValue ()));
    auto stringLiteral = new StringLiteral (mem, stringContext);
    stringLiteral->setValue (std::move (valueVector));
    return stringLiteral;
  }

  static StringLiteral *
  convert (ASTNodeMemory &mem, LiteralStringCase1Context *stringContext)
  {
    assert (stringContext != nullptr);
    auto stringLiteral = new StringLiteral (mem, stringContext);
    return stringLiteral;
  }

  static StringLiteral *
  convert (ASTNodeMemory &mem, LiteralStringCase2Context *stringContext)
  {
    assert (stringContext != nullptr);
    using ParseTree = antlr4::tree::ParseTree;
    using StringValueContext = HorseIRParser::StringValueContext;
    const std::vector<ParseTree *> rawChildren = stringContext->children;
    std::vector<ParseTree *> children{};
    children.reserve (rawChildren.size ());
    std::copy_if (
        rawChildren.cbegin (), rawChildren.cend (),
        std::back_inserter (children),
        [] (ParseTree *parseTree) -> bool
        {
          if (dynamic_cast<StringValueContext *>(parseTree) != nullptr)
            { return true; }
          return parseTree->getText () == "null";
        });
    std::vector<StringLiteral::ElementType> valueVector{};
    valueVector.reserve (children.size ());
    std::transform (
        children.cbegin (), children.cend (), std::back_inserter (valueVector),
        [] (ParseTree *parseTree) -> StringLiteral::ElementType
        {
          StringValueContext *string = nullptr;
          if ((string = dynamic_cast<decltype (string)>(parseTree)) != nullptr)
            { return convert (string); }
          else
            { return StringLiteral::ElementType (nullptr); }
        });
    auto stringLiteral = new StringLiteral (mem, stringContext);
    stringLiteral->setValue (std::move (valueVector));
    return stringLiteral;
  }

  static StringLiteral *
  convert (ASTNodeMemory &mem, LiteralStringCase3Context *stringContext)
  {
    assert (stringContext != nullptr);
    using ParseTree = antlr4::tree::ParseTree;
    std::size_t nullCount = 0;
    const std::vector<ParseTree *> rawChildren = stringContext->children;
    for (auto iter = rawChildren.cbegin (); iter != rawChildren.cend (); ++iter)
      { if ((*iter)->getText () == "null") nullCount += 1; }
    std::vector<StringLiteral::ElementType> valueVector{};
    valueVector.reserve (nullCount);
    for (std::size_t iter = 0; iter < nullCount; ++iter)
      { valueVector.emplace_back (nullptr); }
    auto stringLiteral = new StringLiteral (mem, stringContext);
    stringLiteral->setValue (std::move (valueVector));
    return stringLiteral;
  }

  static StringLiteral *
  convert (ASTNodeMemory &mem, LiteralStringCase4Context *stringContext)
  {
    assert (stringContext != nullptr);
    std::vector<StringLiteral::ElementType> valueVector{};
    valueVector.emplace_back (nullptr);
    auto stringLiteral = new StringLiteral (mem, stringContext);
    stringLiteral->setValue (std::move (valueVector));
    return stringLiteral;
  }

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

    throw CSTConverterException (type);
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
    const auto pTypeEnumContext = cst->typeEnum ();
    const auto elementContext = pTypeEnumContext->element;
    auto enumerationType = new EnumerationType (mem, pTypeEnumContext);

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

    throw CSTConverterException (cst);
  }

  static FunctionType *convert (ASTNodeMemory &mem, TypeFuncCase0Context *cst)
  {
    assert (cst != nullptr);
    auto functionType = new FunctionType (mem, cst);
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
    auto functionType = new FunctionType (mem, cst);
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
    auto functionType = new FunctionType (mem, cst);

    const auto types (cst->type ());
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
    auto functionType = new FunctionType (mem, cst);

    const auto types (cst->type ());
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