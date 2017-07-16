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
            typedef Statement* value_type ;
            StatementIterator() ;
            StatementIterator(Statement* statement) ;
            StatementIterator(const StatementIterator& iterator) ;
            StatementIterator(StatementIterator&& iterator) ;

            value_type nextOnTrue() ;
            value_type nextOnFalse() ;

            StatementIterator::value_type operator *() ;
            StatementIterator& operator =(const StatementIterator& iterator) ;
            StatementIterator& operator =(StatementIterator&& iterator) ;

            bool operator ==(const StatementIterator& iterator) ;
            bool operator !=(const StatementIterator& iterator) ;
        protected:
            Statement* ptr ;
        } ;

        class Statement : public ASTNode {
        public:
            enum class StatementClass {
                Return, Branch, Assign, Label, Phi
            };

            Statement() = delete ;
            Statement(ASTNode* parent, HorseIRParser::StatementContext* cst, ASTNode::MemManagerType& mem, ASTNode::ASTNodeClass type, StatementClass p_StatementClass) ;
            Statement(ASTNode::MemManagerType& mem, ASTNode::ASTNodeClass type, StatementClass p_StatementClass) ;
            Statement(const Statement&) = delete ;
            virtual ~Statement() override = default ;

            StatementClass getStatementClass() const ;
            std::pair<Statement*, Statement*> getOutwardFlow() const ;
            std::vector<Statement*> getInwardFlow() const ;
            Statement& setOutwardFlow(const std::pair<Statement*, Statement*>& flow) ;
            Statement& setOutwardFlow(Statement* trueFlow, Statement* falseFlow) ;
            Statement& appendInwardFlow(Statement* flow) ;
            StatementIterator getIterator() ;

            static Statement* makeStatementASTNode(ASTNode* parent, HorseIRParser::StatementContext* cst, ASTNode::MemManagerType& mem) ;

        protected:
            StatementClass statementClass ;
            std::pair<Statement*, Statement*> outwardFlow ;
            std::vector<Statement*> inwardFlow ;

            void __duplicateShallow(const Statement* statement) ;
            void __duplicateDeep(const Statement* statement, ASTNode::MemManagerType& mem) ;
        } ;

        class ReturnStatement : public Statement {
        public:
            ReturnStatement() = delete ;
            ReturnStatement(ASTNode* parent, HorseIRParser::StmtCoreContext* cst, ASTNode::MemManagerType& mem) ;
            ReturnStatement(ASTNode::MemManagerType& mem) ;
            ReturnStatement(const ReturnStatement&) = delete ;
            ~ReturnStatement() override = default ;

            virtual std::size_t getNumNodesRecursively() const override ;
            virtual std::vector<ASTNode*> getChildren() const override ;
            virtual std::string toString() const override ;
            virtual std::string toTreeString() const override ;

        protected:
            Identifier* id ;

            void __duplicateShallow(const ReturnStatement* returnStmt) ;
            void __duplicateDeep(const ReturnStatement* returnStmt, ASTNode::MemManagerType& mem) ;
        } ;

        class AssignStatement : public Statement {
        public:
            enum class AssignStatementClass {
                Direct, Cast, CheckCast, CheckType
            };
            AssignStatement() = delete ;
            AssignStatement(ASTNode* parent, HorseIRParser::StmtCoreContext* cst, ASTNode::MemManagerType& mem) ;
            AssignStatement(ASTNode::MemManagerType& mem) ;
            AssignStatement(const ReturnStatement&) = delete ;
            ~AssignStatement() override = default ;

            Identifier* getLHSName() const ;
            Type* getLHSType() const ;
            bool isInvocation() const ;
            Operand* getInvokeTarget() const ;
            std::vector<Operand*> getParameters() const ;

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
            inline std::string toStringCaseDirect() const ;
            inline std::string toStringCaseCast() const ;
            inline std::string toStringCaseCheckType() const ;
            inline std::string toStringCaseCheckCast() const ;

            void __duplicateShallow(const AssignStatement* assignStmt) ;
            void __duplicateDeep(const AssignStatement* assignStmt, ASTNode::MemManagerType& mem) ;
        } ;

        class PhiStatement : public Statement {
        public:
            PhiStatement() = delete ;
            PhiStatement(ASTNode* parent, HorseIRParser::StmtCoreContext* cst, ASTNode::MemManagerType& mem) ;
            PhiStatement(ASTNode::MemManagerType& mem) ;
            PhiStatement(const PhiStatement&) = delete ;
            ~PhiStatement() override = default ;

            virtual std::size_t getNumNodesRecursively() const override ;
            virtual std::vector<ASTNode*> getChildren() const override ;
            virtual std::string toString() const override ;
            virtual std::string toTreeString() const override ;

        protected:
            std::map<std::string, Identifier*> inFlowMap ;
            Identifier* lhsID ;
            Type* lhsType ;

            void __duplicateShallow(const PhiStatement* phiStmt) ;
            void __duplicateDeep(const PhiStatement* phiStmt, ASTNode::MemManagerType& mem) ;
        } ;

        class BranchStatement : public Statement {
        public:
            BranchStatement() = delete ;
            BranchStatement(ASTNode* parent, HorseIRParser::StmtCoreContext* cst, ASTNode::MemManagerType& mem) ;
            BranchStatement(ASTNode::MemManagerType& mem) ;
            BranchStatement(const BranchStatement&) = delete ;
            ~BranchStatement() override = default ;

            std::string getTargetLabelName() const ;

            virtual std::size_t getNumNodesRecursively() const override ;
            virtual std::vector<ASTNode*> getChildren() const override ;
            virtual std::string toString() const override ;
            virtual std::string toTreeString() const override ;

        protected:
            std::string targetLabelName ;
            std::pair<bool, Identifier*> checkCondition ;

            void __duplicateShallow(const BranchStatement* branchStmt) ;
            void __duplicateDeep(const BranchStatement* branchStmt, ASTNode::MemManagerType& mem) ;
        } ;

        class LabelStatement : public Statement {
        public:
            LabelStatement() = delete ;
            LabelStatement(ASTNode* parent, HorseIRParser::StmtLabelContext* cst, ASTNode::MemManagerType& mem) ;
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
            Method(ASTNode* parent, HorseIRParser::MethodContext* cst, ASTNode::MemManagerType& mem) ;
            Method(ASTNode::MemManagerType& mem) ;
            ~Method() override = default ;

            StatementIterator begin() const ;
            StatementIterator end() const ;

            std::string getMethodName() const ;
            
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
            Module(ASTNode* parent, HorseIRParser::ModuleContext* cst, ASTNode::MemManagerType& mem) ;
            Module(ASTNode::MemManagerType& mem) ;
            ~Module() override = default ;

            std::string getModuleName() const ;
            std::vector<std::pair<Identifier*, Type*>> getGlobalVariables() const ;
            std::vector<Method*> getMethods() const ;
            Method* getMethod(std::size_t index) const ;
            Method* getMethod(const std::string& methodName) const ;
            
            virtual std::size_t getNumNodesRecursively() const override ;
            virtual std::vector<ASTNode*> getChildren() const override ;
            virtual std::string toString() const override ;
            virtual std::string toTreeString() const override ;

        protected:
            std::string moduleName ;
            std::vector<std::string> importedModules ;
            std::vector<std::pair<Identifier*, Type*>> globalVariables ;
            std::vector<Method*> methods ;
        } ;

        class CompilationUnit : public ASTNode {
        public:
            CompilationUnit() = delete ;
            CompilationUnit(HorseIRParser::ProgramContext* cst, ASTNode::MemManagerType& mem) ;
            CompilationUnit(ASTNode::MemManagerType& mem) ;
            ~CompilationUnit() override = default ;

            CompilationUnit&& merge(CompilationUnit&& obj) ;

            std::vector<Module*> getModules() const ;
            Module* getModule(std::size_t index) const ;
            Module* getModule(const std::string& moduleName) const ;

            virtual std::size_t getNumNodesRecursively() const override ;
            virtual std::vector<ASTNode*> getChildren() const override ;
            virtual std::string toString() const override ;
            virtual std::string toTreeString() const override ;

        protected:
            std::vector<Module*> modules ;
        } ;
    }
}
