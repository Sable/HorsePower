#pragma once

#include <array>
#include <limits>
#include <stdexcept>
#include <regex>
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
        : logic_error ("error occurred during transformation from CST to AST"),
          site (exceptionSite)
    {}

    const antlr4::tree::ParseTree *getExceptionSite () const
    { return site; }

   protected:
    const antlr4::tree::ParseTree *site = nullptr;
  };

  using ASTNodeMemory = ASTNode::ASTNodeMemory;

  using TypeContext = HorseIRParser::TypeContext;
  using TypeCasePrimitiveContext = HorseIRParser::TypeCasePrimitiveContext;
  using TypeCaseWildcardContext = HorseIRParser::TypeCaseWildcardContext;
  using TypeListContext = HorseIRParser::TypeListContext;
  using TypeDictContext = HorseIRParser::TypeDictContext;
  using TypeEnumContext = HorseIRParser::TypeEnumContext;
  using TypeFuncContext = HorseIRParser::TypeFuncContext;

  static Type *convert (ASTNodeMemory &mem, TypeContext *type)
  {
    assert (type != nullptr);
    TypeCasePrimitiveContext *primitive = nullptr;
    TypeCaseWildcardContext *wildcard = nullptr;
    HorseIRParser::TypeCaseListContext *list = nullptr;
    HorseIRParser::TypeCaseDictContext *dict = nullptr;
    HorseIRParser::TypeCaseEnumContext *enume = nullptr;
    HorseIRParser::TypeCaseFuncContext *func = nullptr;

    if ((primitive = dynamic_cast<decltype (primitive)>(type)) != nullptr)
      return convert (mem, primitive);
    if ((wildcard = dynamic_cast<decltype (wildcard)>(type)) != nullptr)
      return convert (mem, wildcard);
    if ((list = dynamic_cast<decltype (list)>(type)) != nullptr)
      return convert (mem, list->typeList ());
    if ((dict = dynamic_cast<decltype (dict)>(type)) != nullptr)
      return convert (mem, dict->typeDict ());
    if ((enume = dynamic_cast<decltype (enume)>(type)) != nullptr)
      return convert (mem, enume->typeEnum ());
    if ((func = dynamic_cast<decltype (func)>(type)) != nullptr)
      return convert (mem, func->typeFunc ());

    throw CSTConverterException (type);
  }

  static WildcardType *
  convert (ASTNodeMemory &mem, TypeCaseWildcardContext *cst)
  {
    assert (cst != nullptr);
    auto wildcardType = mem.alloc<WildcardType> (cst);
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

    auto primitiveType = mem.alloc<PrimitiveType> (cst);
    primitiveType->setPrimitiveClass (iter->second);

    return (primitiveType);
  }

  static ListType *convert (ASTNodeMemory &mem, TypeListContext *cst)
  {
    assert (cst != nullptr);
    const auto elementTypeContext = cst->element;
    auto listType = mem.alloc<ListType> (cst);

    Type *elementType = convert (mem, elementTypeContext);
    listType->setElementType (elementType);

    return listType;
  }

  static DictionaryType *convert (ASTNodeMemory &mem, TypeDictContext *cst)
  {
    assert (cst != nullptr);
    const auto keyTypeContext = cst->key;
    const auto valueTypeContext = cst->value;
    auto dictionaryType = mem.alloc<DictionaryType> (cst);

    Type *keyType = convert (mem, keyTypeContext);
    dictionaryType->setKeyType (keyType);

    Type *valueType = convert (mem, valueTypeContext);
    dictionaryType->setValueType (valueType);

    return dictionaryType;
  }

  static EnumerationType *convert (ASTNodeMemory &mem, TypeEnumContext *cst)
  {
    assert (cst != nullptr);
    const auto elementContext = cst->element;
    auto enumerationType = mem.alloc<EnumerationType> (cst);

    Type *elementType = convert (mem, elementContext);
    enumerationType->setElementType (elementType);

    return enumerationType;
  }

  using TypeFuncCase0Context = HorseIRParser::TypeFuncCase0Context;
  using TypeFuncCase1Context = HorseIRParser::TypeFuncCase1Context;
  using TypeFuncCase2Context = HorseIRParser::TypeFuncCase2Context;
  using TypeFuncCase3Context = HorseIRParser::TypeFuncCase3Context;

  static FunctionType *convert (ASTNodeMemory &mem, TypeFuncContext *cst)
  {
    assert (cst != nullptr);
    TypeFuncCase0Context *case0 = nullptr;
    TypeFuncCase1Context *case1 = nullptr;
    TypeFuncCase2Context *case2 = nullptr;
    TypeFuncCase3Context *case3 = nullptr;

    if ((case0 = dynamic_cast<decltype (case0)>(cst)) != nullptr)
      return convert (mem, case0);
    if ((case1 = dynamic_cast<decltype (case1)>(cst)) != nullptr)
      return convert (mem, case1);
    if ((case2 = dynamic_cast<decltype (case2)>(cst)) != nullptr)
      return convert (mem, case2);
    if ((case3 = dynamic_cast<decltype (case3)>(cst)) != nullptr)
      return convert (mem, case3);

    throw CSTConverterException (cst);
  }

  static FunctionType *convert (ASTNodeMemory &mem, TypeFuncCase0Context *cst)
  {
    assert (cst != nullptr);
    auto functionType = mem.alloc<FunctionType> (cst);
    auto returnTypeContext = cst->type ();

    Type *returnType = convert (mem, returnTypeContext);
    functionType->setReturnType (returnType);

    functionType->setIsFlexible (false);
    return functionType;
  }

  static FunctionType *convert (ASTNodeMemory &mem, TypeFuncCase1Context *cst)
  {
    assert (cst != nullptr);
    auto functionType = mem.alloc<FunctionType> (cst);
    auto returnTypeContext = cst->type ();

    Type *returnType = convert (mem, returnTypeContext);
    functionType->setReturnType (returnType);

    functionType->setIsFlexible (true);
    return functionType;
  }

  static FunctionType *convert (ASTNodeMemory &mem, TypeFuncCase2Context *cst)
  {
    assert (cst != nullptr);
    auto functionType = mem.alloc<FunctionType> (cst);

    const auto types (cst->type ());
    std::vector<Type *> parameterTypes{};
    std::transform (
        types.cbegin (), std::prev (types.cend ()),
        std::back_inserter (parameterTypes),
        [&] (TypeContext *typeContext) -> Type *
        {
          Type *retType = convert (mem, typeContext);
          return retType;
        });

    TypeContext *returnContext = *(std::prev (types.cend ()));
    Type *returnType = convert (mem, returnContext);

    functionType->setParameterTypes (std::move (parameterTypes));
    functionType->setReturnType (returnType);
    functionType->setIsFlexible (false);
    return functionType;
  }

  static FunctionType *convert (ASTNodeMemory &mem, TypeFuncCase3Context *cst)
  {
    assert (cst != nullptr);
    auto functionType = mem.alloc<FunctionType> (cst);

    const auto types (cst->type ());
    std::vector<Type *> parameterTypes{};
    std::transform (
        types.cbegin (), std::prev (types.cend ()),
        std::back_inserter (parameterTypes),
        [&] (TypeContext *typeContext) -> Type *
        {
          Type *retType = convert (mem, typeContext);
          return retType;
        });

    TypeContext *returnContext = *(std::prev (types.cend ()));
    Type *returnType = convert (mem, returnContext);

    functionType->setParameterTypes (std::move (parameterTypes));
    functionType->setReturnType (returnType);
    functionType->setIsFlexible (true);
    return functionType;
  }

  using LiteralContext = HorseIRParser::LiteralContext;
  using LiteralBoolContext = HorseIRParser::LiteralBoolContext;
  using LiteralCharContext = HorseIRParser::LiteralCharContext;
  using LiteralStringContext = HorseIRParser::LiteralStringContext;
  using LiteralIntegerContext = HorseIRParser::LiteralIntegerContext;
  using LiteralFloatContext = HorseIRParser::LiteralFloatContext;
  using LiteralComplexContext = HorseIRParser::LiteralComplexContext;
  using LiteralSymbolContext = HorseIRParser::LiteralSymbolContext;
  using LiteralTMonthContext = HorseIRParser::LiteralTMonthContext;
  using LiteralTDateContext = HorseIRParser::LiteralTDateContext;
  using LiteralTDateTimeContext = HorseIRParser::LiteralTDateTimeContext;
  using LiteralTMinuteContext = HorseIRParser::LiteralTMinuteContext;
  using LiteralTSecondContext = HorseIRParser::LiteralTSecondContext;
  using LiteralTTimeContext = HorseIRParser::LiteralTTimeContext;
  using LiteralFunctionContext = HorseIRParser::LiteralFunctionContext;
  using LiteralListContext = HorseIRParser::LiteralListContext;
  using LiteralDictContext = HorseIRParser::LiteralDictContext;
  using LiteralTableContext = HorseIRParser::LiteralTableContext;
  using LiteralKeyTableContext = HorseIRParser::LiteralKTableContext;

  static Literal *
  convert (ASTNodeMemory &mem, LiteralContext *literal)
  {
    assert (literal != nullptr);
    if (literal->literalBool () != nullptr)
      { return convert (mem, literal->literalBool ()); }
    if (literal->literalChar () != nullptr)
      { return convert (mem, literal->literalChar ()); }
    if (literal->literalString () != nullptr)
      { return convert (mem, literal->literalString ()); }
    if (literal->literalInteger () != nullptr)
      { return convert (mem, literal->literalInteger ()); }
    if (literal->literalFloat () != nullptr)
      { return convert (mem, literal->literalFloat ()); }
    if (literal->literalComplex () != nullptr)
      { return convert (mem, literal->literalComplex ()); }
    if (literal->literalSymbol () != nullptr)
      { return convert (mem, literal->literalSymbol ()); }
    if (literal->literalTDate () != nullptr)
      { return convert (mem, literal->literalTDate ()); }
    if (literal->literalTDateTime () != nullptr)
      { return convert (mem, literal->literalTDateTime ()); }
    if (literal->literalTMinute () != nullptr)
      { return convert (mem, literal->literalTMinute ()); }
    if (literal->literalTMonth () != nullptr)
      { return convert (mem, literal->literalTMonth ()); }
    if (literal->literalTSecond () != nullptr)
      { return convert (mem, literal->literalTSecond ()); }
    if (literal->literalTTime () != nullptr)
      { return convert (mem, literal->literalTTime ()); }
    if (literal->literalFunction () != nullptr)
      { return convert (mem, literal->literalFunction ()); }
    if (literal->literalList () != nullptr)
      { return convert (mem, literal->literalList ()); }
    if (literal->literalDict () != nullptr)
      { return convert (mem, literal->literalDict ()); }
    if (literal->literalTable () != nullptr)
      { return convert (mem, literal->literalTable ()); }
    if (literal->literalKTable () != nullptr)
      { return convert (mem, literal->literalKTable ()); }

    throw CSTConverterException (literal);
  }

  using LiteralBoolCase0 = HorseIRParser::LiteralBoolCase0Context;
  using LiteralBoolCase1 = HorseIRParser::LiteralBoolCase1Context;

  static BoolLiteral *convert (ASTNodeMemory &mem, LiteralBoolContext *literal)
  {
    assert (literal != nullptr);
    LiteralBoolCase0 *case0 = nullptr;
    LiteralBoolCase1 *case1 = nullptr;

    if ((case0 = dynamic_cast<decltype (case0)>(literal)) != nullptr)
      return convert (mem, case0);
    if ((case1 = dynamic_cast<decltype (case1)>(literal)) != nullptr)
      return convert (mem, case1);

    throw CSTConverterException (literal);
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
    auto boolLiteral = mem.alloc<BoolLiteral> (literalBool);
    boolLiteral->setValue (std::move (valueVector));
    auto primitiveType = mem.alloc<PrimitiveType> (literalBool);
    primitiveType->setPrimitiveClass (PrimitiveType::PrimitiveClass::Bool);
    boolLiteral->setLiteralType (primitiveType);
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
    auto boolLiteral = mem.alloc<BoolLiteral> (literalBool);
    boolLiteral->setValue (std::move (valueVector));
    auto primitiveType = mem.alloc<PrimitiveType> (literalBool);
    primitiveType->setPrimitiveClass (PrimitiveType::PrimitiveClass::Bool);
    boolLiteral->setLiteralType (primitiveType);
    return boolLiteral;
  }

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
        std::string convertBuffer;
        auto bufferInserter = std::back_inserter (convertBuffer);
        *bufferInserter = *(iter);
        *bufferInserter = *(++iter);
        *bufferInserter = *(++iter);
        auto ret = std::stoi (convertBuffer, nullptr, 8);
        if (ret >= std::numeric_limits<std::uint8_t>::max ()) return false;
      }
    return true;
  }

  static CharLiteral::ElementType
  convertCharValue (HorseIRParser::CharValueContext *charValue)
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
    auto element = convertCharValue (literalCharContext->charValue ());
    valueVector.emplace_back (std::move (element));
    auto charLiteral = mem.alloc<CharLiteral> (literalCharContext);
    charLiteral->setValue (std::move (valueVector));
    auto primitiveType = mem.alloc<PrimitiveType> (literalCharContext);
    primitiveType->setPrimitiveClass (PrimitiveType::PrimitiveClass::Character);
    charLiteral->setLiteralType (primitiveType);
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
    auto charLiteral = mem.alloc<CharLiteral> (literalCharContext);
    charLiteral->setValue (std::move (valueVector));
    auto primitiveType = mem.alloc<PrimitiveType> (literalCharContext);
    primitiveType->setPrimitiveClass (PrimitiveType::PrimitiveClass::Character);
    charLiteral->setLiteralType (primitiveType);
    return charLiteral;
  }

  static CharLiteral *
  convert (ASTNodeMemory &mem, LiteralCharCase2Context *literalCharContext)
  {
    assert (literalCharContext != nullptr);
    auto charLiteral = mem.alloc<CharLiteral> (literalCharContext);
    auto primitiveType = mem.alloc<PrimitiveType> (literalCharContext);
    primitiveType->setPrimitiveClass (PrimitiveType::PrimitiveClass::Character);
    charLiteral->setLiteralType (primitiveType);
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
            { return convertCharValue (value); }
          return CharLiteral::ElementType (nullptr);
        });
    auto charLiteral = mem.alloc<CharLiteral> (literalCharContext);
    charLiteral->setValue (std::move (valueVector));
    auto primitiveType = mem.alloc<PrimitiveType> (literalCharContext);
    primitiveType->setPrimitiveClass (PrimitiveType::PrimitiveClass::Character);
    charLiteral->setLiteralType (primitiveType);
    return charLiteral;
  }

  static CharLiteral *
  convert (ASTNodeMemory &mem, LiteralCharCase4Context *literalCharContext)
  {
    assert (literalCharContext != nullptr);
    const auto nullTokens = literalCharContext->NULL_TOKEN ();
    std::vector<CharLiteral::ElementType> valueVector{};
    valueVector.reserve (nullTokens.size ());
    for (std::size_t iter = 0; iter < nullTokens.size (); ++iter)
      { valueVector.emplace_back (nullptr); }
    auto charLiteral = mem.alloc<CharLiteral> (literalCharContext);
    charLiteral->setValue (std::move (valueVector));
    auto primitiveType = mem.alloc<PrimitiveType> (literalCharContext);
    primitiveType->setPrimitiveClass (PrimitiveType::PrimitiveClass::Character);
    charLiteral->setLiteralType (primitiveType);
    return charLiteral;
  }

  static CharLiteral *
  convert (ASTNodeMemory &mem, LiteralCharCase5Context *literalCharContext)
  {
    assert (literalCharContext != nullptr);
    std::vector<CharLiteral::ElementType> valueVector{};
    valueVector.emplace_back (nullptr);
    auto charLiteral = mem.alloc<CharLiteral> (literalCharContext);
    charLiteral->setValue (std::move (valueVector));
    auto primitiveType = mem.alloc<PrimitiveType> (literalCharContext);
    primitiveType->setPrimitiveClass (PrimitiveType::PrimitiveClass::Character);
    charLiteral->setLiteralType (primitiveType);
    return charLiteral;
  }

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
  convertStringValue (HorseIRParser::StringValueContext *stringValueContext)
  {
    assert (stringValueContext != nullptr);
    std::string rawString = stringValueContext->LITERAL_STRING ()->getText ();
    rawString = rawString.substr (1, rawString.length () - 2);
    if (!verifyEscapeChar (rawString))
      throw CSTConverterException (stringValueContext);
    std::vector<std::uint8_t> converted = convertEscapedChar (rawString);
    return StringLiteral::ElementType (std::move (converted));
  }

  static StringLiteral *
  convert (ASTNodeMemory &mem, LiteralStringCase0Context *stringContext)
  {
    assert (stringContext != nullptr);
    std::vector<StringLiteral::ElementType> valueVector{};
    auto element = convertStringValue (stringContext->stringValue ());
    valueVector.emplace_back (element);
    auto stringLiteral = mem.alloc<StringLiteral> (stringContext);
    stringLiteral->setValue (std::move (valueVector));
    using PrimitiveClass = PrimitiveType::PrimitiveClass;
    auto primitiveType = mem.alloc<PrimitiveType> (stringContext);
    primitiveType->setPrimitiveClass (PrimitiveClass::String);
    stringLiteral->setLiteralType (primitiveType);
    return stringLiteral;
  }

  static StringLiteral *
  convert (ASTNodeMemory &mem, LiteralStringCase1Context *stringContext)
  {
    assert (stringContext != nullptr);
    auto stringLiteral = mem.alloc<StringLiteral> (stringContext);
    using PrimitiveClass = PrimitiveType::PrimitiveClass;
    auto primitiveType = mem.alloc<PrimitiveType> (stringContext);
    primitiveType->setPrimitiveClass (PrimitiveClass::String);
    stringLiteral->setLiteralType (primitiveType);
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
            { return convertStringValue (string); }
          return StringLiteral::ElementType (nullptr);
        });
    auto stringLiteral = mem.alloc<StringLiteral> (stringContext);
    stringLiteral->setValue (std::move (valueVector));
    using PrimitiveClass = PrimitiveType::PrimitiveClass;
    auto primitiveType = mem.alloc<PrimitiveType> (stringContext);
    primitiveType->setPrimitiveClass (PrimitiveClass::String);
    stringLiteral->setLiteralType (primitiveType);
    return stringLiteral;
  }

  static StringLiteral *
  convert (ASTNodeMemory &mem, LiteralStringCase3Context *stringContext)
  {
    assert (stringContext != nullptr);
    const auto nullTokens = stringContext->NULL_TOKEN ();
    std::vector<StringLiteral::ElementType> valueVector{};
    valueVector.reserve (nullTokens.size ());
    for (std::size_t iter = 0; iter < nullTokens.size (); ++iter)
      { valueVector.emplace_back (nullptr); }
    auto stringLiteral = mem.alloc<StringLiteral> (stringContext);
    stringLiteral->setValue (std::move (valueVector));
    using PrimitiveClass = PrimitiveType::PrimitiveClass;
    auto primitiveType = mem.alloc<PrimitiveType> (stringContext);
    primitiveType->setPrimitiveClass (PrimitiveClass::String);
    stringLiteral->setLiteralType (primitiveType);
    return stringLiteral;
  }

  static StringLiteral *
  convert (ASTNodeMemory &mem, LiteralStringCase4Context *stringContext)
  {
    assert (stringContext != nullptr);
    std::vector<StringLiteral::ElementType> valueVector{};
    valueVector.emplace_back (nullptr);
    auto stringLiteral = mem.alloc<StringLiteral> (stringContext);
    stringLiteral->setValue (std::move (valueVector));
    using PrimitiveClass = PrimitiveType::PrimitiveClass;
    auto primitiveType = mem.alloc<PrimitiveType> (stringContext);
    primitiveType->setPrimitiveClass (PrimitiveClass::String);
    stringLiteral->setLiteralType (primitiveType);
    return stringLiteral;
  }

  using LiteralIntegerCase0Context = HorseIRParser::LiteralIntegerCase0Context;
  using LiteralIntegerCase1Context = HorseIRParser::LiteralIntegerCase1Context;

  static Literal *
  convert (ASTNodeMemory &mem, LiteralIntegerContext *literal)
  {
    assert (literal != nullptr);
    LiteralIntegerCase0Context *case0 = nullptr;
    LiteralIntegerCase1Context *case1 = nullptr;

    if ((case0 = dynamic_cast<decltype (case0)>(literal)) != nullptr)
      return convert (mem, case0);
    if ((case1 = dynamic_cast<decltype (case1)>(literal)) != nullptr)
      return convert (mem, case1);

    throw CSTConverterException (literal);
  }

  template<class T, typename = std::enable_if_t<std::is_signed<T>::value>>
  static typename VectorLiteral<T>::ElementType
  convertIntValueN (HorseIRParser::IntValueNContext *context)
  {
    assert (context != nullptr);
    using ElementType = typename VectorLiteral<T>::ElementType;
    if (context->NULL_TOKEN () != nullptr)
      { return ElementType (nullptr); }
    int sign = 1;
    const auto signToken = context->op;
    if (signToken != nullptr)
      {
        if (signToken->getText () == "-") sign = -1;
      }
    const std::string valueString = context->LITERAL_INTEGER ()->getText ();
    try
      {
        std::size_t pos = 0;
        signed long long rawValue = std::stoll (valueString, &pos, 10);
        if (pos != valueString.length ()) throw CSTConverterException (context);
        if (rawValue < std::numeric_limits<T>::min ())
          { throw CSTConverterException (context); }
        if (rawValue > std::numeric_limits<T>::max ())
          { throw CSTConverterException (context); }
        auto castedValue = static_cast<T>(rawValue);
        return ElementType (sign * castedValue);
      }
    catch (const std::invalid_argument &exception)
      { throw CSTConverterException (context); }
    catch (const std::out_of_range &exception)
      { throw CSTConverterException (context); }
  }

  static Literal *
  convert (ASTNodeMemory &mem, LiteralIntegerCase0Context *literal)
  {
    assert (literal != nullptr);
    using IntValueNContext = HorseIRParser::IntValueNContext;
    const std::string typeString = literal->typeToken->getText ();

    if (typeString == "i8")
      {
        auto integer8Literal = mem.alloc<Integer8Literal> (literal);
        std::vector<Integer8Literal::ElementType> valueVector{};
        auto element = convertIntValueN<std::int8_t> (literal->intValueN ());
        valueVector.emplace_back (std::move (element));
        integer8Literal->setValue (std::move (valueVector));
        using PrimitiveClass = PrimitiveType::PrimitiveClass;
        auto primitiveType = mem.alloc<PrimitiveType> (literal);
        primitiveType->setPrimitiveClass (PrimitiveClass::Integer8);
        integer8Literal->setLiteralType (primitiveType);
        return integer8Literal;
      }
    if (typeString == "i16")
      {
        auto integer16Literal = mem.alloc<Integer16Literal> (literal);
        std::vector<Integer16Literal::ElementType> valueVector{};
        auto element = convertIntValueN<std::int16_t> (literal->intValueN ());
        valueVector.emplace_back (std::move (element));
        integer16Literal->setValue (std::move (valueVector));
        using PrimitiveClass = PrimitiveType::PrimitiveClass;
        auto primitiveType = mem.alloc<PrimitiveType> (literal);
        primitiveType->setPrimitiveClass (PrimitiveClass::Integer16);
        integer16Literal->setLiteralType (primitiveType);
        return integer16Literal;
      }
    if (typeString == "i32")
      {
        auto integer32Literal = mem.alloc<Integer32Literal> (literal);
        std::vector<Integer32Literal::ElementType> valueVector{};
        auto element = convertIntValueN<std::int32_t> (literal->intValueN ());
        valueVector.emplace_back (std::move (element));
        integer32Literal->setValue (std::move (valueVector));
        using PrimitiveClass = PrimitiveType::PrimitiveClass;
        auto primitiveType = mem.alloc<PrimitiveType> (literal);
        primitiveType->setPrimitiveClass (PrimitiveClass::Integer32);
        integer32Literal->setLiteralType (primitiveType);
        return integer32Literal;
      }
    if (typeString == "i64")
      {
        auto integer64Literal = mem.alloc<Integer64Literal> (literal);
        std::vector<Integer64Literal::ElementType> valueVector{};
        auto element = convertIntValueN<std::int64_t> (literal->intValueN ());
        valueVector.emplace_back (std::move (element));
        integer64Literal->setValue (std::move (valueVector));
        using PrimitiveClass = PrimitiveType::PrimitiveClass;
        auto primitiveType = mem.alloc<PrimitiveType> (literal);
        primitiveType->setPrimitiveClass (PrimitiveClass::Integer64);
        integer64Literal->setLiteralType (primitiveType);
        return integer64Literal;
      }
    throw CSTConverterException (literal);
  }

  static Literal *
  convert (ASTNodeMemory &mem, LiteralIntegerCase1Context *literal)
  {
    assert (literal != nullptr);
    using IntValueNContext = HorseIRParser::IntValueNContext;
    const std::string typeString = literal->typeToken->getText ();
    const std::vector<IntValueNContext *> rawContexts = literal->intValueN ();
    if (typeString == "i8")
      {
        auto integer8Literal = mem.alloc<Integer8Literal> (literal);
        std::vector<Integer8Literal::ElementType> valueVector{};
        valueVector.reserve (rawContexts.size ());
        std::transform (
            rawContexts.cbegin (), rawContexts.cend (),
            std::back_inserter (valueVector),
            [] (IntValueNContext *context) -> Integer8Literal::ElementType
            { return convertIntValueN<std::int8_t> (context); });
        integer8Literal->setValue (std::move (valueVector));
        using PrimitiveClass = PrimitiveType::PrimitiveClass;
        auto primitiveType = mem.alloc<PrimitiveType> (literal);
        primitiveType->setPrimitiveClass (PrimitiveClass::Integer8);
        integer8Literal->setLiteralType (primitiveType);
        return integer8Literal;
      }
    if (typeString == "i16")
      {
        auto integer16Literal = mem.alloc<Integer16Literal> (literal);
        std::vector<Integer16Literal::ElementType> valueVector{};
        valueVector.reserve (rawContexts.size ());
        std::transform (
            rawContexts.cbegin (), rawContexts.cend (),
            std::back_inserter (valueVector),
            [] (IntValueNContext *context) -> Integer16Literal::ElementType
            { return convertIntValueN<std::int16_t> (context); });
        integer16Literal->setValue (std::move (valueVector));
        using PrimitiveClass = PrimitiveType::PrimitiveClass;
        auto primitiveType = mem.alloc<PrimitiveType> (literal);
        primitiveType->setPrimitiveClass (PrimitiveClass::Integer16);
        integer16Literal->setLiteralType (primitiveType);
        return integer16Literal;
      }
    if (typeString == "i32")
      {
        auto integer32Literal = mem.alloc<Integer32Literal> (literal);
        std::vector<Integer32Literal::ElementType> valueVector{};
        valueVector.reserve (rawContexts.size ());
        std::transform (
            rawContexts.cbegin (), rawContexts.cend (),
            std::back_inserter (valueVector),
            [] (IntValueNContext *context) -> Integer32Literal::ElementType
            { return convertIntValueN<std::int32_t> (context); });
        integer32Literal->setValue (std::move (valueVector));
        using PrimitiveClass = PrimitiveType::PrimitiveClass;
        auto primitiveType = mem.alloc<PrimitiveType> (literal);
        primitiveType->setPrimitiveClass (PrimitiveClass::Integer32);
        integer32Literal->setLiteralType (primitiveType);
        return integer32Literal;
      }
    if (typeString == "i64")
      {
        auto integer64Literal = mem.alloc<Integer64Literal> (literal);
        std::vector<Integer64Literal::ElementType> valueVector{};
        valueVector.reserve (rawContexts.size ());
        std::transform (
            rawContexts.cbegin (), rawContexts.cend (),
            std::back_inserter (valueVector),
            [] (IntValueNContext *context) -> Integer64Literal::ElementType
            { return convertIntValueN<std::int64_t> (context); });
        integer64Literal->setValue (std::move (valueVector));
        using PrimitiveClass = PrimitiveType::PrimitiveClass;
        auto primitiveType = mem.alloc<PrimitiveType> (literal);
        primitiveType->setPrimitiveClass (PrimitiveClass::Integer64);
        integer64Literal->setLiteralType (primitiveType);
        return integer64Literal;
      }
    throw CSTConverterException (literal);
  }

  using LiteralFloatCase0Context = HorseIRParser::LiteralFloatCase0Context;
  using LiteralFloatCase1Context = HorseIRParser::LiteralFloatCase1Context;

  static Literal *
  convert (ASTNodeMemory &mem, LiteralFloatContext *literal)
  {
    LiteralFloatCase0Context *case0 = nullptr;
    LiteralFloatCase1Context *case1 = nullptr;

    if ((case0 = dynamic_cast<decltype (case0)>(literal)) != nullptr)
      { return convert (mem, case0); }
    if ((case1 = dynamic_cast<decltype (case1)>(literal)) != nullptr)
      { return convert (mem, case1); }

    throw CSTConverterException (literal);
  }

  template<class T, typename = std::enable_if<std::is_floating_point<T>::value>>
  static typename VectorLiteral<T>::ElementType
  convertFloatValueN (
      HorseIRParser::FloatValueNContext *context,
      T (*func) (const std::string &, std::size_t *))
  {
    assert (context != nullptr);
    assert (func != nullptr);
    using ElementType = typename VectorLiteral<T>::ElementType;
    if (context->NULL_TOKEN () != nullptr) return ElementType (nullptr);
    std::ostringstream stream;
    const auto opToken = context->op;
    if (opToken != nullptr) stream << opToken->getText ();
    const std::string valueString = context->value->getText ();
    stream << valueString;
    try
      {
        const std::string streamStream = stream.str ();
        size_t pos = 0;
        T converted = func (streamStream, &pos);
        if (pos != streamStream.length ())
          { throw CSTConverterException (context); }
        return ElementType (converted);
      }
    catch (const std::invalid_argument &exception)
      { throw CSTConverterException (context); }
    catch (const std::out_of_range &exception)
      { throw CSTConverterException (context); }
  }

  static Literal *
  convert (ASTNodeMemory &mem, LiteralFloatCase0Context *literal)
  {
    assert (literal != nullptr);
    const std::string typeString = literal->typeToken->getText ();
    if (typeString == "f32")
      {

        auto fp32Literal = mem.alloc<FP32Literal> (literal);
        std::vector<FP32Literal::ElementType> valueVector{};
        auto element = convertFloatValueN<float> (literal->floatValueN (),
                                                  std::stof);
        valueVector.emplace_back (std::move (element));
        fp32Literal->setValue (std::move (valueVector));
        using PrimitiveClass = PrimitiveType::PrimitiveClass;
        auto primitiveType = mem.alloc<PrimitiveType> (literal);
        primitiveType->setPrimitiveClass (PrimitiveClass::FP32);
        fp32Literal->setLiteralType (primitiveType);
        return fp32Literal;
      }
    if (typeString == "f64")
      {
        auto fp64Literal = mem.alloc<FP64Literal> (literal);
        std::vector<FP64Literal::ElementType> valueVector{};
        auto element = convertFloatValueN<double> (literal->floatValueN (),
                                                   std::stod);
        valueVector.emplace_back (std::move (element));
        fp64Literal->setValue (std::move (valueVector));
        using PrimitiveClass = PrimitiveType::PrimitiveClass;
        auto primitiveType = mem.alloc<PrimitiveType> (literal);
        primitiveType->setPrimitiveClass (PrimitiveClass::FP64);
        fp64Literal->setLiteralType (primitiveType);
        return fp64Literal;
      }
    throw CSTConverterException (literal);
  }

  static Literal *
  convert (ASTNodeMemory &mem, LiteralFloatCase1Context *literal)
  {
    assert (literal != nullptr);
    using FloatValueNContext = HorseIRParser::FloatValueNContext;
    const std::string typeString = literal->typeToken->getText ();
    const std::vector<FloatValueNContext *> valueContexts =
        literal->floatValueN ();
    if (typeString == "f32")
      {
        auto fp32Literal = mem.alloc<FP32Literal> (literal);
        std::vector<FP32Literal::ElementType> valueVector{};
        valueVector.reserve (valueContexts.size ());
        std::transform (
            valueContexts.cbegin (), valueContexts.cend (),
            std::back_inserter (valueVector),
            [] (FloatValueNContext *context) -> FP32Literal::ElementType
            { return convertFloatValueN<float> (context, std::stof); });
        fp32Literal->setValue (std::move (valueVector));
        using PrimitiveClass = PrimitiveType::PrimitiveClass;
        auto primitiveType = mem.alloc<PrimitiveType> (literal);
        primitiveType->setPrimitiveClass (PrimitiveClass::FP32);
        fp32Literal->setLiteralType (primitiveType);
        return fp32Literal;
      }
    if (typeString == "f64")
      {
        auto fp64Literal = mem.alloc<FP64Literal> (literal);
        std::vector<FP64Literal::ElementType> valueVector{};
        valueVector.reserve (valueContexts.size ());
        std::transform (
            valueContexts.cbegin (), valueContexts.cend (),
            std::back_inserter (valueVector),
            [] (FloatValueNContext *context) -> FP64Literal::ElementType
            { return convertFloatValueN<double> (context, std::stod); });
        fp64Literal->setValue (std::move (valueVector));
        using PrimitiveClass = PrimitiveType::PrimitiveClass;
        auto primitiveType = mem.alloc<PrimitiveType> (literal);
        primitiveType->setPrimitiveClass (PrimitiveClass::FP64);
        fp64Literal->setLiteralType (primitiveType);
        return fp64Literal;
      };
    throw CSTConverterException (literal);
  }

  using LiteralComplexCase0Context = HorseIRParser::LiteralComplexCase0Context;
  using LiteralComplexCase1Context = HorseIRParser::LiteralComplexCase1Context;
  using ComplexValueNContext = HorseIRParser::ComplexValueNContext;
  using ComplexValueNCase0Context = HorseIRParser::ComplexValueNCase0Context;
  using ComplexValueNCase1Context = HorseIRParser::ComplexValueNCase1Context;
  using ComplexValueNCase2Context = HorseIRParser::ComplexValueNCase2Context;
  using ComplexValueNCase3Context = HorseIRParser::ComplexValueNCase3Context;
  using ComplexValueNCase4Context = HorseIRParser::ComplexValueNCase4Context;

  static ComplexLiteral *
  convert (ASTNodeMemory &mem, LiteralComplexContext *literal)
  {
    assert (literal != nullptr);
    LiteralComplexCase0Context *case0 = nullptr;
    LiteralComplexCase1Context *case1 = nullptr;

    if ((case0 = dynamic_cast<decltype (case0)>(literal)) != nullptr)
      { return convert (mem, case0); }
    if ((case1 = dynamic_cast<decltype (case1)>(literal)) != nullptr)
      { return convert (mem, case1); }

    throw CSTConverterException (literal);
  }

  static ComplexLiteral::ElementType
  convertComplexValueN (ComplexValueNContext *valueContext)
  {
    assert (valueContext != nullptr);
    ComplexValueNCase0Context *case0 = nullptr;
    ComplexValueNCase1Context *case1 = nullptr;
    ComplexValueNCase2Context *case2 = nullptr;
    ComplexValueNCase3Context *case3 = nullptr;
    ComplexValueNCase4Context *case4 = nullptr;

    if ((case0 = dynamic_cast<decltype (case0)>(valueContext)) != nullptr)
      { return convertComplexValueN (case0); }
    if ((case1 = dynamic_cast<decltype (case1)>(valueContext)) != nullptr)
      { return convertComplexValueN (case1); }
    if ((case2 = dynamic_cast<decltype (case2)>(valueContext)) != nullptr)
      { return convertComplexValueN (case2); }
    if ((case3 = dynamic_cast<decltype (case3)>(valueContext)) != nullptr)
      { return convertComplexValueN (case3); }
    if ((case4 = dynamic_cast<decltype (case4)>(valueContext)) != nullptr)
      { return convertComplexValueN (case4); }

    throw CSTConverterException (valueContext);
  }

  static ComplexLiteral::ElementType
  convertComplexValueN (ComplexValueNCase0Context *valueContext)
  {
    assert (valueContext != nullptr);
    int sign = 1;
    if (valueContext->realOp != nullptr)
      {
        const std::string opString = valueContext->realOp->getText ();
        if (opString == "+") sign = 1;
        if (opString == "-") sign = -1;
      }
    try
      {
        std::size_t pos = 0;
        const std::string realString = valueContext->real->getText ();
        double realValue = std::stod (realString, &pos);
        if (pos != realString.length ())
          { throw CSTConverterException (valueContext); }
        std::complex<double> element (sign * realValue, 0.0);
        return ComplexLiteral::ElementType (element);
      }
    catch (const std::invalid_argument &exception)
      { throw CSTConverterException (valueContext); }
    catch (const std::out_of_range &exception)
      { throw CSTConverterException (valueContext); }
  }

  static ComplexLiteral::ElementType
  convertComplexValueN (ComplexValueNCase1Context *valueContext)
  {
    assert (valueContext != nullptr);
    int sign = 1;
    if (valueContext->imOp != nullptr)
      {
        const std::string opString = valueContext->imOp->getText ();
        if (opString == "+") sign = 1;
        if (opString == "-") sign = -1;
      }
    try
      {
        std::size_t pos = 0;
        const std::string imString = valueContext->im->getText ();
        double imValue = std::stod (imString, &pos);
        if (pos != imString.length ())
          { throw CSTConverterException (valueContext); }
        std::complex<double> element (0.0, sign * imValue);
        return ComplexLiteral::ElementType (element);
      }
    catch (const std::invalid_argument &exception)
      { throw CSTConverterException (valueContext); }
    catch (const std::out_of_range &exception)
      { throw CSTConverterException (valueContext); }
  }

  static ComplexLiteral::ElementType
  convertComplexValueN (ComplexValueNCase2Context *valueContext)
  {
    assert (valueContext != nullptr);
    int realSign = 1, imSign = 1;
    if (valueContext->realOp != nullptr)
      {
        const std::string realOpString = valueContext->realOp->getText ();
        if (realOpString == "+") realSign = 1;
        if (realOpString == "-") realSign = -1;
      }
    const std::string imOpString = valueContext->imOp->getText ();
    if (imOpString == "+") imSign = 1;
    if (imOpString == "-") imSign = -1;

    double realValue, imValue = 1.0;
    try
      {
        std::size_t pos = 0;
        const std::string realValueString = valueContext->real->getText ();
        realValue = std::stod (realValueString, &pos);
        if (pos != realValueString.length ())
          { throw CSTConverterException (valueContext); }
      }
    catch (const std::invalid_argument &exception)
      { throw CSTConverterException (valueContext); }
    catch (const std::out_of_range &exception)
      { throw CSTConverterException (valueContext); }
    if (valueContext->im != nullptr)
      {
        try
          {
            std::size_t pos = 0;
            const std::string imValueString = valueContext->im->getText ();
            imValue = std::stod (imValueString, &pos);
            if (pos != imValueString.length ())
              { throw CSTConverterException (valueContext); }
          }
        catch (const std::invalid_argument &exception)
          { throw CSTConverterException (valueContext); }
        catch (const std::out_of_range &exception)
          { throw CSTConverterException (valueContext); }
      }
    std::complex<double> element (realSign * realValue, imSign * imValue);
    return ComplexLiteral::ElementType (element);
  }

  static ComplexLiteral::ElementType
  convertComplexValueN (ComplexValueNCase3Context *valueContext)
  {
    assert (valueContext != nullptr);
    if (valueContext->imOp == nullptr)
      { return ComplexLiteral::ElementType (std::complex<double> (0.0, 1.0)); }
    const std::string imOpString = valueContext->imOp->getText ();
    if (imOpString == "+")
      { return ComplexLiteral::ElementType (std::complex<double> (0.0, 1.0)); }
    if (imOpString == "-")
      { return ComplexLiteral::ElementType (std::complex<double> (0.0, -1.0)); }
    throw CSTConverterException (valueContext);
  }

  static ComplexLiteral::ElementType
  convertComplexValueN (ComplexValueNCase4Context *valueContext)
  {
    assert (valueContext != nullptr);
    return ComplexLiteral::ElementType (nullptr);
  }

  static ComplexLiteral *
  convert (ASTNodeMemory &mem, LiteralComplexCase0Context *literal)
  {
    assert (literal != nullptr);
    auto complexLiteral = mem.alloc<ComplexLiteral> (literal);
    std::vector<ComplexLiteral::ElementType> valueVector{};
    auto element = convertComplexValueN (literal->complexValueN ());
    valueVector.emplace_back (std::move (element));
    complexLiteral->setValue (std::move (valueVector));
    using PrimitiveClass = PrimitiveType::PrimitiveClass;
    auto primitiveType = mem.alloc<PrimitiveType> (literal);
    primitiveType->setPrimitiveClass (PrimitiveClass::Complex);
    complexLiteral->setLiteralType (primitiveType);
    return complexLiteral;
  }

  static ComplexLiteral *
  convert (ASTNodeMemory &mem, LiteralComplexCase1Context *literal)
  {
    assert (literal != nullptr);
    auto complexLiteral = mem.alloc<ComplexLiteral> (literal);
    const auto valueContexts = literal->complexValueN ();
    std::vector<ComplexLiteral::ElementType> valueVector{};
    valueVector.reserve (valueContexts.size ());
    std::transform (
        valueContexts.cbegin (), valueContexts.cend (),
        std::back_inserter (valueVector),
        [] (ComplexValueNContext *context) -> ComplexLiteral::ElementType
        { return convertComplexValueN (context); });
    complexLiteral->setValue (std::move (valueVector));
    using PrimitiveClass = PrimitiveType::PrimitiveClass;
    auto primitiveType = mem.alloc<PrimitiveType> (literal);
    primitiveType->setPrimitiveClass (PrimitiveClass::Complex);
    complexLiteral->setLiteralType (primitiveType);
    return complexLiteral;
  }

  using SymbolValueContext = HorseIRParser::SymbolValueContext;
  using LiteralSymbolCase0Context = HorseIRParser::LiteralSymbolCase0Context;
  using LiteralSymbolCase1Context = HorseIRParser::LiteralSymbolCase1Context;
  using LiteralSymbolCase2Context = HorseIRParser::LiteralSymbolCase2Context;
  using LiteralSymbolCase3Context = HorseIRParser::LiteralSymbolCase3Context;
  using LiteralSymbolCase4Context = HorseIRParser::LiteralSymbolCase4Context;

  static SymbolLiteral *
  convert (ASTNodeMemory &mem, LiteralSymbolContext *literal)
  {
    assert (literal != nullptr);
    LiteralSymbolCase0Context *case0 = nullptr;
    LiteralSymbolCase1Context *case1 = nullptr;
    LiteralSymbolCase2Context *case2 = nullptr;
    LiteralSymbolCase3Context *case3 = nullptr;
    LiteralSymbolCase4Context *case4 = nullptr;

    if ((case0 = dynamic_cast<decltype (case0)>(literal)) != nullptr)
      { return convert (mem, case0); }
    if ((case1 = dynamic_cast<decltype (case1)>(literal)) != nullptr)
      { return convert (mem, case1); }
    if ((case2 = dynamic_cast<decltype (case2)>(literal)) != nullptr)
      { return convert (mem, case2); }
    if ((case3 = dynamic_cast<decltype (case3)>(literal)) != nullptr)
      { return convert (mem, case3); }
    if ((case4 = dynamic_cast<decltype (case4)>(literal)) != nullptr)
      { return convert (mem, case4); }

    throw CSTConverterException (literal);
  }

  static SymbolLiteral::ElementType
  convertSymbolValue (SymbolValueContext *valueContext)
  {
    assert (valueContext != nullptr);
    const std::string rawString = valueContext->LITERAL_SYMBOL ()->getText ();
    std::string valueString = rawString.substr (1, std::string::npos);
    return SymbolLiteral::ElementType (std::move (valueString));
  }

  static SymbolLiteral *
  convert (ASTNodeMemory &mem, LiteralSymbolCase0Context *literal)
  {
    assert (literal != nullptr);
    auto symbolLiteral = mem.alloc<SymbolLiteral> ();
    std::vector<SymbolLiteral::ElementType> valueVector{};
    auto element = convertSymbolValue (literal->symbolValue ());
    valueVector.emplace_back (std::move (element));
    symbolLiteral->setValue (std::move (valueVector));
    using PrimitiveClass = PrimitiveType::PrimitiveClass;
    auto primitiveType = mem.alloc<PrimitiveType> ();
    primitiveType->setPrimitiveClass (PrimitiveClass::Symbol);
    symbolLiteral->setLiteralType (primitiveType);
    return symbolLiteral;
  }

  static SymbolLiteral *
  convert (ASTNodeMemory &mem, LiteralSymbolCase1Context *literal)
  {
    assert (literal != nullptr);
    auto symbolLiteral = mem.alloc<SymbolLiteral> ();
    using PrimitiveClass = PrimitiveType::PrimitiveClass;
    auto primitiveType = mem.alloc<PrimitiveType> ();
    primitiveType->setPrimitiveClass (PrimitiveClass::Symbol);
    symbolLiteral->setLiteralType (primitiveType);
    return symbolLiteral;
  }

  static SymbolLiteral *
  convert (ASTNodeMemory &mem, LiteralSymbolCase2Context *literal)
  {
    assert (literal != nullptr);
    const auto rawChildren = literal->children;
    std::vector<antlr4::tree::ParseTree *> children{};
    children.reserve (rawChildren.size ());
    std::copy_if (
        rawChildren.cbegin (), rawChildren.cend (),
        std::back_inserter (children),
        [] (antlr4::tree::ParseTree *parseTree) -> bool
        {
          if ((dynamic_cast<SymbolValueContext *>(parseTree)) != nullptr)
            { return true; }
          return parseTree->getText () == "null";
        });
    std::vector<SymbolLiteral::ElementType> valueVector{};
    valueVector.reserve (children.size ());
    std::transform (
        children.cbegin (), children.cend (),
        std::back_inserter (valueVector),
        [] (antlr4::tree::ParseTree *parseTree) -> SymbolLiteral::ElementType
        {
          SymbolValueContext *value = nullptr;
          if ((value = dynamic_cast<decltype (value)>(parseTree)) != nullptr)
            { return convertSymbolValue (value); }
          return SymbolLiteral::ElementType (nullptr);
        });
    auto symbolLiteral = mem.alloc<SymbolLiteral> ();
    symbolLiteral->setValue (std::move (valueVector));
    using PrimitiveClass = PrimitiveType::PrimitiveClass;
    auto primitiveType = mem.alloc<PrimitiveType> ();
    primitiveType->setPrimitiveClass (PrimitiveClass::Symbol);
    symbolLiteral->setLiteralType (primitiveType);
    return symbolLiteral;
  }

  static SymbolLiteral *
  convert (ASTNodeMemory &mem, LiteralSymbolCase3Context *literal)
  {
    assert (literal != nullptr);
    const auto nullTokens = literal->NULL_TOKEN ();
    std::vector<SymbolLiteral::ElementType> valueVector{};
    valueVector.reserve (nullTokens.size ());
    for (std::size_t iter = 0; iter < nullTokens.size (); ++iter)
      { valueVector.emplace_back (SymbolLiteral::ElementType (nullptr)); }
    auto symbolLiteral = mem.alloc<SymbolLiteral> ();
    symbolLiteral->setValue (std::move (valueVector));
    using PrimitiveClass = PrimitiveType::PrimitiveClass;
    auto primitiveType = mem.alloc<PrimitiveType> ();
    primitiveType->setPrimitiveClass (PrimitiveClass::Symbol);
    symbolLiteral->setLiteralType (primitiveType);
    return symbolLiteral;
  }

  static SymbolLiteral *
  convert (ASTNodeMemory &mem, LiteralSymbolCase4Context *literal)
  {
    assert (literal != nullptr);
    auto symbolLiteral = mem.alloc<SymbolLiteral> ();
    std::vector<SymbolLiteral::ElementType> valueVector{};
    valueVector.emplace_back (SymbolLiteral::ElementType (nullptr));
    symbolLiteral->setValue (std::move (valueVector));
    using PrimitiveClass = PrimitiveType::PrimitiveClass;
    auto primitiveType = mem.alloc<PrimitiveType> ();
    primitiveType->setPrimitiveClass (PrimitiveClass::Symbol);
    symbolLiteral->setLiteralType (primitiveType);
    return symbolLiteral;
  }

  using LiteralTMonthCase0Context = HorseIRParser::LiteralTMonthCase0Context;
  using LiteralTMonthCase1Context = HorseIRParser::LiteralTMonthCase1Context;

  static MonthLiteral *
  convert (ASTNodeMemory &mem, LiteralTMonthContext *literal)
  {
    assert (literal != nullptr);
    LiteralTMonthCase0Context *case0 = nullptr;
    LiteralTMonthCase1Context *case1 = nullptr;

    if ((case0 = dynamic_cast<decltype (case0)>(literal)) != nullptr)
      { return convert (mem, case0); }
    if ((case1 = dynamic_cast<decltype (case1)>(literal)) != nullptr)
      { return convert (mem, case1); }

    throw CSTConverterException (literal);
  }

  static MonthLiteral::ElementType
  convertMonthValueN (HorseIRParser::TMonthValueNContext *valueContext)
  {
    assert (valueContext != nullptr);
    if (valueContext->NULL_TOKEN () != nullptr)
      { return MonthLiteral::ElementType (nullptr); }
    const std::string valueString = valueContext->LITERAL_FLOAT ()->getText ();
    const std::regex matchRegex (R"REGEX(([0-9]{4})\.([0-9]{1,2}))REGEX");
    std::smatch matchResult;
    if (!std::regex_match (valueString, matchResult, matchRegex))
      { throw CSTConverterException (valueContext); }
    const std::string yearString = matchResult[1];
    const std::string monthString = matchResult[2];
    std::uint16_t yearValue = 0;
    std::uint8_t monthValue = 0;
    try
      {
        std::size_t pos = 0;
        unsigned long yearConverted = std::stoul (yearString, &pos, 10);
        if (pos != yearString.length ())
          { throw CSTConverterException (valueContext); }
        if (yearConverted < 1900) throw CSTConverterException (valueContext);
        if (yearConverted > 2100) throw CSTConverterException (valueContext);
        yearValue = static_cast<decltype (yearValue)>(yearConverted);

        unsigned long monthConverted = std::stoul (monthString, &pos, 10);
        if (pos != monthString.length ())
          { throw CSTConverterException (valueContext); }
        if (monthConverted == 0) throw CSTConverterException (valueContext);
        if (monthConverted > 12) throw CSTConverterException (valueContext);
        monthValue = static_cast<decltype (monthValue)>(monthConverted);
      }
    catch (const std::invalid_argument &exception)
      { throw CSTConverterException (valueContext); }
    catch (const std::out_of_range &exception)
      { throw CSTConverterException (valueContext); }
    storage::Month element{};
    element.year = yearValue;
    element.month = monthValue;
    return MonthLiteral::ElementType (element);
  }

  static MonthLiteral *
  convert (ASTNodeMemory &mem, LiteralTMonthCase0Context *literal)
  {
    assert (literal != nullptr);
    auto monthLiteral = mem.alloc<MonthLiteral> (literal);
    std::vector<MonthLiteral::ElementType> valueVector{};
    valueVector.emplace_back (convertMonthValueN (literal->tMonthValueN ()));
    monthLiteral->setValue (std::move (valueVector));
    using PrimitiveClass = PrimitiveType::PrimitiveClass;
    auto primitiveType = mem.alloc<PrimitiveType> (literal);
    primitiveType->setPrimitiveClass (PrimitiveClass::Month);
    monthLiteral->setLiteralType (primitiveType);
    return monthLiteral;
  }

  static MonthLiteral *
  convert (ASTNodeMemory &mem, LiteralTMonthCase1Context *literal)
  {
    assert (literal != nullptr);
    using TMonthValueNContext = HorseIRParser::TMonthValueNContext;
    const auto monthValueContexts = literal->tMonthValueN ();
    std::vector<MonthLiteral::ElementType> valueVector{};
    valueVector.reserve (monthValueContexts.size ());
    std::transform (
        monthValueContexts.cbegin (), monthValueContexts.cend (),
        std::back_inserter (valueVector),
        [] (TMonthValueNContext *context) -> MonthLiteral::ElementType
        { return convertMonthValueN (context); });
    auto monthLiteral = mem.alloc<MonthLiteral> (literal);
    monthLiteral->setValue (std::move (valueVector));
    using PrimitiveClass = PrimitiveType::PrimitiveClass;
    auto primitiveType = mem.alloc<PrimitiveType> (literal);
    primitiveType->setPrimitiveClass (PrimitiveClass::Month);
    monthLiteral->setLiteralType (primitiveType);
    return monthLiteral;
  }

  using LiteralTDateCase0Context = HorseIRParser::LiteralTDateCase0Context;
  using LiteralTDateCase1Context = HorseIRParser::LiteralTDateCase1Context;
  using LiteralTDateCase2Context = HorseIRParser::LiteralTDateCase2Context;
  using LiteralTDateCase3Context = HorseIRParser::LiteralTDateCase3Context;
  using LiteralTDateCase4Context = HorseIRParser::LiteralTDateCase4Context;

  static DateLiteral *
  convert (ASTNodeMemory &mem, LiteralTDateContext *literal)
  {
    assert (literal != nullptr);
    LiteralTDateCase0Context *case0 = nullptr;
    LiteralTDateCase1Context *case1 = nullptr;
    LiteralTDateCase2Context *case2 = nullptr;
    LiteralTDateCase3Context *case3 = nullptr;
    LiteralTDateCase4Context *case4 = nullptr;

    if ((case0 = dynamic_cast<decltype (case0)>(literal)) != nullptr)
      { return convert (mem, case0); }
    if ((case1 = dynamic_cast<decltype (case1)>(literal)) != nullptr)
      { return convert (mem, case1); }
    if ((case2 = dynamic_cast<decltype (case2)>(literal)) != nullptr)
      { return convert (mem, case2); }
    if ((case3 = dynamic_cast<decltype (case3)>(literal)) != nullptr)
      { return convert (mem, case3); }
    if ((case4 = dynamic_cast<decltype (case4)>(literal)) != nullptr)
      { return convert (mem, case4); }

    throw CSTConverterException (literal);
  }

  static DateLiteral::ElementType
  convertDateValue (HorseIRParser::TDateValueContext *valueContext)
  {
    assert (valueContext != nullptr);
    const auto dateContext = valueContext->LITERAL_GROUP_3_DATE ();
    const std::string valueString = dateContext->getText ();
    const std::regex matchRegex (
        R"REGEX(([0-9]{4})\.([0-9]{1,2})\.([0-9]{1,2}))REGEX"
    );
    std::smatch matchResult;
    if (!std::regex_match (valueString, matchResult, matchRegex))
      { throw CSTConverterException (valueContext); }
    const std::string yearString = matchResult[1];
    const std::string monthString = matchResult[2];
    const std::string dayString = matchResult[3];
    std::uint16_t yearValue = 0;
    std::uint8_t monthValue = 0;
    std::uint8_t dayValue = 0;
    try
      {
        std::size_t pos = 0;
        unsigned long yearConverted = std::stoul (yearString, &pos, 10);
        if (pos != yearString.length ())
          { throw CSTConverterException (valueContext); }
        if (yearConverted < 1900) throw CSTConverterException (valueContext);
        if (yearConverted > 2100) throw CSTConverterException (valueContext);
        yearValue = static_cast<decltype (yearValue)>(yearConverted);

        unsigned long monthConverted = std::stoul (monthString, &pos, 10);
        if (pos != monthString.length ())
          { throw CSTConverterException (valueContext); }
        if (monthConverted == 0) throw CSTConverterException (valueContext);
        if (monthConverted > 12) throw CSTConverterException (valueContext);
        monthValue = static_cast<decltype (monthValue)>(monthConverted);

        unsigned long dayConverted = std::stoul (dayString, &pos, 10);
        if (pos != monthString.length ())
          { throw CSTConverterException (valueContext); }

        bool isLeapYear;
        if (yearValue % 4 != 0)
          { isLeapYear = false; }
        else if (yearValue % 100 != 0)
          { isLeapYear = true; }
        else
          { isLeapYear = (yearValue % 400 == 0); }

        const std::array<std::uint8_t, 12> nonLeapYearMax = {
            31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
        };
        const std::array<std::uint8_t, 12> leapYearMax = {
            31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
        };
        if (isLeapYear && dayConverted > leapYearMax[monthValue - 1])
          { throw CSTConverterException (valueContext); }
        if ((!isLeapYear) && dayConverted > nonLeapYearMax[monthValue - 1])
          { throw CSTConverterException (valueContext); }
        dayValue = static_cast<decltype (dayValue)>(dayConverted);
      }
    catch (const std::invalid_argument &exception)
      { throw CSTConverterException (valueContext); }
    catch (const std::out_of_range &exception)
      { throw CSTConverterException (valueContext); }
    storage::Date date{};
    date.year = yearValue;
    date.month = monthValue;
    date.day = dayValue;
    return DateLiteral::ElementType (date);
  }

  static DateLiteral *
  convert (ASTNodeMemory &mem, LiteralTDateCase0Context *literal)
  {
    assert (literal != nullptr);
    auto dateLiteral = mem.alloc<DateLiteral> (literal);
    std::vector<DateLiteral::ElementType> valueVector{};
    valueVector.emplace_back (convertDateValue (literal->tDateValue ()));
    dateLiteral->setValue (std::move (valueVector));
    using PrimitiveClass = PrimitiveType::PrimitiveClass;
    auto primitiveType = mem.alloc<PrimitiveType> (literal);
    primitiveType->setPrimitiveClass (PrimitiveClass::Date);
    dateLiteral->setLiteralType (primitiveType);
    return dateLiteral;
  }

  static DateLiteral *
  convert (ASTNodeMemory &mem, LiteralTDateCase1Context *literal)
  {
    assert (literal != nullptr);
    auto dateLiteral = mem.alloc<DateLiteral> (literal);
    using PrimitiveClass = PrimitiveType::PrimitiveClass;
    auto primitiveType = mem.alloc<PrimitiveType> (literal);
    primitiveType->setPrimitiveClass (PrimitiveClass::Date);
    dateLiteral->setLiteralType (primitiveType);
    return dateLiteral;
  }

  static DateLiteral *
  convert (ASTNodeMemory &mem, LiteralTDateCase2Context *literal)
  {
    assert (literal != nullptr);
    using ParseTree = antlr4::tree::ParseTree;
    const auto rawChildren = literal->children;
    std::vector<ParseTree *> children{};
    children.reserve (rawChildren.size ());
    std::copy_if (
        rawChildren.cbegin (), rawChildren.cend (),
        std::back_inserter (children),
        [] (ParseTree *parseTree) -> bool
        {
          using TDateValueContext = HorseIRParser::TDateValueContext;
          if (dynamic_cast<TDateValueContext *>(parseTree) != nullptr)
            { return true; }
          return parseTree->getText () == "null";
        });
    std::vector<DateLiteral::ElementType> valueVector{};
    valueVector.reserve (children.size ());
    std::transform (
        children.cbegin (), children.cend (),
        std::back_inserter (valueVector),
        [] (ParseTree *parseTree) -> DateLiteral::ElementType
        {
          HorseIRParser::TDateValueContext *value = nullptr;
          if ((value = dynamic_cast<decltype (value)>(parseTree)) != nullptr)
            { return convertDateValue (value); }
          return DateLiteral::ElementType (nullptr);
        });
    auto dateLiteral = mem.alloc<DateLiteral> (literal);
    dateLiteral->setValue (std::move (valueVector));
    using PrimitiveClass = PrimitiveType::PrimitiveClass;
    auto primitiveType = mem.alloc<PrimitiveType> (literal);
    primitiveType->setPrimitiveClass (PrimitiveClass::Date);
    dateLiteral->setLiteralType (primitiveType);
    return dateLiteral;
  }

  static DateLiteral *
  convert (ASTNodeMemory &mem, LiteralTDateCase3Context *literal)
  {
    assert (literal != nullptr);
    const auto nullTokens = literal->NULL_TOKEN ();
    auto dateLiteral = mem.alloc<DateLiteral> (literal);
    std::vector<DateLiteral::ElementType> valueVector{};
    valueVector.reserve (nullTokens.size ());
    for (std::size_t iter = 0; iter < nullTokens.size (); ++iter)
      { valueVector.emplace_back (DateLiteral::ElementType (nullptr)); }
    dateLiteral->setValue (std::move (valueVector));
    using PrimitiveClass = PrimitiveType::PrimitiveClass;
    auto primitiveType = mem.alloc<PrimitiveType> (literal);
    primitiveType->setPrimitiveClass (PrimitiveClass::Date);
    dateLiteral->setLiteralType (primitiveType);
    return dateLiteral;
  }

  static DateLiteral *
  convert (ASTNodeMemory &mem, LiteralTDateCase4Context *literal)
  {
    assert (literal != nullptr);
    auto dateLiteral = mem.alloc<DateLiteral> (literal);
    std::vector<DateLiteral::ElementType> dateValue{};
    dateValue.emplace_back (DateLiteral::ElementType (nullptr));
    dateLiteral->setValue (std::move (dateValue));
    using PrimitiveClass = PrimitiveType::PrimitiveClass;
    auto primitiveType = mem.alloc<PrimitiveType> (literal);
    primitiveType->setPrimitiveClass (PrimitiveClass::Date);
    dateLiteral->setLiteralType (primitiveType);
    return dateLiteral;
  }

  using LiteralTDateTime0Context = HorseIRParser::LiteralTDateTimeCase0Context;
  using LiteralTDateTime1Context = HorseIRParser::LiteralTDateTimeCase1Context;
  using LiteralTDateTime2Context = HorseIRParser::LiteralTDateTimeCase2Context;
  using LiteralTDateTime3Context = HorseIRParser::LiteralTDateTimeCase3Context;
  using LiteralTDateTime4Context = HorseIRParser::LiteralTDateTimeCase4Context;

  static DateTimeLiteral *
  convert (ASTNodeMemory &mem, LiteralTDateTimeContext *literal)
  {
    assert (literal != nullptr);
    LiteralTDateTime0Context *case0 = nullptr;
    LiteralTDateTime1Context *case1 = nullptr;
    LiteralTDateTime2Context *case2 = nullptr;
    LiteralTDateTime3Context *case3 = nullptr;
    LiteralTDateTime4Context *case4 = nullptr;

    if ((case0 = dynamic_cast<decltype (case0)>(literal)) != nullptr)
      { return convert (mem, case0); }
    if ((case1 = dynamic_cast<decltype (case1)>(literal)) != nullptr)
      { return convert (mem, case1); }
    if ((case2 = dynamic_cast<decltype (case2)>(literal)) != nullptr)
      { return convert (mem, case2); }
    if ((case3 = dynamic_cast<decltype (case3)>(literal)) != nullptr)
      { return convert (mem, case3); }
    if ((case4 = dynamic_cast<decltype (case4)>(literal)) != nullptr)
      { return convert (mem, case4); }

    throw CSTConverterException (literal);
  }

  static DateTimeLiteral::ElementType
  convertDateTimeValue (HorseIRParser::TDateTimeValueContext *valueContext)
  {
    assert (valueContext != nullptr);
    const auto dateTimeContext = valueContext->LITERAL_GROUP_7_DATETIME ();
    const std::string valueString = dateTimeContext->getText ();
    const std::regex matchRegex (
        R"REGEX(([0-9]{4})\.([0-9]{1,2})\.([0-9]{1,2})T)REGEX"
            R"REGEX(([0-9]{1,2}):([0-9]{1,2}):([0-9]{1,2})\.([0-9]{1,3}))REGEX"
    );
    std::smatch matchResult;
    if (!std::regex_match (valueString, matchResult, matchRegex))
      { throw CSTConverterException (valueContext); }
    const std::string yearString = matchResult[1];
    const std::string monthString = matchResult[2];
    const std::string dayString = matchResult[3];
    const std::string hourString = matchResult[4];
    const std::string minuteString = matchResult[5];
    const std::string secondString = matchResult[6];
    const std::string msString = matchResult[7];
    std::uint16_t yearValue = 0, msValue = 0;
    std::uint8_t monthValue = 0, dayValue = 0, hourValue = 0, minuteValue = 0;
    std::uint8_t secondValue = 0;
    try
      {
        std::size_t pos = 0;
        unsigned long yearConverted = std::stoul (yearString, &pos, 10);
        if (pos != yearString.length ())
          { throw CSTConverterException (valueContext); }
        if (yearConverted < 1900) throw CSTConverterException (valueContext);
        if (yearConverted > 2100) throw CSTConverterException (valueContext);
        yearValue = static_cast<decltype (yearValue)>(yearConverted);

        unsigned long monthConverted = std::stoul (monthString, &pos, 10);
        if (pos != monthString.length ())
          { throw CSTConverterException (valueContext); }
        if (monthConverted == 0) throw CSTConverterException (valueContext);
        if (monthConverted > 12) throw CSTConverterException (valueContext);
        monthValue = static_cast<decltype (monthValue)>(monthConverted);

        unsigned long dayConverted = std::stoul (dayString, &pos, 10);
        if (pos != dayString.length ())
          { throw CSTConverterException (valueContext); }
        bool isLeapYear;
        if (yearValue % 4 != 0)
          { isLeapYear = false; }
        else if (yearValue % 100 != 0)
          { isLeapYear = true; }
        else
          { isLeapYear = (yearValue % 400 == 0); }
        const std::array<std::uint8_t, 12> nonLeapYearMax = {
            31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
        };
        const std::array<std::uint8_t, 12> leapYearMax = {
            31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
        };
        if (isLeapYear && dayConverted > leapYearMax[monthValue - 1])
          { throw CSTConverterException (valueContext); }
        if ((!isLeapYear) && dayConverted > nonLeapYearMax[monthValue - 1])
          { throw CSTConverterException (valueContext); }
        dayValue = static_cast<decltype (dayValue)>(dayConverted);

        unsigned long hourConverted = std::stoul (hourString, &pos, 10);
        if (pos != hourString.length ())
          { throw CSTConverterException (valueContext); }
        if (hourConverted >= 24) throw CSTConverterException (valueContext);
        hourValue = static_cast<decltype (hourValue)>(hourConverted);

        unsigned long minuteConverted = std::stoul (minuteString, &pos, 10);
        if (pos != minuteString.length ())
          { throw CSTConverterException (valueContext); }
        if (minuteConverted >= 60) throw CSTConverterException (valueContext);
        minuteValue = static_cast<decltype (minuteValue)>(minuteConverted);

        unsigned long secondConverted = std::stoul (secondString, &pos, 10);
        if (pos != secondString.length ())
          { throw CSTConverterException (valueContext); }
        if (secondConverted >= 60) throw CSTConverterException (valueContext);
        secondValue = static_cast<decltype (secondValue)>(secondConverted);

        unsigned long msConverted = std::stoul (msString, &pos, 10);
        if (pos != msString.length ())
          { throw CSTConverterException (valueContext); }
        msValue = static_cast<decltype (msValue)>(msConverted);
      }
    catch (const std::invalid_argument &exception)
      { throw CSTConverterException (valueContext); }
    catch (const std::out_of_range &exception)
      { throw CSTConverterException (valueContext); }
    storage::DateTime dateTime{};
    dateTime.year = yearValue;
    dateTime.month = monthValue;
    dateTime.day = dayValue;
    dateTime.hour = hourValue;
    dateTime.minute = minuteValue;
    dateTime.second = secondValue;
    dateTime.millisecond = msValue;
    return DateTimeLiteral::ElementType (dateTime);
  }

  static DateTimeLiteral *
  convert (ASTNodeMemory &mem, LiteralTDateTime0Context *literal)
  {
    assert (literal != nullptr);
    auto dateTimeLiteral = mem.alloc<DateTimeLiteral> (literal);
    std::vector<DateTimeLiteral::ElementType> valueVector{};
    const auto dateTimeValueContext = literal->tDateTimeValue ();
    valueVector.emplace_back (convertDateTimeValue (dateTimeValueContext));
    dateTimeLiteral->setValue (std::move (valueVector));
    using PrimitiveClass = PrimitiveType::PrimitiveClass;
    auto primitiveType = mem.alloc<PrimitiveType> (literal);
    primitiveType->setPrimitiveClass (PrimitiveClass::DateTime);
    dateTimeLiteral->setLiteralType (primitiveType);
    return dateTimeLiteral;
  }

  static DateTimeLiteral *
  convert (ASTNodeMemory &mem, LiteralTDateTime1Context *literal)
  {
    assert (literal != nullptr);
    auto dateTimeLiteral = mem.alloc<DateTimeLiteral> (literal);
    using PrimitiveClass = PrimitiveType::PrimitiveClass;
    auto primitiveType = mem.alloc<PrimitiveType> (literal);
    primitiveType->setPrimitiveClass (PrimitiveClass::DateTime);
    dateTimeLiteral->setLiteralType (primitiveType);
    return dateTimeLiteral;
  }

  static DateTimeLiteral *
  convert (ASTNodeMemory &mem, LiteralTDateTime2Context *literal)
  {
    assert (literal != nullptr);
    using ParseTree = antlr4::tree::ParseTree;
    const std::vector<ParseTree *> rawChildren = literal->children;
    std::vector<ParseTree *> children{};
    children.reserve (rawChildren.size ());
    std::copy_if (
        rawChildren.cbegin (), rawChildren.cend (),
        std::back_inserter (children),
        [] (ParseTree *parseTree) -> bool
        {
          using DateTimeValueContext = HorseIRParser::TDateTimeValueContext;
          if (dynamic_cast<DateTimeValueContext *>(parseTree) != nullptr)
            { return true; }
          return parseTree->getText () == "null";
        });
    std::vector<DateTimeLiteral::ElementType> valueVector{};
    valueVector.reserve (children.size ());
    std::transform (
        children.cbegin (), children.cend (), std::back_inserter (valueVector),
        [] (ParseTree *parseTree) -> DateTimeLiteral::ElementType
        {
          HorseIRParser::TDateTimeValueContext *value = nullptr;
          if ((value = dynamic_cast<decltype (value)>(parseTree)) != nullptr)
            { return convertDateTimeValue (value); }
          return DateTimeLiteral::ElementType (nullptr);
        });
    auto dateTimeLiteral = mem.alloc<DateTimeLiteral> (literal);
    dateTimeLiteral->setValue (std::move (valueVector));
    using PrimitiveClass = PrimitiveType::PrimitiveClass;
    auto primitiveType = mem.alloc<PrimitiveType> (literal);
    primitiveType->setPrimitiveClass (PrimitiveClass::DateTime);
    dateTimeLiteral->setLiteralType (primitiveType);
    return dateTimeLiteral;
  }

  static DateTimeLiteral *
  convert (ASTNodeMemory &mem, LiteralTDateTime3Context *literal)
  {
    assert (literal != nullptr);
    const auto nullTokens = literal->NULL_TOKEN ();
    std::vector<DateTimeLiteral::ElementType> valueVector{};
    valueVector.reserve (nullTokens.size ());
    for (std::size_t iter = 0; iter < nullTokens.size (); ++iter)
      { valueVector.emplace_back (DateTimeLiteral::ElementType (nullptr)); }
    auto dateTimeLiteral = mem.alloc<DateTimeLiteral> (literal);
    dateTimeLiteral->setValue (std::move (valueVector));
    using PrimitiveClass = PrimitiveType::PrimitiveClass;
    auto primitiveType = mem.alloc<PrimitiveType> (literal);
    primitiveType->setPrimitiveClass (PrimitiveClass::DateTime);
    dateTimeLiteral->setLiteralType (primitiveType);
    return dateTimeLiteral;
  }

  static DateTimeLiteral *
  convert (ASTNodeMemory &mem, LiteralTDateTime4Context *literal)
  {
    assert (literal != nullptr);
    auto dateTimeLiteral = mem.alloc<DateTimeLiteral> (literal);
    std::vector<DateTimeLiteral::ElementType> valueVector{};
    valueVector.emplace_back (DateTimeLiteral::ElementType (nullptr));
    dateTimeLiteral->setValue (std::move (valueVector));
    using PrimitiveClass = PrimitiveType::PrimitiveClass;
    auto primitiveType = mem.alloc<PrimitiveType> (literal);
    primitiveType->setPrimitiveClass (PrimitiveClass::DateTime);
    dateTimeLiteral->setLiteralType (primitiveType);
    return dateTimeLiteral;
  }

  using LiteralTMinuteCase0Context = HorseIRParser::LiteralTMinuteCase0Context;
  using LiteralTMinuteCase1Context = HorseIRParser::LiteralTMinuteCase1Context;
  using LiteralTMinuteCase2Context = HorseIRParser::LiteralTMinuteCase2Context;
  using LiteralTMinuteCase3Context = HorseIRParser::LiteralTMinuteCase3Context;
  using LiteralTMinuteCase4Context = HorseIRParser::LiteralTMinuteCase4Context;

  static MinuteLiteral *
  convert (ASTNodeMemory &mem, LiteralTMinuteContext *literal)
  {
    assert (literal != nullptr);
    LiteralTMinuteCase0Context *case0 = nullptr;
    LiteralTMinuteCase1Context *case1 = nullptr;
    LiteralTMinuteCase2Context *case2 = nullptr;
    LiteralTMinuteCase3Context *case3 = nullptr;
    LiteralTMinuteCase4Context *case4 = nullptr;

    if ((case0 = dynamic_cast<decltype (case0)>(literal)) != nullptr)
      { return convert (mem, case0); }
    if ((case1 = dynamic_cast<decltype (case1)>(literal)) != nullptr)
      { return convert (mem, case1); }
    if ((case2 = dynamic_cast<decltype (case2)>(literal)) != nullptr)
      { return convert (mem, case2); }
    if ((case3 = dynamic_cast<decltype (case3)>(literal)) != nullptr)
      { return convert (mem, case3); }
    if ((case4 = dynamic_cast<decltype (case4)>(literal)) != nullptr)
      { return convert (mem, case4); }

    throw CSTConverterException (literal);
  }

  static MinuteLiteral::ElementType
  convertMinuteValue (HorseIRParser::TMinuteValueContext *valueContext)
  {
    assert (valueContext != nullptr);
    const auto minuteContext = valueContext->LITERAL_GROUP_2_MINUTE ();
    const std::string valueString = minuteContext->getText ();
    const std::regex matchRegex (
        R"REGEX(([0-9]{1,2}):([0-9]{1,2}))REGEX"
    );
    std::smatch matchResult;
    if (!std::regex_match (valueString, matchResult, matchRegex))
      { throw CSTConverterException (valueContext); }
    const std::string hourString = matchResult[1];
    const std::string minuteString = matchResult[2];
    std::uint8_t hourValue = 0, minuteValue = 0;
    try
      {
        std::size_t pos;
        unsigned long hourConverted = std::stoul (hourString, &pos, 10);
        if (pos != hourString.length ())
          { throw CSTConverterException (valueContext); }
        if (hourConverted >= 24) throw CSTConverterException (valueContext);
        hourValue = static_cast<decltype (hourValue)>(hourConverted);

        unsigned long minuteConverted = std::stoul (minuteString, &pos, 10);
        if (pos != minuteString.length ())
          { throw CSTConverterException (valueContext); }
        if (minuteConverted >= 60) throw CSTConverterException (valueContext);
        minuteValue = static_cast<decltype (minuteValue)>(minuteConverted);
      }
    catch (const std::invalid_argument &exception)
      { throw CSTConverterException (valueContext); }
    catch (const std::out_of_range &exception)
      { throw CSTConverterException (valueContext); }
    storage::Minute minute{};
    minute.hour = hourValue;
    minute.minute = minuteValue;
    return MinuteLiteral::ElementType (minute);
  }

  static MinuteLiteral *
  convert (ASTNodeMemory &mem, LiteralTMinuteCase0Context *literal)
  {
    assert (literal != nullptr);
    auto minuteLiteral = mem.alloc<MinuteLiteral> (literal);
    std::vector<MinuteLiteral::ElementType> valueVector{};
    valueVector.emplace_back (convertMinuteValue (literal->tMinuteValue ()));
    minuteLiteral->setValue (std::move (valueVector));
    using PrimitiveClass = PrimitiveType::PrimitiveClass;
    auto primitiveType = mem.alloc<PrimitiveType> (literal);
    primitiveType->setPrimitiveClass (PrimitiveClass::Minute);
    minuteLiteral->setLiteralType (primitiveType);
    return minuteLiteral;
  }

  static MinuteLiteral *
  convert (ASTNodeMemory &mem, LiteralTMinuteCase1Context *literal)
  {
    assert (literal != nullptr);
    auto minuteLiteral = mem.alloc<MinuteLiteral> (literal);
    using PrimitiveClass = PrimitiveType::PrimitiveClass;
    auto primitiveType = mem.alloc<PrimitiveType> (literal);
    primitiveType->setPrimitiveClass (PrimitiveClass::Minute);
    minuteLiteral->setLiteralType (primitiveType);
    return minuteLiteral;
  }

  static MinuteLiteral *
  convert (ASTNodeMemory &mem, LiteralTMinuteCase2Context *literal)
  {
    assert (literal != nullptr);
    using ParseTree = antlr4::tree::ParseTree;
    const std::vector<ParseTree *> rawChildren = literal->children;
    std::vector<ParseTree *> children{};
    children.reserve (rawChildren.size ());
    std::copy_if (
        rawChildren.cbegin (), rawChildren.cend (),
        std::back_inserter (children),
        [] (ParseTree *parseTree) -> bool
        {
          using MinuteValueContext = HorseIRParser::TMinuteValueContext;
          if (dynamic_cast<MinuteValueContext *>(parseTree) != nullptr)
            { return true; }
          return parseTree->getText () == "null";
        });
    std::vector<MinuteLiteral::ElementType> valueVector{};
    valueVector.reserve (children.size ());
    std::transform (
        children.cbegin (), children.cend (), std::back_inserter (valueVector),
        [] (ParseTree *parseTree) -> MinuteLiteral::ElementType
        {
          HorseIRParser::TMinuteValueContext *value = nullptr;
          if ((value = dynamic_cast<decltype (value)>(parseTree)) != nullptr)
            { return convertMinuteValue (value); }
          return MinuteLiteral::ElementType (nullptr);
        });
    auto minuteLiteral = mem.alloc<MinuteLiteral> (literal);
    minuteLiteral->setValue (std::move (valueVector));
    using PrimitiveClass = PrimitiveType::PrimitiveClass;
    auto primitiveType = mem.alloc<PrimitiveType> (literal);
    primitiveType->setPrimitiveClass (PrimitiveClass::Minute);
    minuteLiteral->setLiteralType (primitiveType);
    return minuteLiteral;
  }

  static MinuteLiteral *
  convert (ASTNodeMemory &mem, LiteralTMinuteCase3Context *literal)
  {
    assert (literal != nullptr);
    const auto nullTokens = literal->NULL_TOKEN ();
    std::vector<MinuteLiteral::ElementType> valueVector{};
    valueVector.reserve (nullTokens.size ());
    for (std::size_t iter = 0; iter < nullTokens.size (); ++iter)
      { valueVector.emplace_back (MinuteLiteral::ElementType (nullptr)); }
    auto minuteLiteral = mem.alloc<MinuteLiteral> (literal);
    minuteLiteral->setValue (std::move (valueVector));
    using PrimitiveClass = PrimitiveType::PrimitiveClass;
    auto primitiveType = mem.alloc<PrimitiveType> (literal);
    primitiveType->setPrimitiveClass (PrimitiveClass::Minute);
    minuteLiteral->setLiteralType (primitiveType);
    return minuteLiteral;
  }

  static MinuteLiteral *
  convert (ASTNodeMemory &mem, LiteralTMinuteCase4Context *literal)
  {
    assert (literal != nullptr);
    auto minuteLiteral = mem.alloc<MinuteLiteral> (literal);
    std::vector<MinuteLiteral::ElementType> valueVector{};
    valueVector.emplace_back (MinuteLiteral::ElementType (nullptr));
    minuteLiteral->setValue (std::move (valueVector));
    using PrimitiveClass = PrimitiveType::PrimitiveClass;
    auto primitiveType = mem.alloc<PrimitiveType> (literal);
    primitiveType->setPrimitiveClass (PrimitiveClass::Minute);
    minuteLiteral->setLiteralType (primitiveType);
    return minuteLiteral;
  }

  using LiteralTSecondCase0Context = HorseIRParser::LiteralTSecondCase0Context;
  using LiteralTSecondCase1Context = HorseIRParser::LiteralTSecondCase1Context;
  using LiteralTSecondCase2Context = HorseIRParser::LiteralTSecondCase2Context;
  using LiteralTSecondCase3Context = HorseIRParser::LiteralTSecondCase3Context;
  using LiteralTSecondCase4Context = HorseIRParser::LiteralTSecondCase4Context;

  static SecondLiteral *
  convert (ASTNodeMemory &mem, LiteralTSecondContext *literal)
  {
    assert (literal != nullptr);
    LiteralTSecondCase0Context *case0 = nullptr;
    LiteralTSecondCase1Context *case1 = nullptr;
    LiteralTSecondCase2Context *case2 = nullptr;
    LiteralTSecondCase3Context *case3 = nullptr;
    LiteralTSecondCase4Context *case4 = nullptr;

    if ((case0 = dynamic_cast<decltype (case0)>(literal)) != nullptr)
      { return convert (mem, case0); }
    if ((case1 = dynamic_cast<decltype (case1)>(literal)) != nullptr)
      { return convert (mem, case1); }
    if ((case2 = dynamic_cast<decltype (case2)>(literal)) != nullptr)
      { return convert (mem, case2); }
    if ((case3 = dynamic_cast<decltype (case3)>(literal)) != nullptr)
      { return convert (mem, case3); }
    if ((case4 = dynamic_cast<decltype (case4)>(literal)) != nullptr)
      { return convert (mem, case4); }

    throw CSTConverterException (literal);
  }

  static SecondLiteral::ElementType
  convertSecondValue (HorseIRParser::TSecondValueContext *valueContext)
  {
    assert (valueContext != nullptr);
    const auto secondContext = valueContext->LITERAL_GROUP_3_SECOND ();
    const std::string valueString = secondContext->getText ();
    const std::regex matchRegex (
        R"REGEX(([0-9]{1,2}):([0-9]{1,2}):([0-9]{1,2}))REGEX"
    );
    std::smatch matchResult;
    if (!std::regex_match (valueString, matchResult, matchRegex))
      { throw CSTConverterException (valueContext); }
    const std::string hourString = matchResult[1];
    const std::string minuteString = matchResult[2];
    const std::string secondString = matchResult[3];
    std::uint8_t hourValue = 0, minuteValue = 0, secondValue = 0;
    try
      {
        std::size_t pos;
        unsigned long hourConverted = std::stoul (hourString, &pos, 10);
        if (pos != hourString.length ())
          { throw CSTConverterException (valueContext); }
        if (hourConverted >= 24) throw CSTConverterException (valueContext);
        hourValue = static_cast<decltype (hourValue)>(hourConverted);

        unsigned long minuteConverted = std::stoul (minuteString, &pos, 10);
        if (pos != minuteString.length ())
          { throw CSTConverterException (valueContext); }
        if (minuteConverted >= 60) throw CSTConverterException (valueContext);
        minuteValue = static_cast<decltype (minuteValue)>(minuteConverted);

        unsigned long secondConverted = std::stoul (secondString, &pos, 10);
        if (pos != secondString.length ())
          { throw CSTConverterException (valueContext); }
        if (secondConverted >= 60) throw CSTConverterException (valueContext);
        secondValue = static_cast<decltype (secondValue)>(secondConverted);
      }
    catch (const std::invalid_argument &exception)
      { throw CSTConverterException (nullptr); }
    catch (const std::out_of_range &exception)
      { throw CSTConverterException (nullptr); }
    storage::Second second{};
    second.hour = hourValue;
    second.minute = minuteValue;
    second.second = secondValue;
    return SecondLiteral::ElementType (second);
  }

  static SecondLiteral *
  convert (ASTNodeMemory &mem, LiteralTSecondCase0Context *literal)
  {
    assert (literal != nullptr);
    auto secondLiteral = mem.alloc<SecondLiteral> (literal);
    std::vector<SecondLiteral::ElementType> valueVector{};
    valueVector.emplace_back (convertSecondValue (literal->tSecondValue ()));
    secondLiteral->setValue (std::move (valueVector));
    using PrimitiveClass = PrimitiveType::PrimitiveClass;
    auto primitiveType = mem.alloc<PrimitiveType> (literal);
    primitiveType->setPrimitiveClass (PrimitiveClass::Second);
    secondLiteral->setLiteralType (primitiveType);
    return secondLiteral;
  }

  static SecondLiteral *
  convert (ASTNodeMemory &mem, LiteralTSecondCase1Context *literal)
  {
    assert (literal != nullptr);
    auto secondLiteral = mem.alloc<SecondLiteral> (literal);
    using PrimitiveClass = PrimitiveType::PrimitiveClass;
    auto primitiveType = mem.alloc<PrimitiveType> (literal);
    primitiveType->setPrimitiveClass (PrimitiveClass::Second);
    secondLiteral->setLiteralType (primitiveType);
    return secondLiteral;
  }

  static SecondLiteral *
  convert (ASTNodeMemory &mem, LiteralTSecondCase2Context *literal)
  {
    assert (literal != nullptr);
    using ParseTree = antlr4::tree::ParseTree;
    const std::vector<ParseTree *> rawChildren = literal->children;
    std::vector<ParseTree *> children{};
    children.reserve (rawChildren.size ());
    std::copy_if (
        rawChildren.cbegin (), rawChildren.cend (),
        std::back_inserter (children),
        [] (ParseTree *parseTree) -> bool
        {
          using TSecondValueContext = HorseIRParser::TSecondValueContext;
          if (dynamic_cast<TSecondValueContext *>(parseTree) != nullptr)
            { return true; }
          return parseTree->getText () == "null";
        });
    std::vector<SecondLiteral::ElementType> valueVector{};
    valueVector.reserve (children.size ());
    std::transform (
        children.cbegin (), children.cend (), std::back_inserter (valueVector),
        [] (ParseTree *parseTree) -> SecondLiteral::ElementType
        {
          HorseIRParser::TSecondValueContext *value = nullptr;
          if ((value = dynamic_cast<decltype (value)>(parseTree)) != nullptr)
            { return convertSecondValue (value); }
          return SecondLiteral::ElementType (nullptr);
        });
    auto secondLiteral = mem.alloc<SecondLiteral> (literal);
    secondLiteral->setValue (std::move (valueVector));
    using PrimitiveClass = PrimitiveType::PrimitiveClass;
    auto primitiveType = mem.alloc<PrimitiveType> (literal);
    primitiveType->setPrimitiveClass (PrimitiveClass::Second);
    secondLiteral->setLiteralType (primitiveType);
    return secondLiteral;
  }

  static SecondLiteral *
  convert (ASTNodeMemory &mem, LiteralTSecondCase3Context *literal)
  {
    assert (literal != nullptr);
    const auto nullTokens = literal->NULL_TOKEN ();
    std::vector<SecondLiteral::ElementType> valueVector{};
    valueVector.reserve (nullTokens.size ());
    for (std::size_t iter = 0; iter < nullTokens.size (); ++iter)
      { valueVector.emplace_back (SecondLiteral::ElementType (nullptr)); }
    auto secondLiteral = mem.alloc<SecondLiteral> (literal);
    secondLiteral->setValue (std::move (valueVector));
    using PrimitiveClass = PrimitiveType::PrimitiveClass;
    auto primitiveType = mem.alloc<PrimitiveType> (literal);
    primitiveType->setPrimitiveClass (PrimitiveClass::Second);
    secondLiteral->setLiteralType (primitiveType);
    return secondLiteral;
  }

  static SecondLiteral *
  convert (ASTNodeMemory &mem, LiteralTSecondCase4Context *literal)
  {
    assert (literal != nullptr);
    auto secondLiteral = mem.alloc<SecondLiteral> (literal);
    std::vector<SecondLiteral::ElementType> valueVector{};
    valueVector.emplace_back (SecondLiteral::ElementType (nullptr));
    secondLiteral->setValue (std::move (valueVector));
    using PrimitiveClass = PrimitiveType::PrimitiveClass;
    auto primitiveType = mem.alloc<PrimitiveType> (literal);
    primitiveType->setPrimitiveClass (PrimitiveClass::Second);
    secondLiteral->setLiteralType (primitiveType);
    return secondLiteral;
  }

  using LiteralTTimeCase0Context = HorseIRParser::LiteralTTimeCase0Context;
  using LiteralTTimeCase1Context = HorseIRParser::LiteralTTimeCase1Context;
  using LiteralTTimeCase2Context = HorseIRParser::LiteralTTimeCase2Context;
  using LiteralTTimeCase3Context = HorseIRParser::LiteralTTimeCase3Context;
  using LiteralTTimeCase4Context = HorseIRParser::LiteralTTimeCase4Context;

  static TimeLiteral *
  convert (ASTNodeMemory &mem, LiteralTTimeContext *literal)
  {
    assert (literal != nullptr);
    LiteralTTimeCase0Context *case0 = nullptr;
    LiteralTTimeCase1Context *case1 = nullptr;
    LiteralTTimeCase2Context *case2 = nullptr;
    LiteralTTimeCase3Context *case3 = nullptr;
    LiteralTTimeCase4Context *case4 = nullptr;

    if ((case0 = dynamic_cast<decltype (case0)>(literal)) != nullptr)
      { return convert (mem, case0); }
    if ((case1 = dynamic_cast<decltype (case1)>(literal)) != nullptr)
      { return convert (mem, case1); }
    if ((case2 = dynamic_cast<decltype (case2)>(literal)) != nullptr)
      { return convert (mem, case2); }
    if ((case3 = dynamic_cast<decltype (case3)>(literal)) != nullptr)
      { return convert (mem, case3); }
    if ((case4 = dynamic_cast<decltype (case4)>(literal)) != nullptr)
      { return convert (mem, case4); }

    throw CSTConverterException (literal);
  }

  static TimeLiteral::ElementType
  convertTimeValue (HorseIRParser::TTimeValueContext *valueContext)
  {
    assert (valueContext != nullptr);
    const auto timeContext = valueContext->LITERAL_GROUP_4_TIME ();
    const std::string timeString = timeContext->getText ();
    const std::regex matchRegex (
        R"REGEX(([0-9]{1,2}):([0-9]{1,2}):([0-9]{1,2})\.([0-9]{1,3}))REGEX"
    );
    std::smatch matchResult;
    if (!std::regex_match (timeString, matchResult, matchRegex))
      { throw CSTConverterException (valueContext); }
    const std::string hourString = matchResult[1];
    const std::string minuteString = matchResult[2];
    const std::string secondString = matchResult[3];
    const std::string msString = matchResult[4];
    std::uint8_t hourValue = 0, minuteValue = 0, secondValue = 0;
    std::uint16_t msValue = 0;
    try
      {
        std::size_t pos;
        unsigned long hourConverted = std::stoul (hourString, &pos, 10);
        if (pos != hourString.length ())
          { throw CSTConverterException (valueContext); }
        if (hourConverted >= 24) throw CSTConverterException (valueContext);
        hourValue = static_cast<decltype (hourValue)>(hourConverted);

        unsigned long minuteConverted = std::stoul (minuteString, &pos, 10);
        if (pos != minuteString.length ())
          { throw CSTConverterException (valueContext); }
        if (minuteConverted >= 60) throw CSTConverterException (valueContext);
        minuteValue = static_cast<decltype (minuteValue)>(minuteConverted);

        unsigned long secondConverted = std::stoul (secondString, &pos, 10);
        if (pos != secondString.length ())
          { throw CSTConverterException (valueContext); }
        if (secondConverted >= 60) throw CSTConverterException (valueContext);
        secondValue = static_cast<decltype (secondValue)>(secondConverted);

        unsigned long msConverted = std::stoul (msString, &pos, 10);
        if (pos != msString.length ())
          { throw CSTConverterException (valueContext); }
        msValue = static_cast<decltype (msValue)>(msConverted);
      }
    catch (const std::invalid_argument &exception)
      { throw CSTConverterException (valueContext); }
    catch (const std::out_of_range &exception)
      { throw CSTConverterException (valueContext); }
    storage::Time time{};
    time.hour = hourValue;
    time.minute = minuteValue;
    time.second = secondValue;
    time.millisecond = msValue;
    return TimeLiteral::ElementType (time);
  }

  static TimeLiteral *
  convert (ASTNodeMemory &mem, LiteralTTimeCase0Context *literal)
  {
    assert (literal != nullptr);
    auto timeLiteral = mem.alloc<TimeLiteral> (literal);
    std::vector<TimeLiteral::ElementType> valueVector{};
    valueVector.emplace_back (convertTimeValue (literal->tTimeValue ()));
    timeLiteral->setValue (std::move (valueVector));
    using PrimitiveClass = PrimitiveType::PrimitiveClass;
    auto primitiveType = mem.alloc<PrimitiveType> (literal);
    primitiveType->setPrimitiveClass (PrimitiveClass::Time);
    timeLiteral->setLiteralType (primitiveType);
    return timeLiteral;
  }

  static TimeLiteral *
  convert (ASTNodeMemory &mem, LiteralTTimeCase1Context *literal)
  {
    assert (literal != nullptr);
    auto timeLiteral = mem.alloc<TimeLiteral> (literal);
    using PrimitiveClass = PrimitiveType::PrimitiveClass;
    auto primitiveType = mem.alloc<PrimitiveType> (literal);
    primitiveType->setPrimitiveClass (PrimitiveClass::Time);
    timeLiteral->setLiteralType (primitiveType);
    return timeLiteral;
  }

  static TimeLiteral *
  convert (ASTNodeMemory &mem, LiteralTTimeCase2Context *literal)
  {
    assert (literal != nullptr);
    using ParseTree = antlr4::tree::ParseTree;
    const std::vector<ParseTree *> rawChildren = literal->children;
    std::vector<ParseTree *> children{};
    children.reserve (rawChildren.size ());
    std::copy_if (
        rawChildren.cbegin (), rawChildren.cend (),
        std::back_inserter (children),
        [] (ParseTree *parseTree) -> bool
        {
          using TimeValueContext = HorseIRParser::TTimeValueContext;
          if (dynamic_cast<TimeValueContext *>(parseTree) != nullptr)
            { return true; }
          return parseTree->getText () == "null";
        });
    std::vector<TimeLiteral::ElementType> valueVector{};
    valueVector.reserve (children.size ());
    std::transform (
        children.cbegin (), children.cend (), std::back_inserter (valueVector),
        [] (ParseTree *parseTree) -> TimeLiteral::ElementType
        {
          HorseIRParser::TTimeValueContext *value = nullptr;
          if ((value = dynamic_cast<decltype (value)>(parseTree)) != nullptr)
            { return convertTimeValue (value); }
          return TimeLiteral::ElementType (nullptr);
        });
    auto timeLiteral = mem.alloc<TimeLiteral> (literal);
    timeLiteral->setValue (std::move (valueVector));
    using PrimitiveClass = PrimitiveType::PrimitiveClass;
    auto primitiveType = mem.alloc<PrimitiveType> (literal);
    primitiveType->setPrimitiveClass (PrimitiveClass::Time);
    timeLiteral->setLiteralType (primitiveType);
    return timeLiteral;
  }

  static TimeLiteral *
  convert (ASTNodeMemory &mem, LiteralTTimeCase3Context *literal)
  {
    assert (literal != nullptr);
    const auto nullTokens = literal->NULL_TOKEN ();
    std::vector<TimeLiteral::ElementType> valueVector{};
    valueVector.reserve (nullTokens.size ());
    for (std::size_t iter = 0; iter < nullTokens.size (); ++iter)
      { valueVector.emplace_back (TimeLiteral::ElementType (nullptr)); }
    auto timeLiteral = mem.alloc<TimeLiteral> (literal);
    timeLiteral->setValue (std::move (valueVector));
    using PrimitiveClass = PrimitiveType::PrimitiveClass;
    auto primitiveType = mem.alloc<PrimitiveType> (literal);
    primitiveType->setPrimitiveClass (PrimitiveClass::Time);
    timeLiteral->setLiteralType (primitiveType);
    return timeLiteral;
  }

  static TimeLiteral *
  convert (ASTNodeMemory &mem, LiteralTTimeCase4Context *literal)
  {
    assert (literal != nullptr);
    auto timeLiteral = mem.alloc<TimeLiteral> (literal);
    std::vector<TimeLiteral::ElementType> valueVector{};
    valueVector.emplace_back (TimeLiteral::ElementType (nullptr));
    timeLiteral->setValue (std::move (valueVector));
    using PrimitiveClass = PrimitiveType::PrimitiveClass;
    auto primitiveType = mem.alloc<PrimitiveType> (literal);
    primitiveType->setPrimitiveClass (PrimitiveClass::Time);
    timeLiteral->setLiteralType (primitiveType);
    return timeLiteral;
  }

  using LiteralFunction0Context = HorseIRParser::LiteralFunctionCase0Context;
  using LiteralFunction1Context = HorseIRParser::LiteralFunctionCase1Context;

  static FunctionLiteral *
  convert (ASTNodeMemory &mem, LiteralFunctionContext *literal)
  {
    assert (literal != nullptr);
    LiteralFunction0Context *case0 = nullptr;
    LiteralFunction1Context *case1 = nullptr;

    if ((case0 = dynamic_cast<decltype (case0)>(literal)) != nullptr)
      { return convert (mem, case0); }
    if ((case1 = dynamic_cast<decltype (case1)>(literal)) != nullptr)
      { return convert (mem, case1); }

    throw CSTConverterException (literal);
  }

  static FunctionLiteral::ElementType
  convertFunctionValue (HorseIRParser::FunctionValueNContext *valueContext)
  {
    assert (valueContext != nullptr);
    if (valueContext->NULL_TOKEN () != nullptr)
      { return FunctionLiteral::ElementType (nullptr); }
    const auto functionContext = valueContext->LITERAL_FUNCTION ();
    std::string functionString = functionContext->getText ();
    functionString = functionString.substr (1, std::string::npos);
    const auto commaPos = std::find (
        functionString.cbegin (), functionString.cend (), '.'
    );
    std::string moduleName, methodName;
    if (commaPos == functionString.cend ())
      {
        moduleName = "";
        methodName = functionString;
      }
    else
      {
        std::copy (functionString.cbegin (), commaPos,
                   std::back_inserter (moduleName));
        std::copy (std::next (commaPos), functionString.cend (),
                   std::back_inserter (methodName));
      }
    storage::Function function{};
    function.moduleName = std::move (moduleName);
    function.methodName = std::move (methodName);
    return FunctionLiteral::ElementType (std::move (function));
  }

  static FunctionLiteral *
  convert (ASTNodeMemory &mem, LiteralFunction0Context *literal)
  {
    assert (literal != nullptr);
    auto functionLiteral = mem.alloc<FunctionLiteral> (literal);
    std::vector<FunctionLiteral::ElementType> valueVector{};
    const auto functionValueContext = literal->functionValueN ();
    valueVector.emplace_back (convertFunctionValue (functionValueContext));
    functionLiteral->setValue (std::move (valueVector));
    functionLiteral->setLiteralType (convert (mem, literal->typeFunc ()));
    return functionLiteral;
  }

  static FunctionLiteral *
  convert (ASTNodeMemory &mem, LiteralFunction1Context *literal)
  {
    assert (literal != nullptr);
    using FunctionValueContext = HorseIRParser::FunctionValueNContext;
    auto functionLiteral = mem.alloc<FunctionLiteral> (literal);
    const auto functionValueContexts = literal->functionValueN ();
    std::vector<FunctionLiteral::ElementType> valueVector{};
    valueVector.reserve (functionValueContexts.size ());
    std::transform (
        functionValueContexts.cbegin (), functionValueContexts.cend (),
        std::back_inserter (valueVector),
        [] (FunctionValueContext *context) -> FunctionLiteral::ElementType
        { return convertFunctionValue (context); });
    functionLiteral->setValue (std::move (valueVector));
    functionLiteral->setLiteralType (convert (mem, literal->typeFunc ()));
    return functionLiteral;
  }

  using LiteralListCase0Context = HorseIRParser::LiteralListCase0Context;
  using LiteralListCase1Context = HorseIRParser::LiteralListCase1Context;
  using LiteralListCase2Context = HorseIRParser::LiteralListCase2Context;

  static ListLiteral *
  convert (ASTNodeMemory &mem, LiteralListContext *literal)
  {
    assert (literal != nullptr);
    LiteralListCase0Context *case0 = nullptr;
    LiteralListCase1Context *case1 = nullptr;
    LiteralListCase2Context *case2 = nullptr;

    if ((case0 = dynamic_cast<decltype (case0)>(literal)) != nullptr)
      { return convert (mem, case0); }
    if ((case1 = dynamic_cast<decltype (case1)>(literal)) != nullptr)
      { return convert (mem, case1); }
    if ((case2 = dynamic_cast<decltype (case2)>(literal)) != nullptr)
      { return convert (mem, case2); }

    throw CSTConverterException (literal);
  }

  static ListLiteral *
  convert (ASTNodeMemory &mem, LiteralListCase0Context *literal)
  {
    assert (literal != nullptr);
    auto listLiteral = mem.alloc<ListLiteral> ();
    std::vector<ListLiteral::ElementType> valueVector{};
    listLiteral->setValue (std::move (valueVector));
    listLiteral->setLiteralType (convert (mem, literal->typeList ()));
    return listLiteral;
  }

  static ListLiteral *
  convert (ASTNodeMemory &mem, LiteralListCase1Context *literal)
  {
    assert (literal != nullptr);
    const auto literals = literal->literal ();
    std::vector<ListLiteral::ElementType> valueVector{};
    std::transform (
        literals.cbegin (), literals.cend (), std::back_inserter (valueVector),
        [&] (LiteralContext *context) -> ListLiteral::ElementType
        { return convert (mem, context); });
    auto listLiteral = mem.alloc<ListLiteral> ();
    listLiteral->setValue (std::move (valueVector));
    listLiteral->setLiteralType (convert (mem, literal->typeList ()));
    return listLiteral;
  }

  static ListLiteral *
  convert (ASTNodeMemory &mem, LiteralListCase2Context *literal)
  {
    assert (literal != nullptr);
    auto listLiteral = mem.alloc<ListLiteral> ();
    listLiteral->setValue (nullptr);
    listLiteral->setLiteralType (convert (mem, literal->typeList ()));
    return listLiteral;
  }

  using LiteralDictCase0Context = HorseIRParser::LiteralDictCase0Context;
  using LiteralDictCase1Context = HorseIRParser::LiteralDictCase1Context;
  using LiteralDictCase2Context = HorseIRParser::LiteralDictCase2Context;

  static DictionaryLiteral *
  convert (ASTNodeMemory &mem, LiteralDictContext *literal)
  {
    assert (literal != nullptr);
    LiteralDictCase0Context *case0 = nullptr;
    LiteralDictCase1Context *case1 = nullptr;
    LiteralDictCase2Context *case2 = nullptr;

    if ((case0 = dynamic_cast<decltype (case0)>(literal)) != nullptr)
      { return convert (mem, case0); }
    if ((case1 = dynamic_cast<decltype (case1)>(literal)) != nullptr)
      { return convert (mem, case1); }
    if ((case2 = dynamic_cast<decltype (case2)>(literal)) != nullptr)
      { return convert (mem, case2); }

    throw CSTConverterException (literal);
  }

  static DictionaryLiteral *
  convert (ASTNodeMemory &mem, LiteralDictCase0Context *literal)
  {
    assert (literal != nullptr);
    auto dictionaryLiteral = mem.alloc<DictionaryLiteral> ();
    std::vector<DictionaryLiteral::ElementType> valueVector{};
    dictionaryLiteral->setValue (std::move (valueVector));
    dictionaryLiteral->setLiteralType (convert (mem, literal->typeDict ()));
    return dictionaryLiteral;
  }

  static DictionaryLiteral *
  convert (ASTNodeMemory &mem, LiteralDictCase1Context *literal)
  {
    assert (literal != nullptr);
    const auto literals = literal->literal ();
    assert (literals.size () % 2 == 0);
    std::vector<DictionaryLiteral::ElementType> valueVector{};
    for (auto iter = literals.cbegin (); iter != literals.cend (); ++iter)
      {
        DictionaryLiteral::ElementType element{};
        element.key = convert (mem, *iter);
        std::advance (iter, 1);
        element.value = convert (mem, *iter);
        valueVector.emplace_back (element);
      }
    auto dictionaryLiteral = mem.alloc<DictionaryLiteral> ();
    dictionaryLiteral->setValue (std::move (valueVector));
    dictionaryLiteral->setLiteralType (convert (mem, literal->typeDict ()));
    return dictionaryLiteral;
  }

  static DictionaryLiteral *
  convert (ASTNodeMemory &mem, LiteralDictCase2Context *literal)
  {
    assert (literal != nullptr);
    auto dictionaryLiteral = mem.alloc<DictionaryLiteral> ();
    dictionaryLiteral->setValue (nullptr);
    dictionaryLiteral->setLiteralType (convert (mem, literal->typeDict ()));
    return dictionaryLiteral;
  }

  using LiteralTableCase0Context = HorseIRParser::LiteralTableCase0Context;
  using LiteralTableCase1Context = HorseIRParser::LiteralTableCase1Context;
  using LiteralTableCase2Context = HorseIRParser::LiteralTableCase2Context;

  static TableLiteral *
  convert (ASTNodeMemory &mem, LiteralTableContext *literal)
  {
    assert (literal != nullptr);
    LiteralTableCase0Context *case0 = nullptr;
    LiteralTableCase1Context *case1 = nullptr;
    LiteralTableCase2Context *case2 = nullptr;

    if ((case0 = dynamic_cast<decltype (case0)>(literal)) != nullptr)
      { return convert (mem, case0); }
    if ((case1 = dynamic_cast<decltype (case1)>(literal)) != nullptr)
      { return convert (mem, case1); }
    if ((case2 = dynamic_cast<decltype (case2)>(literal)) != nullptr)
      { return convert (mem, case2); }

    throw CSTConverterException (literal);
  }

  static std::string
  convertTableHead (HorseIRParser::TableHeaderContext *context)
  {
    assert (context != nullptr);
    if (context->name () != nullptr) return context->name ()->id->getText ();
    const std::string rawName = context->LITERAL_SYMBOL ()->getText ();
    return rawName.substr (1, std::string::npos);
  }

  static Literal *
  convert (ASTNodeMemory &mem, HorseIRParser::DbContentContext *context)
  {
    assert (context != nullptr);

    if (context->literalBool () != nullptr)
      { return convert (mem, context->literalBool ()); }
    else if (context->literalChar () != nullptr)
      { return convert (mem, context->literalChar ()); }
    else if (context->literalInteger () != nullptr)
      { return convert (mem, context->literalInteger ()); }
    else if (context->literalFloat () != nullptr)
      { return convert (mem, context->literalFloat ()); }
    else if (context->literalComplex () != nullptr)
      { return convert (mem, context->literalComplex ()); }
    else if (context->literalSymbol () != nullptr)
      { return convert (mem, context->literalSymbol ()); }
    else if (context->literalTTime () != nullptr)
      { return convert (mem, context->literalTTime ()); }
    else if (context->literalTSecond () != nullptr)
      { return convert (mem, context->literalTSecond ()); }
    else if (context->literalTMonth () != nullptr)
      { return convert (mem, context->literalTMonth ()); }
    else if (context->literalTMinute () != nullptr)
      { return convert (mem, context->literalTMinute ()); }
    else if (context->literalTDate () != nullptr)
      { return convert (mem, context->literalTDate ()); }
    else if (context->literalTDateTime () != nullptr)
      { return convert (mem, context->literalTDateTime ()); }
    else if (context->literalString () != nullptr)
      { return convert (mem, context->literalString ()); }

    throw CSTConverterException (context);
  }

  static TableLiteral *
  convert (ASTNodeMemory &mem, LiteralTableCase0Context *literal)
  {
    assert (literal != nullptr);
    auto tableLiteral = mem.alloc<TableLiteral> (literal);
    std::vector<TableLiteral::ElementType> valueVector{};
    tableLiteral->setValue (std::move (valueVector));
    auto primitiveType = mem.alloc<PrimitiveType> (literal);
    primitiveType->setPrimitiveClass (PrimitiveType::PrimitiveClass::Table);
    tableLiteral->setLiteralType (primitiveType);
    return tableLiteral;
  }

  static TableLiteral *
  convert (ASTNodeMemory &mem, LiteralTableCase1Context *literal)
  {
    assert (literal != nullptr);
    using TableColumnContext = HorseIRParser::TableColumnContext;
    const auto tableColumns = literal->tableColumn ();
    std::vector<TableLiteral::ElementType> valueVector{};
    valueVector.reserve (tableColumns.size ());
    std::transform (
        tableColumns.cbegin (), tableColumns.cend (),
        std::back_inserter (valueVector),
        [&] (TableColumnContext *element) -> TableLiteral::ElementType
        {
          auto headContext = element->tableHeader ();
          auto dbContentContext = element->dbContent ();
          std::string columnName = convertTableHead (headContext);
          Literal *content = convert (mem, dbContentContext);
          TableLiteral::ElementType returnElement{};
          returnElement.head = std::move (columnName);
          returnElement.content = content;
          return returnElement;
        });
    auto tableLiteral = mem.alloc<TableLiteral> (literal);
    tableLiteral->setValue (std::move (valueVector));
    auto primitiveType = mem.alloc<PrimitiveType> (literal);
    primitiveType->setPrimitiveClass (PrimitiveType::PrimitiveClass::Table);
    tableLiteral->setLiteralType (primitiveType);
    return tableLiteral;
  }

  static TableLiteral *
  convert (ASTNodeMemory &mem, LiteralTableCase2Context *literal)
  {
    assert (literal != nullptr);
    auto tableLiteral = mem.alloc<TableLiteral> (literal);
    tableLiteral->setValue (nullptr);
    auto primitiveType = mem.alloc<PrimitiveType> (literal);
    primitiveType->setPrimitiveClass (PrimitiveType::PrimitiveClass::Table);
    tableLiteral->setLiteralType (primitiveType);
    return tableLiteral;
  }

  using LiteralKeyTableCase0Context = HorseIRParser::LiteralKTableCase0Context;
  using LiteralKeyTableCase1Context = HorseIRParser::LiteralKTableCase1Context;

  static KeyTableLiteral *
  convert (ASTNodeMemory &mem, LiteralKeyTableContext *literal)
  {
    assert (literal != nullptr);
    LiteralKeyTableCase0Context *case0 = nullptr;
    LiteralKeyTableCase1Context *case1 = nullptr;

    if ((case0 = dynamic_cast<decltype (case0)>(literal)) != nullptr)
      { return convert (mem, case0); }
    if ((case1 = dynamic_cast<decltype (case1)>(literal)) != nullptr)
      { return convert (mem, case1); }

    throw CSTConverterException (literal);
  }

  static KeyTableLiteral *
  convert (ASTNodeMemory &mem, LiteralKeyTableCase0Context *literal)
  {
    assert (literal != nullptr);
    using TableKeyedColumnContext = HorseIRParser::TableKeyedColumnContext;
    using TableColumnContext = HorseIRParser::TableColumnContext;
    const auto keyedColumns = literal->tableKeyedColumn ();
    const auto normalColumns = literal->tableColumn ();
    std::vector<KeyTableLiteral::ElementType> valueVector{};
    valueVector.reserve (keyedColumns.size () + normalColumns.size ());

    std::transform (
        keyedColumns.cbegin (), keyedColumns.cend (),
        std::back_inserter (valueVector),
        [&] (TableKeyedColumnContext *context) -> KeyTableLiteral::ElementType
        {
          const auto tableHeaderContext = context->tableHeader ();
          const auto dbContentContext = context->dbContent ();
          std::string tableHead = convertTableHead (tableHeaderContext);
          Literal *dbContent = convert (mem, dbContentContext);
          KeyTableLiteral::ElementType element{};
          element.head = std::move (tableHead);
          element.content = dbContent;
          element.isKey = true;
          return element;
        });
    std::transform (
        normalColumns.cbegin (), normalColumns.cend (),
        std::back_inserter (valueVector),
        [&] (TableColumnContext *context) -> KeyTableLiteral::ElementType
        {
          const auto tableHeaderContext = context->tableHeader ();
          const auto dbContentContext = context->dbContent ();
          std::string tableHead = convertTableHead (tableHeaderContext);
          Literal *dbContent = convert (mem, dbContentContext);
          KeyTableLiteral::ElementType element{};
          element.head = std::move (tableHead);
          element.content = dbContent;
          element.isKey = false;
          return element;
        });

    auto keyTableLiteral = mem.alloc<KeyTableLiteral> (literal);
    keyTableLiteral->setValue (std::move (valueVector));
    auto primitiveType = mem.alloc<PrimitiveType> (literal);
    primitiveType->setPrimitiveClass (PrimitiveType::PrimitiveClass::KeyTable);
    keyTableLiteral->setLiteralType (primitiveType);
    return keyTableLiteral;
  }

  static KeyTableLiteral *
  convert (ASTNodeMemory &mem, LiteralKeyTableCase1Context *literal)
  {
    assert (literal != nullptr);
    auto keyTableLiteral = mem.alloc<KeyTableLiteral> (literal);
    keyTableLiteral->setValue (nullptr);
    auto primitiveType = mem.alloc<PrimitiveType> (literal);
    primitiveType->setPrimitiveClass (PrimitiveType::PrimitiveClass::KeyTable);
    keyTableLiteral->setLiteralType (primitiveType);
    return keyTableLiteral;
  }
};

}
}