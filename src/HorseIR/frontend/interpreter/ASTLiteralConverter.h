#pragma once

#include "../ast/AST.h"

namespace horseIR {
namespace interpreter {

template <typename IntermediateType, typename InterpreterEnvType>
struct ASTLiteralConverter {
    using EnvDispatcher = typename InterpreterEnvType::DispatcherType ;
    using EnvGlobalVarStorage = typename InterpreterEnvType::GlobalVarStorageType ;
    
    static IntermediateType caseNilLiteral(ast::NilLiteral* ast) ;
    static IntermediateType caseComplexLiteral(ast::ComplexLiteral* ast) ;
    static IntermediateType caseBoolLiteral(ast::BoolLiteral* ast) ;
    static IntermediateType caseCharLiteral(ast::CharLiteral* ast) ;
    static IntermediateType caseInterger8Literal(ast::Integer8Literal* ast) ;
    static IntermediateType caseInterger16Literal(ast::Integer16Literal* ast) ;
    static IntermediateType caseInterger32Literal(ast::Integer32Literal* ast) ;
    static IntermediateType caseInterger64Literal(ast::Integer64Literal* ast) ;
    static IntermediateType caseSymbolLiteral(ast::SymbolLiteral* ast) ;
    static IntermediateType caseFunctionLiteral(ast::FunctionLiteral* ast) ;
} ;

}
}
