#pragma once

#define HORSEIR_AST_DEFAULT_MODULE_NAME     "__DEFAULT"

#include <vector>
#include <string>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <type_traits>
#include <utility>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <memory>
#include <stdexcept>
#include <unordered_map>
#include <antlr4-runtime.h>

#include "HorseIRLexer.h"
#include "HorseIRParser.h"
#include "HorseIRListener.h"
#include "HorseIRVisitor.h"
#include "HorseIRBaseListener.h"
#include "HorseIRBaseVisitor.h"

#include "ASTNode.h"

#include "type/Type.h"
#include "type/PrimitiveType.h"
#include "type/WildcardType.h"
#include "type/ListType.h"
#include "type/DictionaryType.h"
#include "type/EnumerationType.h"
#include "type/FunctionType.h"

#include "operand/Operand.h"
#include "operand/Identifier.h"
#include "operand/Literal.h"
#include "operand/VectorLiteral.h"
#include "operand/BoolLiteral.h"
#include "operand/CharLiteral.h"
#include "operand/StringLiteral.h"
#include "operand/Integer8Literal.h"
#include "operand/Integer16Literal.h"
#include "operand/Integer32Literal.h"
#include "operand/Integer64Literal.h"
#include "operand/FP32Literal.h"
#include "operand/FP64Literal.h"
#include "operand/ComplexLiteral.h"
#include "operand/SymbolLiteral.h"
#include "operand/MonthLiteral.h"
#include "operand/DateLiteral.h"
#include "operand/DateTimeLiteral.h"
#include "operand/MinuteLiteral.h"
#include "operand/SecondLiteral.h"
#include "operand/TimeLiteral.h"
#include "operand/FunctionLiteral.h"
#include "operand/ListLiteral.h"
#include "operand/DictionaryLiteral.h"
#include "operand/TableLiteral.h"
#include "operand/KeyTableLiteral.h"
#include "operand/EnumerationLiteral.h"

#include "statement/Statement.h"
#include "statement/LabelStatement.h"
#include "statement/AssignStatement.h"
#include "statement/InvokeStatement.h"
#include "statement/PhiStatement.h"
#include "statement/ReturnStatement.h"
#include "statement/BranchStatement.h"

#include "Method.h"
#include "Module.h"
#include "CompilationUnit.h"