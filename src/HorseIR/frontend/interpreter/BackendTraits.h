#pragma once

#include "../ast/AST.h"
#include "MethodMETA.h"

namespace horseIR {
namespace interpreter {

template <typename IntermediateType>
struct BackendTraits {
    static bool isBooleanTrue(IntermediateType backendValue) ;
    static bool isBooleanFalse(IntermediateType backendValue) ;
    static IntermediateType createDuplicate(IntermediateType backendValue) ;
    static IntermediateType packMethodMETA(MethodMETA<IntermediateType>* methodMETA) ;
    static MethodMETA<IntermediateType>* unpackMethodMETA(IntermediateType backendValue) ;
    
    typedef struct {
        static IntermediateType caseNilLiteral(ast::NilLiteral* ast) ;
        static IntermediateType caseComplexLiteral(ast::ComplexLiteral* ast) ;
        static IntermediateType caseBoolLiteral(ast::BoolLiteral* ast) ;
        static IntermediateType caseCharLiteral(ast::CharLiteral* ast) ;
        static IntermediateType caseInteger8Literal(ast::Integer8Literal* ast) ;
        static IntermediateType caseInteger16Literal(ast::Integer16Literal* ast) ;
        static IntermediateType caseInteger32Literal(ast::Integer32Literal* ast) ;
        static IntermediateType caseInteger64Literal(ast::Integer64Literal* ast) ;
        static IntermediateType caseSymbolLiteral(ast::SymbolLiteral* ast) ;
        static IntermediateType caseTableLiteral(ast::TableLiteral* ast) ;
    } ASTLiteralConverter;
} ;

template <>
struct BackendTraits<void*> {
    static bool isBooleanTrue(void* backendValue) {
        return true ;
    }
    static bool isBooleanFalse(void* backendValue) {
        return false ;
    }
    static void* createDuplicate(void* backedValue) {
        return backedValue ;
    }
    static void* packMethodMETA(MethodMETA<void*>* methodMETA) {
        return nullptr ;
    }
    static MethodMETA<void*>* unpackMethodMETA(void* backendValue) {
        return nullptr ;
    }
    
    typedef struct {
        static void* caseNilLiteral(ast::NilLiteral* ast) {
            return nullptr ;
        }
        static void* caseComplexLiteral(ast::ComplexLiteral* ast) {
            return nullptr ;
        }
        static void* caseBoolLiteral(ast::BoolLiteral* ast) {
            return nullptr ;
        }
        static void* caseCharLiteral(ast::CharLiteral* ast) {
            return nullptr ;
        }
        static void* caseInteger8Literal(ast::Integer8Literal* ast) {
            return nullptr ;
        }
        static void* caseInteger16Literal(ast::Integer16Literal* ast) {
            return nullptr ;
        }
        static void* caseInteger32Literal(ast::Integer32Literal* ast) {
            return nullptr ;
        }
        static void* caseInteger64Literal(ast::Integer64Literal* ast) {
            return nullptr ;
        }
        static void* caseSymbolLiteral(ast::SymbolLiteral* ast) {
            return nullptr ;
        }
        static void* caseTableLiteral(ast::TableLiteral* ast) {
            return nullptr ;
        }
    } ASTLiteralConverter; 
} ;

}
}

