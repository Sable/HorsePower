#include <vector>
#include <cassert>
#include <string>
#include <utility>
#include <sstream>
#include "../grammar/HorseIRParser.h"

#include "../Structure.h"

using namespace horseIR::ast ;

AssignStatement::AssignStatement(HorseIRParser::StmtCoreContext *cst, ASTNode::MemManagerType &mem,
                                 ASTNode::ASTNodeClass type)
    : Statement(cst, mem, ASTNode::ASTNodeClass::AssignStatement, StatementClass::Assign)
{
    assert(cst != nullptr) ;
    assert(cst->statementCore() != nullptr) ;
    auto stmtNameExprContext = dynamic_cast<HorseIRParser::StmtNameExprContext*>(cst->statementCore()) ;
    assert(stmtNameExprContext != nullptr) ;

    lhsName = new Identifier(stmtNameExprContext->generalName(), mem) ;
    lhsType = Type::makeTypeASTNode(stmtNameExprContext->type(), mem) ;

    HorseIRParser::ExpressionContext* expressionContext = stmtNameExprContext->expression() ;
    HorseIRParser::ExprBasicContext* basicContext = nullptr ;
    HorseIRParser::ExprBasicTypeContext* basicTypeContext = nullptr ;
    HorseIRParser::ExprCheckTypeContext* checkTypeContext = nullptr ;
    HorseIRParser::ExprCheckCastContext* checkCastContext = nullptr ;
    if ((basicContext = dynamic_cast<decltype(basicContext)>(expressionContext)) != nullptr) {
        if (basicContext->methodCall() != nullptr) {
            parseMethodInvoke(basicContext->methodCall(), mem) ;
        } else if (basicContext->operand() != nullptr) {
            parseOperand(basicContext->operand(), mem) ;
        } else {
            assert(false) ;
        }
        retTypeValidation = std::make_pair(AssignStatementClass::Direct, nullptr) ;
    } else if ((basicTypeContext = dynamic_cast<decltype(basicTypeContext)>(expressionContext)) != nullptr) {
        if (basicTypeContext->methodCall() != nullptr) {
            parseMethodInvoke(basicTypeContext->methodCall(), mem) ;
        } else if (basicTypeContext->operand() != nullptr) {
            parseOperand(basicTypeContext->operand(), mem) ;
        } else {
            assert(false) ;
        }
        Type* castType = Type::makeTypeASTNode(basicTypeContext->type(), mem) ;
        retTypeValidation = std::make_pair(AssignStatementClass::Cast, castType) ;
    } else if ((checkTypeContext = dynamic_cast<decltype(checkTypeContext)>(expressionContext)) != nullptr) {
        if (checkTypeContext->methodCall() != nullptr) {
            parseMethodInvoke(checkTypeContext->methodCall(), mem) ;
        } else if (checkTypeContext->operand() != nullptr) {
            parseOperand(checkTypeContext->operand(), mem) ;
        } else {
            assert(false) ;
        }
        Type* checkType = Type::makeTypeASTNode(checkTypeContext->type(), mem) ;
        retTypeValidation = std::make_pair(AssignStatementClass::CheckType, checkType) ;
    } else if ((checkCastContext = dynamic_cast<decltype(checkCastContext)>(expressionContext)) != nullptr) {
        if (checkCastContext->methodCall() != nullptr) {
            parseMethodInvoke(checkCastContext->methodCall(), mem) ;
        } else if (checkCastContext->operand() != nullptr) {
            parseOperand(checkCastContext->operand(), mem) ;
        } else {
            assert(false) ;
        }
        Type* castType = Type::makeTypeASTNode(checkCastContext->type(), mem) ;
        retTypeValidation = std::make_pair(AssignStatementClass::CheckCast, castType) ;
    } else {
        assert(false) ;
    }
}

inline void AssignStatement::parseMethodInvoke(HorseIRParser::MethodCallContext *methodCallContext,
                                        ASTNode::MemManagerType &mem)
{
    assert(methodCallContext != nullptr) ;
    HorseIRParser::MethodInvContext* methodInvContext = nullptr ;
    HorseIRParser::MethodFunContext* methodFunContext = nullptr ;
    if ((methodInvContext = dynamic_cast<decltype(methodInvContext)>(methodCallContext)) != nullptr) {
        Operand* invokeTarget = new Identifier(methodInvContext->generalName(), mem) ;
        isInvoke = std::make_pair(true, invokeTarget) ;
        HorseIRParser::ArgumentListContext* argumentListContext = methodInvContext->argumentList() ;
        const auto argumentOperands (argumentListContext->operand()) ;
        for (auto iter = argumentOperands.cbegin(); iter != argumentOperands.cend(); ++iter) {
            if ((*iter)->generalName() != nullptr) {
                Operand* operand = new Identifier((*iter)->generalName(), mem) ;
                parameters.push_back(operand) ;
            } else if ((*iter)->literal() != nullptr) {
                Operand* operand = Literal::makeLiteralASTNode((*iter)->literal(), mem) ;
                parameters.push_back(operand) ;
            } else {
                assert(false) ;
            }
        }
    } else if ((methodFunContext = dynamic_cast<decltype(methodFunContext)>(methodCallContext)) != nullptr) {
        Operand* invokeTarget = Literal::makeLiteralASTNode(static_cast<HorseIRParser::LiteralContext*>(methodFunContext->literalFunction()), mem) ;
        isInvoke = std::make_pair(true, invokeTarget) ;
        HorseIRParser::ArgumentListContext* argumentListContext = methodFunContext->argumentList() ;
        const auto argumentOperands (argumentListContext->operand()) ;
        for (auto iter = argumentOperands.cbegin(); iter != argumentOperands.cend(); ++iter) {
            if ((*iter)->generalName() != nullptr) {
                Operand* operand = new Identifier((*iter)->generalName(), mem) ;
                parameters.push_back(operand) ;
            } else if ((*iter)->literal() != nullptr) {
                Operand* operand = Literal::makeLiteralASTNode((*iter)->literal(), mem) ;
                parameters.push_back(operand) ;
            } else {
                assert(false) ;
            }
        }
    } else {
        assert(false) ;
    }
}

inline void AssignStatement::parseOperand(HorseIRParser::OperandContext *operandContext, ASTNode::MemManagerType &mem)
{
    assert(operandContext != nullptr) ;
    isInvoke = std::make_pair(false, nullptr) ;
    if (operandContext->generalName() != nullptr) {
        Operand* operand = new Identifier(operandContext->generalName(), mem) ;
        parameters.push_back(operand) ;
    } else if (operandContext->literal() != nullptr) {
        Operand* operand = Literal::makeLiteralASTNode(operandContext->literal(), mem) ;
        parameters.push_back(operand) ;
    } else {
        assert(false) ;
    }
}

AssignStatement::AssignStatement(ASTNode::MemManagerType &mem, ASTNode::ASTNodeClass type)
    : Statement(mem, ASTNode::ASTNodeClass::AssignStatement, StatementClass::Assign),
      isInvoke{std::make_pair(false, nullptr)},
      retTypeValidation{std::make_pair(AssignStatementClass::Direct, nullptr)},
      lhsName{nullptr},
      lhsType{nullptr}
{}

std::size_t AssignStatement::getNumNodesRecursively() const
{
    std::size_t count = 1 ;
    count += (isInvoke.first)? isInvoke.second->getNumNodesRecursively() : 0 ;
    count += (retTypeValidation == AssignStatementClass::Direct)? 0 : retTypeValidation.second->getNumNodesRecursively() ;
    for (auto iter = parameters.cbegin(); iter != parameters.cend(); ++iter) {
        count = count + (*iter)->getNumNodesRecursively() ;
    }
    count += lhsName->getNumNodesRecursively() ;
    count += lhsType->getNumNodesRecursively() ;
    return count ;
}

std::vector<ASTNode*> AssignStatement::getChildren() const
{
    std::vector<ASTNode*> retVector ;
    retVector.push_back(static_cast<ASTNode*>(lhsName)) ;
    retVector.push_back(static_cast<ASTNode*>(lhsType)) ;
    switch (retTypeValidation.first) {
        case AssignStatementClass::Direct: {
            if (isInvoke.first) retVector.push_back(static_cast<ASTNode*>(isInvoke.second)) ;
            for (auto iter = parameters.cbegin(); iter != parameters.cend(); ++iter) {
                retVector.push_back(static_cast<ASTNode*>(*iter)) ;
            }
            break ;
        }
        case AssignStatementClass::Cast: {
            retVector.push_back(static_cast<ASTNode*>(retTypeValidation.second)) ;
            if (isInvoke.first) retVector.push_back(static_cast<ASTNode*>(isInvoke.second)) ;
            for (auto iter = parameters.cbegin(); iter != parameters.cend(); ++iter) {
                retVector.push_back(static_cast<ASTNode*>(*iter)) ;
            }
            break ;
        }
        case AssignStatementClass::CheckCast: {
            if (isInvoke.first) retVector.push_back(static_cast<ASTNode*>(isInvoke.second)) ;
            for (auto iter = parameters.cbegin(); iter != parameters.cend(); ++iter) {
                retVector.push_back(static_cast<ASTNode*>(*iter)) ;
            }
            retVector.push_back(static_cast<ASTNode*>(retTypeValidation.second)) ;
            break ;
        }
        case AssignStatementClass::CheckType: {
            if (isInvoke.first) retVector.push_back(static_cast<ASTNode*>(isInvoke.second)) ;
            for (auto iter = parameters.cbegin(); iter != parameters.cend(); ++iter) {
                retVector.push_back(static_cast<ASTNode*>(*iter)) ;
            }
            retVector.push_back(static_cast<ASTNode*>(retTypeValidation.second)) ;
            break ;
        }
    }
    return retVector ;
}

inline std::string AssignStatement::caseDirectToString() const
{
    assert(retTypeValidation.first == AssignStatementClass::Direct) ;
    std::ostringstream stream ;
    stream << lhsName->toString() << " :" << lhsType->toString() << " = " ;
    if (isInvoke.first) {
        stream << isInvoke.second->toString() << "(" ;
        for (auto iter = parameters.cbegin(); iter != parameters.cend(); ++iter) {
            if (iter + 1 != parameters.cend()) {
                stream << (*iter)->toString() << ", " ;
            } else {
                stream << (*iter)->toString() ;
            }
        }
        stream << ')' ;
    } else {
        stream << parameters[0]->toString() ;
    }
    stream << " ;" ;
    return stream.str() ;
}

inline std::string AssignStatement::caseCastToString() const
{
    assert(retTypeValidation.first == AssignStatementClass::Cast) ;
    std::ostringstream stream ;
    stream << lhsName->toString() << " :" << lhsType->toString() << " = "
           << '(' << retTypeValidation.second->toString() << ')' << ' ' ;
    if (isInvoke.first) {
        stream << isInvoke.second->toString() << "(" ;
        for (auto iter = parameters.cbegin(); iter != parameters.cend(); ++iter) {
            if (iter + 1 != parameters.cend()) {
                stream << (*iter)->toString() << ", " ;
            } else {
                stream << (*iter)->toString() ;
            }
        }
        stream << ')' ;
    } else {
        stream << parameters[0]->toString() ;
    }
    stream << " ;" ;
    return stream.str() ;
}

inline std::string AssignStatement::caseCheckTypeToString() const
{
    assert(retTypeValidation.first == AssignStatementClass::CheckType) ;
    std::ostringstream stream ;
    stream << lhsName->toString() << " :" << lhsType->toString() << " = "
           << "check_type(" ;
    if (isInvoke.first) {
        stream << isInvoke.second->toString() << "(" ;
        for (auto iter = parameters.cbegin(); iter != parameters.cend(); ++iter) {
            if (iter + 1 != parameters.cend()) {
                stream << (*iter)->toString() << ", " ;
            } else {
                stream << (*iter)->toString() ;
            }
        }
        stream << ')' ;
    } else {
        stream << parameters[0]->toString() ;
    }
    stream << ", " << retTypeValidation.second->toString() << ") ;" ;
    return stream.str() ;
}

inline std::string AssignStatement::caseCheckCastToString() const
{
    assert(retTypeValidation.first == AssignStatementClass::CheckCast) ;
    std::ostringstream stream ;
    stream << lhsName->toString() << " :" << lhsType->toString() << " = "
           << "check_cast(" ;
    if (isInvoke.first) {
        stream << isInvoke.second->toString() << "(" ;
        for (auto iter = parameters.cbegin(); iter != parameters.cend(); ++iter) {
            if (iter + 1 != parameters.cend()) {
                stream << (*iter)->toString() << ", " ;
            } else {
                stream << (*iter)->toString() ;
            }
        }
        stream << ')' ;
    } else {
        stream << parameters[0]->toString() ;
    }
    stream << ", " << retTypeValidation.second->toString() << ") ;" ;
    return stream.str() ;
}

std::string AssignStatement::toString() const
{
    switch (retTypeValidation.first) {
        case AssignStatementClass::Direct : return caseDirectToString() ;
        case AssignStatementClass::Cast: return caseCastToString() ;
        case AssignStatementClass::CheckType: return caseCheckTypeToString() ;
        case AssignStatementClass::CheckCast: return caseCheckCastToString() ;
    }
}

std::string AssignStatement::toTreeString() const
{
    return "(AssignStatement)" ;
}