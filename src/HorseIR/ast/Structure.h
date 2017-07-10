#pragma once

#include <utility>
#include <vector>
#include <map>
#include <string>
#include <iterator>

#include "ASTNode.h"
#include "Type.h"
#include "Operand.h"

namespace horseIR {
    namespace ast {
        class Statement ;

        class StatementIterator {
        public:
            StatementIterator() ;
            StatementIterator(Statement* statement) ;
            StatementIterator(StatementIterator& iterator) ;
            StatementIterator(StatementIterator&& iterator) ;

            StatementIterator nextOnTrue() const ;
            StatementIterator nextOnFalse() const ;

            Statement* operator *() ;
            StatementIterator& operator =(StatementIterator& iterator) ;
            StatementIterator& operator =(StatementIterator&& iterator) ;

        protected:
            Statement* ptr ;
        } ;

        class Statement : public ASTNode {
        public:
            enum class StatementClass {
                Return, Branch, Assign, Label, Phi
            };

            Statement() = delete ;
            Statement(HorseIRParser::StatementContext* cst, ASTNode::MemManagerType& mem, ASTNode::ASTNodeClass type, StatementClass p_StatementClass) ;
            Statement(ASTNode::MemManagerType& mem, ASTNode::ASTNodeClass type, StatementClass p_StatementClass) ;
            virtual ~Statement() override = default ;

            StatementClass getStatementClass() const ;
            std::pair<Statement*, Statement*> getOutwardFlow() const ;
            std::vector<Statement*> getInwardFlow() const ;
            Statement& setOutwardFlow(const std::pair<Statement*, Statement*>& flow) ;
            Statement& setOutwardFlow(Statement* trueFlow, Statement* falseFlow) ;
            Statement& appendInwardFlow(Statement* flow) ;
            StatementIterator getIterator() ;

            static Statement* makeStatementASTNode(HorseIRParser::StatementContext* cst, ASTNode::MemManagerType& mem) ;

        protected:
            StatementClass statementClass ;
            std::pair<Statement*, Statement*> outwardFlow ;
            std::vector<Statement*> inwardFlow ;
        } ;

        class ReturnStatement : public Statement {
        public:
            ReturnStatement() = delete ;
            ReturnStatement(HorseIRParser::StmtCoreContext* cst, ASTNode::MemManagerType& mem) ;
            ReturnStatement(ASTNode::MemManagerType& mem) ;
            ~ReturnStatement() override = default ;

            virtual std::size_t getNumNodesRecursively() const override ;
            virtual std::vector<ASTNode*> getChildren() const override ;
            virtual std::string toString() const override ;
            virtual std::string toTreeString() const override ;

        protected:
            Identifier* id ;
        } ;

        class AssignStatement : public Statement {
        public:
            enum class AssignStatementClass {
                Direct, Cast, CheckCast, CheckType
            };
            AssignStatement() = delete ;
            AssignStatement(HorseIRParser::StmtCoreContext* cst, ASTNode::MemManagerType& mem) ;
            AssignStatement(ASTNode::MemManagerType& mem) ;
            ~AssignStatement() override = default ;

            virtual std::size_t getNumNodesRecursively() const override ;
            virtual std::vector<ASTNode*> getChildren() const override ;
            virtual std::string toString() const override ;
            virtual std::string toTreeString() const override ;

        protected:
            std::pair<bool, Operand*> isInvoke ;
            std::pair<AssignStatementClass, Type*> retTypeValidation ;
            std::vector<Operand*> parameters ;
            Identifier* lhsName ;
            Type* lhsType ;

        private:
            inline void parseMethodInvoke(HorseIRParser::MethodCallContext* methodCallContext, ASTNode::MemManagerType& mem) ;
            inline void parseOperand(HorseIRParser::OperandContext* operandContext, ASTNode::MemManagerType& mem) ;
            inline std::string caseDirectToString() const ;
            inline std::string caseCastToString() const ;
            inline std::string caseCheckTypeToString() const ;
            inline std::string caseCheckCastToString() const ;
        } ;

        class PhiStatement : public Statement {
        public:
            PhiStatement() = delete ;
            PhiStatement(HorseIRParser::StmtCoreContext* cst, ASTNode::MemManagerType& mem) ;
            PhiStatement(ASTNode::MemManagerType& mem) ;
            ~PhiStatement() override = default ;

            virtual std::size_t getNumNodesRecursively() const override ;
            virtual std::vector<ASTNode*> getChildren() const override ;
            virtual std::string toString() const override ;
            virtual std::string toTreeString() const override ;

        protected:
            std::map<std::string, Identifier*> inFlowMap ;
            Identifier* lhsID ;
            Type* lhsType ;
        } ;

        class BranchStatement : public Statement {
        public:
            BranchStatement() = delete ;
            BranchStatement(HorseIRParser::StmtCoreContext* cst, ASTNode::MemManagerType& mem) ;
            BranchStatement(ASTNode::MemManagerType& mem) ;
            ~BranchStatement() override = default ;

            std::string getTargetLabelName() const ;

            virtual std::size_t getNumNodesRecursively() const override ;
            virtual std::vector<ASTNode*> getChildren() const override ;
            virtual std::string toString() const override ;
            virtual std::string toTreeString() const override ;

        protected:
            std::string targetLabelName ;
            std::pair<bool, Identifier*> checkCondition ;
        } ;

        class LabelStatement : public Statement {
        public:
            LabelStatement() = delete ;
            LabelStatement(HorseIRParser::StmtLabelContext* cst, ASTNode::MemManagerType& mem) ;
            LabelStatement(ASTNode::MemManagerType& mem) ;
            ~LabelStatement() override = default ;

            std::string getLabelName() const ;

            virtual std::size_t getNumNodesRecursively() const override ;
            virtual std::vector<ASTNode*> getChildren() const override ;
            virtual std::string toString() const override ;
            virtual std::string toTreeString() const override ;

        protected:
            std::string labelName ;
        } ;

        class Method : public ASTNode {
        public:
            Method() = delete ;
            Method(HorseIRParser::MethodContext* cst, ASTNode::MemManagerType& mem) ;
            Method(ASTNode::MemManagerType& mem) ;
            ~Method() override = default ;

            virtual std::size_t getNumNodesRecursively() const override ;
            virtual std::vector<ASTNode*> getChildren() const override ;
            virtual std::string toString() const override ;
            virtual std::string toTreeString() const override ;

        protected:
            std::string methodName ;
            std::vector<std::pair<std::string, Type*>> parameters ;
            Type* returnType ;
            std::vector<Statement*> statements ;

        private:
            void linkStatementFlow() ;
            void inferStatementParameterSignature() ;
        } ;

        class Module : public ASTNode {
        public:
            Module() = delete ;
            Module(HorseIRParser::ModuleContext* cst, ASTNode::MemManagerType& mem) ;
            Module(ASTNode::MemManagerType& mem) ;
            ~Module() override = default ;

            virtual std::size_t getNumNodesRecursively() const override ;
            virtual std::vector<ASTNode*> getChildren() const override ;
            virtual std::string toString() const override ;
            virtual std::string toTreeString() const override ;

        protected:
            std::string moduleName ;
            std::vector<std::string> importedModules ;
            std::vector<std::pair<std::string, Type*>> globalVariables ;
            std::vector<Method*> methods ;
        } ;

        class CompilationUnit : public ASTNode {
        public:
            CompilationUnit() = delete ;
            CompilationUnit(HorseIRParser::ProgramContext* cst, ASTNode::MemManagerType& mem) ;
            CompilationUnit(ASTNode::MemManagerType& mem) ;
            ~CompilationUnit() override = default ;

            CompilationUnit&& merge(CompilationUnit&& obj) ;

            virtual std::size_t getNumNodesRecursively() const override ;
            virtual std::vector<ASTNode*> getChildren() const override ;
            virtual std::string toString() const override ;
            virtual std::string toTreeString() const override ;

        protected:
            std::vector<Module*> modules ;
        } ;
    }
}