#include "TypeUtils.h"

using namespace horseIR::ast;

bool TypeUtils::isSame (const Type *lhs, const Type *rhs)
{
  assert (lhs != nullptr && rhs != nullptr);
  const auto lhsTypeClass = lhs->getTypeClass ();
  const auto rhsTypeClass = lhs->getTypeClass ();

  if (lhsTypeClass == Type::TypeClass::Primitive)
    {
      if (rhsTypeClass != Type::TypeClass::Primitive) return false;
      auto castedLHS = dynamic_cast<const PrimitiveType *>(lhs);
      auto castedRHS = dynamic_cast<const PrimitiveType *>(rhs);
      return castedLHS->getPrimitiveClass () == castedRHS->getPrimitiveClass ();
    }
  if (lhsTypeClass == Type::TypeClass::Wildcard)
    { return rhs->getTypeClass () == Type::TypeClass::Wildcard; }
  if (lhsTypeClass == Type::TypeClass::List)
    {
      if (rhsTypeClass != Type::TypeClass::List) return false;
      auto castedLHS = dynamic_cast<const ListType *>(lhs);
      auto castedRHS = dynamic_cast<const ListType *>(rhs);
      const auto lhsElementType = castedLHS->getElementType ();
      const auto rhsElementType = castedRHS->getElementType ();
      return isSame (lhsElementType, rhsElementType);
    }
  if (lhsTypeClass == Type::TypeClass::Enumeration)
    {
      if (rhsTypeClass != Type::TypeClass::Enumeration) return false;
      auto castedLHS = dynamic_cast<const EnumerationType *>(lhs);
      auto castedRHS = dynamic_cast<const EnumerationType *>(rhs);
      const auto lhsElementType = castedLHS->getElementType ();
      const auto rhsElementType = castedRHS->getElementType ();
      return isSame (lhsElementType, rhsElementType);
    }
  if (lhsTypeClass == Type::TypeClass::Dictionary)
    {
      if (rhsTypeClass != Type::TypeClass::Dictionary) return false;
      auto castedLHS = dynamic_cast<const DictionaryType *>(lhs);
      auto castedRHS = dynamic_cast<const DictionaryType *>(rhs);
      return isSame (castedLHS->getKeyType (), castedRHS->getKeyType ()) &&
             isSame (castedLHS->getValueType (), castedRHS->getValueType ());
    }
  if (lhsTypeClass == Type::TypeClass::Function)
    {
      if (rhsTypeClass != Type::TypeClass::Function) return false;
      auto castedLHS = dynamic_cast<const FunctionType *>(lhs);
      auto castedRHS = dynamic_cast<const FunctionType *>(rhs);
      bool paramIsSame = true;
      const auto lhsParamTypes = castedLHS->getParameterTypes ();
      const auto rhsParamTypes = castedRHS->getParameterTypes ();
      if (lhsParamTypes.size () == rhsParamTypes.size ())
        {
          for (std::size_t iter = 0; iter < lhsParamTypes.size (); ++iter)
            if (!isSame (lhsParamTypes[iter], rhsParamTypes[iter]))
              {
                paramIsSame = false;
                break;
              }
        }
      else
        { paramIsSame = false; }
      const bool retIsSame =
          isSame (castedLHS->getReturnType (), castedRHS->getReturnType ());
      const bool flexibleIsSame =
          castedLHS->getIsFlexible () == castedRHS->getIsFlexible ();
      return paramIsSame && retIsSame && flexibleIsSame;
    }
  assert (false);
  return false;
}

bool TypeUtils::isSubType (const Type *lhs, const Type *rhs)
{
  assert (lhs != nullptr && rhs != nullptr);
  const auto lhsTypeClass = lhs->getTypeClass ();
  const auto rhsTypeClass = rhs->getTypeClass ();

  if (rhsTypeClass == Type::TypeClass::Primitive)
    {
      if (lhsTypeClass != Type::TypeClass::Primitive) return false;
      auto castedLHS = dynamic_cast<const PrimitiveType *>(lhs);
      auto castedRHS = dynamic_cast<const PrimitiveType *>(rhs);
      return castedLHS->getPrimitiveClass () == castedRHS->getPrimitiveClass ();
    }
  if (rhsTypeClass == Type::TypeClass::Wildcard)
    { return true; }
  if (rhsTypeClass == Type::TypeClass::List)
    {
      if (lhsTypeClass != Type::TypeClass::List) return false;
      auto castedLHS = dynamic_cast<const ListType *>(lhs);
      auto castedRHS = dynamic_cast<const ListType *>(rhs);
      const auto lhsElementType = castedLHS->getElementType ();
      const auto rhsElementType = castedRHS->getElementType ();
      return isSubType (lhsElementType, rhsElementType);
    }
  if (rhsTypeClass == Type::TypeClass::Enumeration)
    {
      if (lhsTypeClass != Type::TypeClass::Enumeration) return false;
      auto castedLHS = dynamic_cast<const EnumerationType *>(lhs);
      auto castedRHS = dynamic_cast<const EnumerationType *>(rhs);
      const auto lhsElementType = castedLHS->getElementType ();
      const auto rhsElementType = castedRHS->getElementType ();
      return isSubType (lhsElementType, rhsElementType);
    }
  if (rhsTypeClass == Type::TypeClass::Dictionary)
    {
      if (lhsTypeClass != Type::TypeClass::Dictionary) return false;
      auto castedLHS = dynamic_cast<const DictionaryType *>(lhs);
      auto castedRHS = dynamic_cast<const DictionaryType *>(rhs);
      return isSubType (castedLHS->getKeyType (), castedRHS->getKeyType ()) &&
             isSubType (castedLHS->getValueType (), castedRHS->getValueType ());
    }
  if (rhsTypeClass == Type::TypeClass::Function)
    {
      if (lhsTypeClass != Type::TypeClass::Function) return false;

    }
  assert (false);
  return false;
}