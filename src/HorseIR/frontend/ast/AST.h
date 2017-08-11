#pragma once

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
#include <locale>
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
#include "operand/BoolLiteral.h"
#include "operand/CharLiteral.h"