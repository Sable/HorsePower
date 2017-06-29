#pragma once

#include <string>
#include <complex>
#include <cstdint>
#include <tuple>

#include "ASTNode.h"
#include "Type.h"
#include "antlr4-runtime.h"
#include "../grammar/HorseIRParser.h"

namespace horseIR {
    namespace ast {
        class Literal : public ASTNode {
        public:
            enum class LiteralClass {

            } ;
            Literal() = delete ;
            Literal(antlr4::tree::ParseTree* cst, ASTNode::MemManagerType& mem, Literal::LiteralClass, ASTNode::ASTNodeType type) ;
            Literal(ASTNode::MemManagerType& mem, Literal::LiteralClass p_literalClass, ASTNode::ASTNodeType type) ;

            constexpr Literal::LiteralClass getLiteralClass() const ;
            virtual horseIR::ast::Type* getLiteralType() const = 0;

            static Literal* makeLiteralASTNode(HorseIRParser::LiteralContext* cst, ASTNode::MemManagerType& mem) ;
        protected:
            const Literal::LiteralClass literalClass ;
        } ;

        class NilLiteral : public Literal {
        public:
            typedef std::nullptr_t InternalType ;
            
            NilLiteral() = delete ;
            NilLiteral(HorseIRParser::LiteralNilContext* cst, ASTNode::MemManagerType& mem) ;
            NilLiteral(ASTNode::MemManagerType& mem) ;

            virtual horseIR::ast::Type* getLiteralType() const override ;
            InternalType getValue() const ;

            virtual std::size_t getNumNodesRecursively() const override ;
            virtual std::vector<ASTNode*> getChildren() const override ;
            virtual std::string toString() const override ;
            virtual std::string toTreeString() const override ;
        protected:
            Type* type ;
        } ;

        class ComplexLiteral : public Literal {
        public:
            typedef std::complex<double> InternalType ;

            ComplexLiteral() = delete ;
            ComplexLiteral(HorseIRParser::LiteralComplexContext* cst, ASTNode::MemManagerType& mem) ;
            ComplexLiteral(HorseIRParser::LiteralComplexCase0Context* cst, ASTNode::MemManagerType& mem) ;
            ComplexLiteral(HorseIRParser::LiteralComplexCase1Context* cst, ASTNode::MemManagerType& mem) ;
            ComplexLiteral(HorseIRParser::LiteralComplexCase2Context* cst, ASTNode::MemManagerType& mem) ;
            ComplexLiteral(HorseIRParser::LiteralComplexCase3Context* cst, ASTNode::MemManagerType& mem) ;
            ComplexLiteral(ASTNode::MemManagerType& mem) ;

            virtual horseIR::ast::Type* getLiteralType() const override ;
            InternalType getValue() const ;

            virtual std::size_t getNumNodesRecursively() const override ;
            virtual std::vector<ASTNode*> getChildren() const override ;
            virtual std::string toString() const override ;
            virtual std::string toTreeString() const override ;

        protected:
            Type* type ;
            InternalType value ;
        } ;

        class BoolLiteral : public Literal {
        public:
            typedef std::int_least32_t InternalType ;

            BoolLiteral() = delete ;
            BoolLiteral(HorseIRParser::LiteralBoolContext* cst, ASTNode::MemManagerType& mem) ;
            BoolLiteral(ASTNode::MemManagerType& mem) ;

            virtual horseIR::ast::Type* getLiteralType() const override ;
            InternalType getValue() const ;
            bool getBoolValue() const ;

            virtual std::size_t getNumNodesRecursively() const override ;
            virtual std::vector<ASTNode*> getChildren() const override ;
            virtual std::string toString() const override ;
            virtual std::string toTreeString() const override ;
        } ;

        class CharLiteral : public Literal {
        public:
            typedef char InternalType ;

            CharLiteral() = delete ;
            CharLiteral(HorseIRParser::LiteralCharContext* cst, ASTNode::MemManagerType& mem) ;
            CharLiteral(ASTNode::MemManagerType& mem) ;

            virtual horseIR::ast::Type* getLiteralType() const override ;
            InternalType getValue() const ;

            virtual std::size_t getNumNodesRecursively() const override ;
            virtual std::vector<ASTNode*> getChildren() const override ;
            virtual std::string toString() const override ;
            virtual std::string toTreeString() const override ;
        } ;

        class IntegerLiteral : public Literal {
        public:
            enum class IntegerLiteralClass {
                Integer8Literal, Integer16Literal, Integer32Literal, Integer64Literal
            } ;

            IntegerLiteral() = delete ;
            IntegerLiteral(HorseIRParser::LiteralIntegerContext* cst,
                           ASTNode::MemManagerType& mem,
                           ASTNode::ASTNodeType type,
                           IntegerLiteral::IntegerLiteralClass p_integerLiteralClass) ;
            IntegerLiteral(ASTNode::MemManagerType& mem,
                           ASTNode::ASTNodeType type,
                           IntegerLiteral::IntegerLiteralClass p_integerLiteralClass) ;
            
            virtual std::int8_t  getInt8Value()  const = 0 ;
            virtual std::int16_t getInt16Value() const = 0 ;
            virtual std::int32_t getInt32Value() const = 0 ;
            virtual std::int64_t getInt64Value() const = 0 ;
            IntegerLiteralClass getIntegerLiteralClass() const ;

            static IntegerLiteral* makeIntegerLiteralASTNode(HorseIRParser::LiteralIntegerContext* cst, ASTNode::MemManagerType& mem) ;
        protected:
            IntegerLiteral::IntegerLiteralClass integerLiteralClass ;
        } ;

        class Integer8Literal : public IntegerLiteral {
        public:
            typedef std::int_fast8_t InternalType ;
            
            Integer8Literal() = delete ;
            Integer8Literal(HorseIRParser::LiteralIntegerContext* cst, ASTNode::MemManagerType& mem) ;
            Integer8Literal(ASTNode::MemManagerType& mem) ;

            virtual horseIR::ast::Type* getLiteralType() const override ;
            virtual std::int8_t  getInt8Value()  const override ;
            virtual std::int16_t getInt16Value() const override ;
            virtual std::int32_t getInt32Value() const override ;
            virtual std::int64_t getInt64Value() const override ;
            InternalType getValue() const ;

            virtual std::size_t getNumNodesRecursively() const override ;
            virtual std::vector<ASTNode*> getChildren() const override ;
            virtual std::string toString() const override ;
            virtual std::string toTreeString() const override ;

        protected:
            Type* type ;
            InternalType value ;
        } ;

        class Integer16Literal : public IntegerLiteral {
        public:
            typedef std::int_fast16_t InternalType ;

            Integer16Literal() = delete ;
            Integer16Literal(HorseIRParser::LiteralIntegerContext* cst, ASTNode::MemManagerType& mem) ;
            Integer16Literal(ASTNode::MemManagerType& mem) ;

            virtual horseIR::ast::Type* getLiteralType() const override ;
            virtual std::int8_t  getInt8Value()  const override ;
            virtual std::int16_t getInt16Value() const override ;
            virtual std::int32_t getInt32Value() const override ;
            virtual std::int64_t getInt64Value() const override ;
            InternalType getValue() const ;

            virtual std::size_t getNumNodesRecursively() const override ;
            virtual std::vector<ASTNode*> getChildren() const override ;
            virtual std::string toString() const override ;
            virtual std::string toTreeString() const override ;

        protected:
            Type* type ;
            InternalType value ;
        } ;

        class Integer32Literal : public IntegerLiteral {
        public:
            typedef std::int_fast32_t InternalType ;

            Integer32Literal() = delete ;
            Integer32Literal(HorseIRParser::LiteralIntegerContext* cst, ASTNode::MemManagerType& mem) ;
            Integer32Literal(ASTNode::MemManagerType& mem) ;

            virtual horseIR::ast::Type* getLiteralType() const override ;
            virtual std::int8_t  getInt8Value()  const override ;
            virtual std::int16_t getInt16Value() const override ;
            virtual std::int32_t getInt32Value() const override ;
            virtual std::int64_t getInt64Value() const override ;
            InternalType getValue() const ;

            virtual std::size_t getNumNodesRecursively() const override ;
            virtual std::vector<ASTNode*> getChildren() const override ;
            virtual std::string toString() const override ;
            virtual std::string toTreeString() const override ;

        protected:
            Type* type ;
            InternalType value ;
        } ;

        class Integer64Literal : public IntegerLiteral {
            typedef std::int_fast64_t InternalType ;

            Integer64Literal() = delete ;
            Integer64Literal(HorseIRParser::LiteralIntegerContext* cst, ASTNode::MemManagerType& mem) ;
            Integer64Literal(ASTNode::MemManagerType& mem) ;

            virtual std::int8_t  getInt8Value()  const override ;
            virtual std::int16_t getInt16Value() const override ;
            virtual std::int32_t getInt32Value() const override ;
            virtual std::int64_t getInt64Value() const override ;
            InternalType getValue() const ;

            virtual std::size_t getNumNodesRecursively() const override ;
            virtual std::vector<ASTNode*> getChildren() const override ;
            virtual std::string toString() const override ;
            virtual std::string toTreeString() const override ;

        protected:
            Type* type ;
            InternalType value ;
        } ;

        class FPLiteral : public Literal {
        public:
            enum class FPLiteralClass {
                FP32Literal, FP64Literal
            };

            FPLiteral() = delete ;
            FPLiteral(HorseIRParser::LiteralFloatContext* cst,
                      ASTNode::MemManagerType& mem,
                      ASTNode::ASTNodeType type,
                      FPLiteral::FPLiteralClass p_FPLiteralClass) ;
            FPLiteral(ASTNode::MemManagerType& mem,
                      ASTNode::ASTNodeType type,
                      FPLiteral::FPLiteralClass p_FPLiteralClass) ;

            virtual float  getFP32Value() const = 0 ;
            virtual double getFP64Value() const = 0 ;
            FPLiteralClass getFPLiteralClass() const ;

            static FPLiteral* makeFPLiteralASTNode(HorseIRParser::LiteralFloatContext* cst, ASTNode::MemManagerType& mem) ;

        protected:
            FPLiteral::FPLiteralClass fpLiteralClass ;
        };

        class FP32Literal : public FPLiteral {
        public:
            typedef float InternalType ;

            FP32Literal() = delete ;
            FP32Literal(HorseIRParser::LiteralFloatContext* cst, ASTNode::MemManagerType& mem) ;
            FP32Literal(ASTNode::MemManagerType& mem) ;

            virtual horseIR::ast::Type* getLiteralType() const override ;
            virtual float  getFP32Value() const override ;
            virtual double getFP64Value() const override ;
            IntegerLiteral getValue() const ;

            virtual std::size_t getNumNodesRecursively() const override ;
            virtual std::vector<ASTNode*> getChildren() const override ;
            virtual std::string toString() const override ;
            virtual std::string toTreeString() const override ;

        protected:
            Type* type ;
            InternalType value ;
        };

        class FP64Literal : public FPLiteral {
        public:
            typedef double InternalType ;

            FP64Literal() = delete ;
            FP64Literal(HorseIRParser::LiteralFloatContext* cst, ASTNode::MemManagerType& mem) ;
            FP64Literal(ASTNode::MemManagerType& mem) ;

            virtual float  getFP32Value() const override ;
            virtual double getFP64Value() const override ;
            IntegerLiteral getValue() const ;

            virtual std::size_t getNumNodesRecursively() const override ;
            virtual std::vector<ASTNode*> getChildren() const override ;
            virtual std::string toString() const override ;
            virtual std::string toTreeString() const override ;

        protected:
            Type* type ;
            InternalType value ;
        };

        class SymbolLiteral : public Literal {
        public:
            typedef std::string InternalType ;

            SymbolLiteral() = delete ;
            SymbolLiteral(HorseIRParser::LiteralSymbolContext* cst, ASTNode::MemManagerType& mem) ;
            SymbolLiteral(ASTNode::MemManagerType& mem) ;

            InternalType getValue() const ;

            virtual std::size_t getNumNodesRecursively() const override ;
            virtual std::vector<ASTNode*> getChildren() const override ;
            virtual std::string toString() const override ;
            virtual std::string toTreeString() const override ;

        protected:
            Type* type ;
            InternalType value ;
        };

        class TimeLiteral : public Literal {
        public:
            typedef std::int_fast32_t YearType ;
            typedef std::uint_fast8_t MonthType ;
            typedef std::uint_fast8_t DayType ;
            typedef std::uint_fast8_t HourType ;
            typedef std::uint_fast8_t MinuteType ;
            typedef std::uint_fast8_t SecondType ;
            typedef std::uint_fast8_t MicrosecondType ;

            enum class TimeLiteralClass {
                TimeMonthLiteral, TimeDateLiteral, TimeDateTimeLiteral, TimeMinuteLiteral,
                TimeSecondLiteral, TimeTimeLiteral
            };

            TimeLiteral() = delete ;
            TimeLiteral(HorseIRParser::LiteralTimeContext* cst,
                        ASTNode::MemManagerType& mem,
                        ASTNode::ASTNodeType type,
                        TimeLiteral::TimeLiteralClass p_TimeLiteralClass) ;
            TimeLiteral(ASTNode::MemManagerType& mem,
                        ASTNode::ASTNodeType type,
                        TimeLiteral::TimeLiteralClass p_TimeLiteralClass) ;

            TimeLiteralClass getTimeLiteralClass() const ;

            static TimeLiteral* makeTimeLiteralASTNode(HorseIRParser::LiteralTimeContext* cst, ASTNode::MemManagerType& mem) ;

        protected:
            TimeLiteral::TimeLiteralClass timeLiteralClass ;
        };

        class TimeMonthLiteral : public TimeLiteral {
        public:
            typedef std::tuple<TimeLiteral::YearType, TimeLiteral::MonthType> InternalType ;

            TimeMonthLiteral() = delete ;
            TimeMonthLiteral(HorseIRParser::LiteralTimeMonthContext* cst, ASTNode::MemManagerType& mem) ;
            TimeMonthLiteral(ASTNode::MemManagerType& mem) ;

            virtual horseIR::ast::Type* getLiteralType() const override ;
            InternalType getValue() const ;
            TimeLiteral::YearType getYearValue() const ;
            TimeLiteral::MonthType getMonthValue() const ;

            virtual std::size_t getNumNodesRecursively() const override ;
            virtual std::vector<ASTNode*> getChildren() const override ;
            virtual std::string toString() const override ;
            virtual std::string toTreeString() const override ;

        protected:
            Type* type ;
            InternalType value ;
        };

        class TimeDateLiteral : public TimeLiteral {
        public:
            typedef std::tuple<TimeLiteral::YearType, TimeLiteral::MonthType, TimeLiteral::DayType> InternalType ;

            TimeDateLiteral() = delete ;
            TimeDateLiteral(HorseIRParser::LiteralTimeDateContext* cst, ASTNode::MemManagerType& mem) ;
            TimeDateLiteral(ASTNode::MemManagerType& mem) ;

            virtual horseIR::ast::Type* getLiteralType() const override ;
            InternalType getValue() const ;
            TimeLiteral::YearType getYearValue() const ;
            TimeLiteral::MonthType getMonthValue() const ;
            TimeLiteral::DayType  getDayValue() const ;

            virtual std::size_t getNumNodesRecursively() const override ;
            virtual std::vector<ASTNode*> getChildren() const override ;
            virtual std::string toString() const override ;
            virtual std::string toTreeString() const override ;

        protected:
            Type* type ;
            InternalType value ;
        };

        class TimeDateTimeLiteral : public TimeLiteral {
        public:
            typedef std::tuple<
                        TimeLiteral::YearType,
                        TimeLiteral::MonthType,
                        TimeLiteral::DayType,
                        TimeLiteral::HourType,
                        TimeLiteral::MinuteType,
                        TimeLiteral::SecondType,
                        TimeLiteral::MicrosecondType
                    > InternalType ;

            TimeDateTimeLiteral() = delete ;
            TimeDateTimeLiteral(HorseIRParser::LiteralTimeDateContext* cst, ASTNode::MemManagerType& mem) ;
            TimeDateTimeLiteral(ASTNode::MemManagerType& mem) ;

            virtual horseIR::ast::Type* getLiteralType() const override ;
            InternalType getValue() const ;
            TimeLiteral::YearType getYearValue() const ;
            TimeLiteral::MonthType getMonthValue() const ;
            TimeLiteral::DayType  getDayValue() const ;
            TimeLiteral::HourType  getHourValue() const ;
            TimeLiteral::MinuteType getMinuteValue() const ;
            TimeLiteral::SecondType getSecondValue() const ;
            TimeLiteral::MicrosecondType getMicrosecondType() const ;

            virtual std::size_t getNumNodesRecursively() const override ;
            virtual std::vector<ASTNode*> getChildren() const override ;
            virtual std::string toString() const override ;
            virtual std::string toTreeString() const override ;

        protected:
            Type* type ;
            InternalType value ;
        };

        class TimeMinuteLiteral : public TimeLiteral {
        public:
            typedef std::tuple<TimeLiteral::HourType, TimeLiteral::MinuteType> InternalType ;

            TimeMinuteLiteral() = delete ;
            TimeMinuteLiteral(HorseIRParser::LiteralTimeMinuteContext* cst, ASTNode::MemManagerType& mem) ;
            TimeMinuteLiteral(ASTNode::MemManagerType& mem) ;

            virtual horseIR::ast::Type* getLiteralType() const override ;
            InternalType getValue() const ;
            TimeLiteral::HourType getHourValue() const ;
            TimeLiteral::MinuteType getMinuteType() const ;

            virtual std::size_t getNumNodesRecursively() const override ;
            virtual std::vector<ASTNode*> getChildren() const override ;
            virtual std::string toString() const override ;
            virtual std::string toTreeString() const override ;

        protected:
            Type* type ;
            InternalType value ;
        };

        class TimeSecondLiteral : public TimeLiteral {
        public:
            typedef std::tuple<TimeLiteral::HourType, TimeLiteral::MinuteType, TimeLiteral::SecondType> InternalType ;

            TimeSecondLiteral() = delete ;
            TimeSecondLiteral(HorseIRParser::LiteralTimeSecondContext* cst, ASTNode::MemManagerType& mem) ;
            TimeSecondLiteral(ASTNode::MemManagerType& mem) ;

            virtual horseIR::ast::Type* getLiteralType() const override ;
            InternalType getValue() const ;
            TimeLiteral::HourType getHourValue() const ;
            TimeLiteral::MinuteType getMinuteType() const ;
            TimeLiteral::SecondType getSecondType() const ;

            virtual std::size_t getNumNodesRecursively() const override ;
            virtual std::vector<ASTNode*> getChildren() const override ;
            virtual std::string toString() const override ;
            virtual std::string toTreeString() const override ;

        protected:
            Type* type ;
            InternalType value ;
        };

        class TimeTimeLiteral : public TimeLiteral {
        public:
            typedef std::tuple<
                        TimeLiteral::HourType ,
                        TimeLiteral::MinuteType ,
                        TimeLiteral::SecondType ,
                        TimeLiteral::MicrosecondType
                    > InternalType ;

            TimeTimeLiteral() = delete ;
            TimeTimeLiteral(HorseIRParser::LiteralTimeTimeContext* cst, ASTNode::MemManagerType& mem) ;
            TimeTimeLiteral(ASTNode::MemManagerType& mem) ;

            virtual horseIR::ast::Type* getLiteralType() const override ;
            InternalType getValue() const ;
            TimeLiteral::HourType getHourValue() const ;
            TimeLiteral::MinuteType getMinuteValue() const ;
            TimeLiteral::SecondType getSecondValue() const ;
            TimeLiteral::MicrosecondType getMicrosecondValue() const ;

            virtual std::size_t getNumNodesRecursively() const override ;
            virtual std::vector<ASTNode*> getChildren() const override ;
            virtual std::string toString() const override ;
            virtual std::string toTreeString() const override ;

        protected:
            Type* type ;
            InternalType value ;
        };

        class FunctionLiteral : public Literal {
        public:
            typedef std::string InternalType ;

            FunctionLiteral() = delete ;
            FunctionLiteral(HorseIRParser::LiteralFunctionContext* cst, ASTNode::MemManagerType& mem) ;
            FunctionLiteral(ASTNode::MemManagerType& mem) ;

            virtual horseIR::ast::Type* getLiteralType() const override ;
            InternalType getValue() const ;

            virtual std::size_t getNumNodesRecursively() const override ;
            virtual std::vector<ASTNode*> getChildren() const override ;
            virtual std::string toString() const override ;
            virtual std::string toTreeString() const override ;

        protected:
            Type* type ;
            InternalType value ;
        };

        class TableLiteral : public Literal {
        public:
            typedef std::string InternalType ;

            TableLiteral() = delete ;
            TableLiteral(HorseIRParser::LiteralTableContext* cst, ASTNode::MemManagerType& mem) ;
            TableLiteral(ASTNode::MemManagerType& mem) ;

            virtual horseIR::ast::Type* getLiteralType() const override ;
            InternalType getValue() const ;

            virtual std::size_t getNumNodesRecursively() const override ;
            virtual std::vector<ASTNode*> getChildren() const override ;
            virtual std::string toString() const override ;
            virtual std::string toTreeString() const override ;

        protected:
            Type* type ;
            InternalType value ;
        };

        class KeyTableLiteral : public Literal {
        public:
            typedef std::string InternalType ;

            KeyTableLiteral() = delete ;
            KeyTableLiteral(HorseIRParser::LiteralKtableContext* cst, ASTNode::MemManagerType& mem) ;
            KeyTableLiteral(ASTNode::MemManagerType& mem) ;

            virtual horseIR::ast::Type* getLiteralType() const override ;
            InternalType getValue() const ;

            virtual std::size_t getNumNodesRecursively() const override ;
            virtual std::vector<ASTNode*> getChildren() const override ;
            virtual std::string toString() const override ;
            virtual std::string toTreeString() const override ;

        protected:
            Type* type ;
            InternalType value ;
        };

        class StringLiteral : public Literal {
        public:
            typedef std::string InternalType ;

            StringLiteral() = delete ;
            StringLiteral(HorseIRParser::LiteralStringContext* cst, ASTNode::MemManagerType& mem) ;
            StringLiteral(ASTNode::MemManagerType& mem) ;

            virtual horseIR::ast::Type* getLiteralType() const override ;
            InternalType getValue() const ;

            virtual std::size_t getNumNodesRecursively() const override ;
            virtual std::vector<ASTNode*> getChildren() const override ;
            virtual std::string toString() const override ;
            virtual std::string toTreeString() const override ;
        };
    }
}
