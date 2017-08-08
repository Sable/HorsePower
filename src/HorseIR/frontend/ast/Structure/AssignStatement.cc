#include <vector>
#include <cassert>
#include <string>
#include <utility>
#include <sstream>

#include "../AST.h"

using namespace horseIR::ast ;

AssignStatement::AssignStatement(ASTNode* parent, HorseIRParser::StmtCoreContext *cst, ASTNode::MemManagerType &mem)
    : Statement(parent, cst, mem, ASTNode::ASTNodeClass::AssignStatement, StatementClass::Assign)
{
    assert(cst != nullptr) ;
    assert(cst->statementCore() != nullptr) ;
    auto stmtNameExprContext = dynamic_cast<HorseIRParser::StmtNameExprContext*>(cst->statementCore()) ;
    assert(stmtNameExprContext != nullptr) ;

    lhsName = new Identifier(this, stmtNameExprContext->name(), mem) ;
    lhsType = Type::makeTypeASTNode(this, stmtNameExprContext->type(), mem) ;

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
        Type* castType = Type::makeTypeASTNode(this, basicTypeContext->type(), mem) ;
        retTypeValidation = std::make_pair(AssignStatementClass::Cast, castType) ;
    } else if ((checkTypeContext = dynamic_cast<decltype(checkTypeContext)>(expressionContext)) != nullptr) {
        if (checkTypeContext->methodCall() != nullptr) {
            parseMethodInvoke(checkTypeContext->methodCall(), mem) ;
        } else if (checkTypeContext->operand() != nullptr) {
            parseOperand(checkTypeContext->operand(), mem) ;
        } else {
            assert(false) ;
        }
        Type* checkType = Type::makeTypeASTNode(this, checkTypeContext->type(), mem) ;
        retTypeValidation = std::make_pair(AssignStatementClass::CheckType, checkType) ;
    } else if ((checkCastContext = dynamic_cast<decltype(checkCastContext)>(expressionContext)) != nullptr) {
        if (checkCastContext->methodCall() != nullptr) {
            parseMethodInvoke(checkCastContext->methodCall(), mem) ;
        } else if (checkCastContext->operand() != nullptr) {
            parseOperand(checkCastContext->operand(), mem) ;
        } else {
            assert(false) ;
        }
        Type* castType = Type::makeTypeASTNode(this, checkCastContext->type(), mem) ;
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
        Operand* invokeTarget = new Identifier(this, methodInvContext->name(), mem) ;
        isInvoke = std::make_pair(true, invokeTarget) ;
        HorseIRParser::ArgumentListContext* argumentListContext = methodInvContext->argumentList() ;
        const auto argumentOperands (argumentListContext->operand()) ;
        for (auto iter = argumentOperands.cbegin(); iter != argumentOperands.cend(); ++iter) {
            if ((*iter)->name() != nullptr) {
                Operand* operand = new Identifier(this, (*iter)->name(), mem) ;
                parameters.push_back(operand) ;
            } else if ((*iter)->literal() != nullptr) {
                Operand* operand = Literal::makeLiteralASTNode(this, (*iter)->literal(), mem) ;
                parameters.push_back(operand) ;
            } else {
                assert(false) ;
            }
        }
    } else if ((methodFunContext = dynamic_cast<decltype(methodFunContext)>(methodCallContext)) != nullptr) {
        Operand* invokeTarget = new FunctionLiteral(this, methodFunContext->literalFunction(), mem) ;
        isInvoke = std::make_pair(true, invokeTarget) ;
        HorseIRParser::ArgumentListContext* argumentListContext = methodFunContext->argumentList() ;
        const auto argumentOperands (argumentListContext->operand()) ;
        for (auto iter = argumentOperands.cbegin(); iter != argumentOperands.cend(); ++iter) {
            if ((*iter)->name() != nullptr) {
                Operand* operand = new Identifier(this, (*iter)->name(), mem) ;
                parameters.push_back(operand) ;
            } else if ((*iter)->literal() != nullptr) {
                Operand* operand = Literal::makeLiteralASTNode(this, (*iter)->literal(), mem) ;
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
    if (operandContext->name() != nullptr) {
        Operand* operand = new Identifier(this, operandContext->name(), mem) ;
        parameters.push_back(operand) ;
    } else if (operandContext->literal() != nullptr) {
        Operand* operand = Literal::makeLiteralASTNode(this, operandContext->literal(), mem) ;
        parameters.push_back(operand) ;
    } else {
        assert(false) ;
    }
}

AssignStatement::AssignStatement(ASTNode::MemManagerType &mem)
    : Statement(mem, ASTNode::ASTNodeClass::AssignStatement, StatementClass::Assign),
      isInvoke{std::make_pair(false, nullptr)},
      retTypeValidation{std::make_pair(AssignStatementClass::Direct, nullptr)},
      lhsName{nullptr},
      lhsType{nullptr}
{}

Identifier* AssignStatement::getLHSName() const
{
    return lhsName ;
}

Type* AssignStatement::getLHSType() const
{
    return lhsType ;
}

bool AssignStatement::getIsInvocation() const
{
    return isInvoke.first ;
}

Operand* AssignStatement::getInvokeTarget() const
{
    return (isInvoke.first? isInvoke.second : nullptr) ;
}

std::vector<Operand*> AssignStatement::getParameters() const
{
    return parameters ;
}

std::size_t AssignStatement::getNumNodesRecursively() const
{
    std::size_t count = 1 ;
    count += (isInvoke.first)? isInvoke.second->getNumNodesRecursively() : 0 ;
    count += (retTypeValidation.first == AssignStatementClass::Direct)? 0 : retTypeValidation.second->getNumNodesRecursively() ;
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

inline std::string AssignStatement::toStringCaseDirect() const
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

inline std::string AssignStatement::toStringCaseCast() const
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

inline std::string AssignStatement::toStringCaseCheckType() const
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

inline std::string AssignStatement::toStringCaseCheckCast() const
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
    case AssignStatementClass::Direct : return toStringCaseDirect() ;
    case AssignStatementClass::Cast: return toStringCaseCast() ;
    case AssignStatementClass::CheckType: return toStringCaseCheckType() ;
    case AssignStatementClass::CheckCast: return toStringCaseCheckCast() ;
    }
}

std::string AssignStatement::toTreeString() const
{
    return "(AssignStatement)" ;
}

void AssignStatement::__duplicateShallow(const AssignStatement *assignStmt)
{
    assert(assignStmt != nullptr) ;
    Statement::__duplicateShallow(assignStmt) ;
    isInvoke = std::make_pair(assignStmt->isInvoke.first, assignStmt->isInvoke.second) ;
    retTypeValidation = std::make_pair(assignStmt->retTypeValidation.first, assignStmt->retTypeValidation.second) ;
    parameters = assignStmt->parameters ;
    lhsName = assignStmt->lhsName ;
    lhsType = assignStmt->lhsType ;
    return ;
}

void AssignStatement::__duplicateDeep(const AssignStatement *assignStmt, ASTNode::MemManagerType &mem) {
    assert(assignStmt != nullptr);
    Statement::__duplicateDeep(assignStmt, mem);
    Operand *duplicateInvokeOperand = nullptr;
    if (assignStmt->isInvoke.second != nullptr) {
        duplicateInvokeOperand = static_cast<Operand *>(assignStmt->isInvoke.second->duplicateDeep(mem));
        (void) duplicateInvokeOperand->setParentASTNode(this);
    }
    isInvoke = std::make_pair(assignStmt->isInvoke.first, duplicateInvokeOperand);
    Type *duplicateRetTypeVal = nullptr;
    if (assignStmt->retTypeValidation.second != nullptr) {
        duplicateRetTypeVal = static_cast<Type *>(assignStmt->retTypeValidation.second->duplicateDeep(mem));
        (void) duplicateRetTypeVal->setParentASTNode(this);
    }
    retTypeValidation = std::make_pair(assignStmt->retTypeValidation.first, duplicateRetTypeVal) ;
    std::vector<Operand*> duplicateParameters {} ;
    for (auto iter = assignStmt->parameters.cbegin(); iter != assignStmt->parameters.cend(); ++iter) {
        assert(*iter != nullptr) ;
        Operand* duplicateParam = static_cast<Operand*>((*iter)->duplicateDeep(mem)) ;
        (void) duplicateParam->setParentASTNode(this) ;
        duplicateParameters.push_back(duplicateParam) ;
    }
    parameters = std::move(duplicateParameters) ;
    Identifier* duplicateLHSName = nullptr ;
    if (assignStmt->lhsName != nullptr) {
        duplicateLHSName = static_cast<Identifier*>(assignStmt->lhsName->duplicateDeep(mem)) ;
        (void) duplicateLHSName->setParentASTNode(this) ;
    }
    lhsName = duplicateLHSName ;
    Type* duplicateLHSType = nullptr ;
    if (assignStmt->lhsType != nullptr) {
        duplicateLHSType = static_cast<Type*>(assignStmt->lhsType->duplicateDeep(mem)) ;
        (void) duplicateLHSType->setParentASTNode(this) ;
    }
    lhsType = duplicateLHSType ;
    return ;
}
