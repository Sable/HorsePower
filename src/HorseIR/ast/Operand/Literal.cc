#include <cassert>
#include <Structure.h>

#include "../Operand.h"

using namespace horseIR::ast ;

Literal::Literal(HorseIRParser::LiteralContext *cst, ASTNode::MemManagerType &mem, Literal::LiteralClass p_literalClass,
                 ASTNode::ASTNodeClass type)
    : literalClass(p_literalClass),
      Operand(cst, mem, type, Operand::OperandClass::Literal)
{}

Literal::Literal(ASTNode::MemManagerType &mem, Literal::LiteralClass p_literalClass, ASTNode::ASTNodeClass type)
    : literalClass(p_literalClass),
      Operand(mem, type, Operand::OperandClass::Literal)
{}

Literal::LiteralClass Literal::getLiteralClass() const
{
    return literalClass ;
}

Literal* Literal::makeLiteralASTNode(HorseIRParser::LiteralContext *cst, ASTNode::MemManagerType &mem)
{
    assert(cst != nullptr) ;
    HorseIRParser::LiteralCaseBoolContext* boolContext = nullptr ;
    HorseIRParser::LiteralCaseCharContext* charContext = nullptr ;
    HorseIRParser::LiteralCaseIntegerContext* integerContext = nullptr ;
    HorseIRParser::LiteralCaseFloatContext* floatContext = nullptr ;
    HorseIRParser::LiteralCaseComplexContext* complexContext = nullptr ;
    HorseIRParser::LiteralCaseSymbolContext* symbolContext = nullptr ;
    HorseIRParser::LiteralCaseTimeContext* timeContext = nullptr ;
    HorseIRParser::LiteralCaseFunctionContext* functionContext = nullptr ;
    HorseIRParser::LiteralCaseTableContext* tableContext = nullptr ;
    HorseIRParser::LiteralCaseKtableContext* ktableContext = nullptr ;
    HorseIRParser::LiteralCaseStringContext* stringContext = nullptr ;
    HorseIRParser::LiteralCaseListContext* listContext = nullptr ;
    HorseIRParser::LiteralCaseDictContext* dictContext = nullptr ;
    HorseIRParser::LiteralCaseNilContext* nilContext = nullptr ;

    if ((boolContext = dynamic_cast<decltype(boolContext)>(cst)) != nullptr) {
        BoolLiteral* boolLiteral = new BoolLiteral(boolContext, mem) ;
        return boolLiteral ;
    } else if ((charContext = dynamic_cast<decltype(charContext)>(cst)) != nullptr) {
        CharLiteral* charLiteral = new CharLiteral(charContext, mem) ;
        return charLiteral ;
    } else if ((integerContext = dynamic_cast<decltype(integerContext)>(cst)) != nullptr) {
        IntegerLiteral* integerLiteral = IntegerLiteral::makeIntegerLiteralASTNode(integerContext, mem) ;
        return integerLiteral ;
        /*
    } else if ((floatContext = dynamic_cast<decltype(floatContext)>(cst)) != nullptr) {
        FPLiteral* fpLiteral = FPLiteral::makeFPLiteralASTNode(floatContext, mem) ;
        return fpLiteral ;
         */
    } else if ((complexContext = dynamic_cast<decltype(complexContext)>(cst)) != nullptr) {
        ComplexLiteral* complexLiteral = new ComplexLiteral(complexContext, mem) ;
        return complexLiteral ;
    } else if ((symbolContext = dynamic_cast<decltype(symbolContext)>(cst)) != nullptr) {
        SymbolLiteral* symbolLiteral = new SymbolLiteral(symbolContext, mem) ;
        return symbolLiteral ;
        /*
    } else if ((timeContext = dynamic_cast<decltype(timeContext)>(cst)) != nullptr) {
        TimeLiteral* timeLiteral = TimeLiteral::makeTimeLiteralASTNode(timeContext, mem) ;
        return timeLiteral ;
         */
    } else if ((functionContext = dynamic_cast<decltype(functionContext)>(cst)) != nullptr) {
        FunctionLiteral* functionLiteral = new FunctionLiteral(functionContext, mem) ;
        return functionLiteral ;
    } else if ((tableContext = dynamic_cast<decltype(tableContext)>(cst)) != nullptr) {
        TableLiteral* tableLiteral = new TableLiteral(tableContext, mem) ;
        return tableLiteral ;
        /*
    } else if ((ktableContext = dynamic_cast<decltype(ktableContext)>(cst)) != nullptr) {
        KeyTableLiteral* keyTableLiteral = new KeyTableLiteral(ktableContext, mem) ;
        return keyTableLiteral ;
    } else if ((stringContext = dynamic_cast<decltype(stringContext)>(cst)) != nullptr) {
        StringLiteral* stringLiteral = new StringLiteral(stringContext, mem) ;
        return stringLiteral ;
         */
    } else if ((listContext = dynamic_cast<decltype(listContext)>(cst)) != nullptr) {
        throw std::runtime_error("not yet implement") ;
    } else if ((dictContext = dynamic_cast<decltype(dictContext)>(cst)) != nullptr) {
        throw std::runtime_error("not yet implement") ;
    } else if ((nilContext = dynamic_cast<decltype(nilContext)>(cst)) != nullptr) {
        NilLiteral* nilLiteral = new NilLiteral(nilContext, mem) ;
        return nilLiteral ;
    } else {
        assert(false) ;
        return nullptr ;
    }
}