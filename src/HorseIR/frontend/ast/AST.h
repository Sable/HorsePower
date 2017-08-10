#pragma once

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