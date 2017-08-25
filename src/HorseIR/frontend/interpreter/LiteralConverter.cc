#include <stdexcept>

#include "LiteralConverter.h"
#include "../../backend/h_global.h"

#define NOT_YET_IMPLEMENTED std::runtime_error("not yet implemented")
#define ALLOC_NODE(x) {                                  \
            (x) = allocNode();                           \
            if ((x) == nullptr) throw std::bad_alloc (); \
        }
#define CAST_APPLY(T, func, x) return func(dynamic_cast<const T*>(x));

using namespace horseIR::interpreter;
using namespace horseIR::ast;

V LiteralConverter::convert (const Literal *literal)
{
  assert (literal != nullptr);
  switch (literal->getLiteralClass ())
    {
      case Literal::LiteralClass::Bool:
        { CAST_APPLY (BoolLiteral, caseBoolLiteral, literal); }
      case Literal::LiteralClass::Character:
        { CAST_APPLY (CharLiteral, caseCharLiteral, literal); }
      case Literal::LiteralClass::Integer8:
        { CAST_APPLY (Integer8Literal, caseInteger8Literal, literal); }
      case Literal::LiteralClass::Integer16:
        { CAST_APPLY (Integer16Literal, caseInteger16Literal, literal); }
      case Literal::LiteralClass::Integer32:
        { CAST_APPLY (Integer32Literal, caseInteger32Literal, literal); }
      case Literal::LiteralClass::Integer64:
        { CAST_APPLY (Integer64Literal, caseInteger64Literal, literal); }
      case Literal::LiteralClass::FP32:
        { CAST_APPLY (FP32Literal, caseFP32Literal, literal); }
      case Literal::LiteralClass::FP64:
        { CAST_APPLY (FP64Literal, caseFP64Literal, literal); }
      case Literal::LiteralClass::Complex:
        { CAST_APPLY (ComplexLiteral, caseComplexLiteral, literal); }
      case Literal::LiteralClass::Symbol:
        { CAST_APPLY (SymbolLiteral, caseSymbolLiteral, literal); }
      case Literal::LiteralClass::String:
        { CAST_APPLY (StringLiteral, caseStringLiteral, literal); }
      case Literal::LiteralClass::Date:
        { CAST_APPLY (DateLiteral, caseDateLiteral, literal); }
      case Literal::LiteralClass::DateTime:
        { CAST_APPLY (DateTimeLiteral, caseDateTimeLiteral, literal); }
      case Literal::LiteralClass::Minute:
        { CAST_APPLY (MinuteLiteral, caseMinuteLiteral, literal); }
      case Literal::LiteralClass::Month:
        { CAST_APPLY (MonthLiteral, caseMonthLiteral, literal); }
      case Literal::LiteralClass::Second:
        { CAST_APPLY (SecondLiteral, caseSecondLiteral, literal); }
      case Literal::LiteralClass::Time:
        { CAST_APPLY (TimeLiteral, caseTimeLiteral, literal); }
      case Literal::LiteralClass::Function:
        { throw /* TODO: */ NOT_YET_IMPLEMENTED; }
      case Literal::LiteralClass::List:
        { CAST_APPLY (ListLiteral, caseListLiteral, literal); }
      case Literal::LiteralClass::Dictionary:
        { CAST_APPLY (DictionaryLiteral, caseDictionaryLiteral, literal); }
      case Literal::LiteralClass::Enumeration:
        { CAST_APPLY (EnumerationLiteral, caseEnumerationLiteral, literal); }
      case Literal::LiteralClass::Table:
        { CAST_APPLY (TableLiteral, caseTableLiteral, literal); }
      case Literal::LiteralClass::KeyTable:
        { CAST_APPLY (KeyTableLiteral, caseKeyTableLiteral, literal); }
    }
}

V LiteralConverter::caseBoolLiteral (const BoolLiteral *literal)
{
  assert (literal != nullptr);
  const auto values = literal->getValue ();
  V backendNode = nullptr;
  ALLOC_NODE (backendNode);
  initV (backendNode, H_B, static_cast<L>(values.size ()));
  writeVectorLiteral (
      backendNode, values,
      [] (const BoolLiteral::ElementType &element) -> B
      { throw /* TODO: */ NOT_YET_IMPLEMENTED; },
      [] (const BoolLiteral::ElementType::ValueType &value) -> B
      { return static_cast<B>(value); },
      [] (V v, const B &backendValue, std::size_t pos) -> void
      { vB (v, pos) = backendValue; });
  return backendNode;
}

V LiteralConverter::caseCharLiteral (const CharLiteral *literal)
{
  assert (literal != nullptr);
  const auto values = literal->getValue ();
  V backendNode = nullptr;
  ALLOC_NODE (backendNode);
  initV (backendNode, H_C, static_cast<L>(values.size ()));
  writeVectorLiteral (
      backendNode, values,
      [] (const CharLiteral::ElementType &element) -> C
      { throw /* TODO: */ NOT_YET_IMPLEMENTED; },
      [] (const CharLiteral::ElementType::ValueType &value) -> C
      { return static_cast<C>(value); },
      [] (V v, const C &backendValue, std::size_t pos) -> void
      { vC (v, pos) = backendValue; });
  return backendNode;
}

V LiteralConverter::caseComplexLiteral (const ComplexLiteral *literal)
{
  assert (literal != nullptr);
  const auto values = literal->getValue ();
  V backendNode = nullptr;
  ALLOC_NODE (backendNode);
  initV (backendNode, H_X, static_cast<L>(values.size ()));
  writeVectorLiteral (
      backendNode, values,
      [] (const ComplexLiteral::ElementType &element) -> X
      { throw /* TODO: */ NOT_YET_IMPLEMENTED; },
      [] (const ComplexLiteral::ElementType::ValueType &value) -> X
      {
        X backendValue{};
        backendValue.real = static_cast<F>(value.real ());
        backendValue.imag = static_cast<F>(value.imag ());
        return backendValue;
      },
      [] (V v, const X &backendValue, std::size_t pos) -> void
      { vX (v, pos) = backendValue; });
  return backendNode;
}

V LiteralConverter::caseDateLiteral (const DateLiteral *literal)
{
  assert (literal != nullptr);
  const auto values = literal->getValue ();
  V backendNode = nullptr;
  ALLOC_NODE (backendNode);
  initV (backendNode, H_D, static_cast<L>(values.size ()));
  writeVectorLiteral (
      backendNode, values,
      [] (const DateLiteral::ElementType &element) -> D
      { throw /* TODO: */ NOT_YET_IMPLEMENTED; },
      [] (const DateLiteral::ElementType::ValueType &value) -> D
      {
        auto backendValue = static_cast<D>(0);
        backendValue = backendValue + static_cast<D>(value.year);
        backendValue = backendValue * 100 + static_cast<D>(value.month);
        backendValue = backendValue * 100 + static_cast<D>(value.day);
        return backendValue;
      },
      [] (V v, const D &backendValue, std::size_t pos) -> void
      { vD (v, pos) = backendValue; }
  );
  return backendNode;
}

V LiteralConverter::caseDateTimeLiteral (const DateTimeLiteral *literal)
{
  assert (literal != nullptr);
  const auto values = literal->getValue ();
  V backendNode = nullptr;
  ALLOC_NODE (backendNode);
  initV (backendNode, H_Z, static_cast<L>(values.size ()));
  writeVectorLiteral (
      backendNode, values,
      [] (const DateTimeLiteral::ElementType &element) -> Z
      { throw /* TODO: */ NOT_YET_IMPLEMENTED; },
      [] (const DateTimeLiteral::ElementType::ValueType &value) -> Z
      {
        auto backendValue = static_cast<Z>(0);
        backendValue = backendValue + static_cast<Z>(value.year);
        backendValue = backendValue * 100 + static_cast<Z>(value.month);
        backendValue = backendValue * 100 + static_cast<Z>(value.day);
        backendValue = backendValue * 60 + static_cast<Z>(value.hour);
        backendValue = backendValue * 60 + static_cast<Z>(value.minute);
        backendValue = backendValue * 60 + static_cast<Z>(value.second);
        backendValue = backendValue * 1000 + static_cast<Z>(value.millisecond);
        return backendValue;
      },
      [] (V v, const Z &backendValue, std::size_t pos) -> void
      { vZ (v, pos) = backendValue; }
  );
  return backendNode;
}

V
LiteralConverter::caseDictionaryLiteral (const ast::DictionaryLiteral *literal)
{
  assert (literal != nullptr);
  throw /* TODO: */ NOT_YET_IMPLEMENTED;
}

V LiteralConverter::caseEnumerationLiteral (
    const ast::EnumerationLiteral *literal
)
{
  assert (literal != nullptr);
  throw /* TODO: */ NOT_YET_IMPLEMENTED;
}

V LiteralConverter::caseFP32Literal (const FP32Literal *literal)
{
  assert (literal != nullptr);
  const auto values = literal->getValue ();
  V backendNode = nullptr;
  ALLOC_NODE (backendNode);
  initV (backendNode, H_F, static_cast<L>(values.size ()));
  writeVectorLiteral (
      backendNode, values,
      [] (const FP32Literal::ElementType &element) -> F
      { throw /* TODO: */ NOT_YET_IMPLEMENTED; },
      [] (const FP32Literal::ElementType::ValueType &value) -> F
      { return static_cast<F>(value); },
      [] (V v, const F &backendValue, std::size_t pos) -> void
      { vF (v, pos) = backendValue; });
  return backendNode;
}

V LiteralConverter::caseFP64Literal (const FP64Literal *literal)
{
  assert (literal != nullptr);
  const auto values = literal->getValue ();
  V backendNode = nullptr;
  ALLOC_NODE (backendNode);
  initV (backendNode, H_E, static_cast<L>(values.size ()));
  writeVectorLiteral (
      backendNode, values,
      [] (const FP64Literal::ElementType &element) -> E
      { throw /* TODO: */ NOT_YET_IMPLEMENTED; },
      [] (const FP64Literal::ElementType::ValueType &value) -> E
      { return static_cast<E>(value); },
      [] (V v, const E &backendValue, std::size_t pos) -> void
      { vE (v, pos) = backendValue; });
  return backendNode;
}

V LiteralConverter::caseInteger8Literal (const Integer8Literal *literal)
{
  assert (literal != nullptr);
  throw /* TODO: */ NOT_YET_IMPLEMENTED;
}

V LiteralConverter::caseInteger16Literal (const Integer16Literal *literal)
{
  assert (literal != nullptr);
  const auto values = literal->getValue ();
  V backendNode = nullptr;
  ALLOC_NODE (backendNode);
  initV (backendNode, H_H, static_cast<L>(values.size ()));
  writeVectorLiteral (
      backendNode, values,
      [] (const Integer16Literal::ElementType &element) -> H
      { throw /* TODO: */ NOT_YET_IMPLEMENTED; },
      [] (const Integer16Literal::ElementType::ValueType &value) -> H
      { return static_cast<H>(value); },
      [] (V v, const H &backendValue, std::size_t pos) -> void
      { vH (v, pos) = backendValue; });
  return backendNode;
}

V LiteralConverter::caseInteger32Literal (const Integer32Literal *literal)
{
  assert (literal != nullptr);
  const auto values = literal->getValue ();
  V backendNode = nullptr;
  ALLOC_NODE (backendNode);
  initV (backendNode, H_I, static_cast<L>(values.size ()));
  writeVectorLiteral (
      backendNode, values,
      [] (const Integer32Literal::ElementType &element) -> I
      { throw /* TODO: */ NOT_YET_IMPLEMENTED; },
      [] (const Integer32Literal::ElementType::ValueType &value) -> I
      { return static_cast<I>(value); },
      [] (V v, const I &backendValue, std::size_t pos) -> void
      { vI (v, pos) = backendValue; });
  return backendNode;
}

V LiteralConverter::caseInteger64Literal (const Integer64Literal *literal)
{
  assert (literal != nullptr);
  const auto values = literal->getValue ();
  V backendNode = nullptr;
  ALLOC_NODE (backendNode);
  initV (backendNode, H_L, static_cast<L>(values.size ()));
  writeVectorLiteral (
      backendNode, values,
      [] (const Integer64Literal::ElementType &element) -> L
      { throw /* TODO: */ NOT_YET_IMPLEMENTED; },
      [] (const Integer64Literal::ElementType::ValueType &value) -> L
      { return static_cast<L>(value); },
      [] (V v, const L &backendValue, std::size_t pos) -> void
      { vL (v, pos) = backendValue; });
  return backendNode;
}

V LiteralConverter::caseKeyTableLiteral (const KeyTableLiteral *literal)
{
  assert (literal != nullptr);
  throw /* TODO: */ NOT_YET_IMPLEMENTED;
}

V LiteralConverter::caseListLiteral (const ListLiteral *literal)
{
  assert (literal != nullptr);
  throw /* TODO: */ NOT_YET_IMPLEMENTED;
}

V LiteralConverter::caseMinuteLiteral (const MinuteLiteral *literal)
{
  assert (literal != nullptr);
  const auto values = literal->getValue ();
  V backendNode = nullptr;
  ALLOC_NODE (backendNode);
  initV (backendNode, H_U, static_cast<L>(values.size ()));
  writeVectorLiteral (
      backendNode, values,
      [] (const MinuteLiteral::ElementType &element) -> U
      { throw /* TODO: */ NOT_YET_IMPLEMENTED; },
      [] (const MinuteLiteral::ElementType::ValueType &value) -> U
      {
        auto backendValue = static_cast<U>(0);
        backendValue = backendValue + static_cast<U>(value.hour);
        backendValue = backendValue * 60 + static_cast<U>(value.minute);
        return backendValue;
      },
      [] (V v, const U &backendValue, std::size_t pos) -> void
      { vU (v, pos) = backendValue; });
  return backendNode;
}

V LiteralConverter::caseMonthLiteral (const MonthLiteral *literal)
{
  assert (literal != nullptr);
  const auto values = literal->getValue ();
  V backendNode = nullptr;
  ALLOC_NODE (backendNode);
  initV (backendNode, H_M, static_cast<L>(values.size ()));
  writeVectorLiteral (
      backendNode, values,
      [] (const MonthLiteral::ElementType &element) -> M
      { throw /* TODO: */ NOT_YET_IMPLEMENTED; },
      [] (const MonthLiteral::ElementType::ValueType &value) -> M
      {
        auto backendValue = static_cast<M>(0);
        backendValue = backendValue + static_cast<M>(value.year);
        backendValue = backendValue * 100 + static_cast<M>(value.month);
        return backendValue;
      },
      [] (V v, const M &backendValue, std::size_t pos) -> void
      { vM (v, pos) = backendValue; });
  return backendNode;
}

V LiteralConverter::caseSecondLiteral (const SecondLiteral *literal)
{
  assert (literal != nullptr);
  const auto values = literal->getValue ();
  V backendNode = nullptr;
  ALLOC_NODE (backendNode);
  initV (backendNode, H_W, static_cast<L>(values.size ()));
  writeVectorLiteral (
      backendNode, values,
      [] (const SecondLiteral::ElementType &element) -> W
      { throw /* TODO: */ NOT_YET_IMPLEMENTED; },
      [] (const SecondLiteral::ElementType::ValueType &value) -> W
      {
        auto backendValue = static_cast<W>(0);
        backendValue = backendValue + value.hour;
        backendValue = backendValue * 60 + value.minute;
        backendValue = backendValue * 60 + value.second;
        return backendValue;
      },
      [] (V v, const W &backendValue, std::size_t pos) -> void
      { vW (v, pos) = backendValue; });
  return backendNode;
}

V LiteralConverter::caseStringLiteral (const StringLiteral *literal)
{
  assert (literal != nullptr);
  throw /* TODO: */ NOT_YET_IMPLEMENTED;
}

V LiteralConverter::caseSymbolLiteral (const SymbolLiteral *literal)
{
  assert (literal != nullptr);
  const auto values = literal->getValue ();
  V backendNode = nullptr;
  ALLOC_NODE (backendNode);
  initV (backendNode, H_Q, static_cast<L>(values.size ()));
  writeVectorLiteral (
      backendNode, values,
      [] (const SymbolLiteral::ElementType &element) -> Q
      { throw /* TODO: */ NOT_YET_IMPLEMENTED; },
      [] (const SymbolLiteral::ElementType::ValueType &value) -> Q
      { return static_cast<Q>(getSymbol ((S) (value.c_str ()))); },
      [] (V v, const Q &backendValue, std::size_t pos)
      { vQ (v, pos) = backendValue; });
  return backendNode;
}

V LiteralConverter::caseTableLiteral (const TableLiteral *literal)
{
  assert (literal != nullptr);
  throw /* TODO: */ NOT_YET_IMPLEMENTED;
}

V LiteralConverter::caseTimeLiteral (const TimeLiteral *literal)
{
  assert (literal != nullptr);
  const auto values = literal->getValue ();
  V backendNode = nullptr;
  ALLOC_NODE (backendNode);
  initV (backendNode, H_T, static_cast<L>(values.size ()));
  writeVectorLiteral (
      backendNode, values,
      [] (const TimeLiteral::ElementType &element) -> T
      { throw /* TODO: */ NOT_YET_IMPLEMENTED; },
      [] (const TimeLiteral::ElementType::ValueType &value) -> T
      {
        auto backendValue = static_cast<T>(0);
        backendValue = backendValue + value.hour;
        backendValue = backendValue * 60 + value.minute;
        backendValue = backendValue * 60 + value.second;
        backendValue = backendValue * 1000 + value.millisecond;
        return backendValue;
      },
      [] (V v, const T &backendValue, std::size_t pos) -> void
      { vT (v, pos) = backendValue; });
  return backendNode;
}