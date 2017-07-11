#include <cassert>

#include "../AST.h"

using namespace horseIR::ast ;

IntegerLiteral::IntegerLiteral(ASTNode* parent, const antlr4::tree::ParseTree *cst, ASTNode::MemManagerType &mem,
                               ASTNode::ASTNodeClass type, IntegerLiteral::IntegerLiteralClass p_integerLiteralClass)
    : Literal(parent, cst, mem, Literal::LiteralClass::IntegerLiteral, type),
      integerLiteralClass(p_integerLiteralClass)
{}

IntegerLiteral::IntegerLiteral(ASTNode::MemManagerType &mem, ASTNode::ASTNodeClass type,
                               IntegerLiteral::IntegerLiteralClass p_integerLiteralClass)
    : Literal(mem, Literal::LiteralClass::IntegerLiteral, type),
      integerLiteralClass(p_integerLiteralClass)
{}

IntegerLiteral::IntegerLiteralClass IntegerLiteral::getIntegerLiteralClass() const
{
    return integerLiteralClass ;
}

IntegerLiteral* IntegerLiteral::makeIntegerLiteralASTNode(ASTNode* parent, HorseIRParser::LiteralCaseIntegerContext *cst,
                                                          ASTNode::MemManagerType &mem)
{
    assert(cst != nullptr) ;

    auto literalPtr = cst->literalInteger() ;
    auto literalTypeLex = literalPtr->valueType->getText() ;
    if (literalTypeLex == "i8") {
        Integer8Literal* literal = new Integer8Literal(parent, cst, mem) ;
        return literal ;
    } else if (literalTypeLex == "i16") {
        Integer16Literal* literal = new Integer16Literal(parent, cst, mem) ;
        return literal ;
    } else if (literalTypeLex == "i32") {
        Integer32Literal* literal = new Integer32Literal(parent, cst, mem) ;
        return literal ;
    } else if (literalTypeLex == "i64") {
        Integer64Literal* literal = new Integer64Literal(parent, cst, mem) ;
        return literal ;
    } else {
        assert(false) ;
        return nullptr ;
    }
}
