#include "antlr4-runtime.h"

#include "./ast/AST.h"
#include "./ast/ASTVisitor.h"

#include "./misc/Collections.h"
#include "./interpreter/ExternalMethod.h"

const char* rawProgram = ""
    "/*\n"
    " * varchar(99) -> sym\n"
    " * int         -> i64\n"
    " */\n"
    "module default {\n"
    "    import Builtin.*;\n"
    "    def find_valid_index(colVal :list<i64>, indexBool :list<i64>) : list<i64> {\n"
    "        colSize   :? =  @len(colVal);\n"
    "        validBool :? = @lt(indexBool,colSize);\n"
    "        indexSize :? = @len(indexBool);\n"
    "        indexRange:? = @range(indexSize);\n"
    "        validIndex:? = @reduce(validBool, indexRange);\n"
    "        return validIndex; \n"
    "    }\n"
    "    def find_valid_item(colVal :list<i64>, indexBool :list<i64>)  : list<i64> {\n"
    "        colSize   :? = @len(colVal);\n"
    "        validBool :? = @lt(indexBool,colSize);\n"
    "        validItem :? = @reduce(validBool, indexBool);\n"
    "        return validItem;\n"
    "    }\n"
    "    def main() : table {\n"
    "        s0:list<sym> = check_cast(@column_value(`Employee,   `LastName)      , list<sym>);\n"
    "        s1:list<i64> = check_cast(@column_value(`Employee,   `DepartmentID)  , list<i64>);\n"
    "        s2:list<i64> = check_cast(@column_value(`Department, `DepartmentID)  , list<i64>);\n"
    "        s3:list<sym> = check_cast(@column_value(`Department, `DepartmentName), list<sym>);\n"
    "\n"
    "        t0:list<i64> = @index_of       (s2,s1);\n"
    "        t1:list<i64> = find_valid_index(s2,t0);\n"
    "        t2:list<i64> = find_valid_item (s2,t0);\n"
    "\n"
    "        r0:list<sym> = @index(s0,t1);\n"
    "        r1:list<i64> = @index(s1,t1);\n"
    "        r2:list<sym> = @index(s3,t2);\n"
    "\n"
    "        d0:dict<sym,sym> = @dict(`LastName      , r0);\n"
    "        d1:dict<sym,i64> = @dict(`DepartmentID  , r1);\n"
    "        d2:dict<sym,sym> = @dict(`DepartmentName, r2);\n"
    "\n"
    "        z0:?    = @list(d0,d1,d2);\n"
    "        z:table = @table(z0);\n"
    "        return z;\n"
    "    }\n"
    "    \n"
    "}"
    "" ;

const char* rawType = "(func<?, i32, ...:?>, func<?, ?, ... :bool> ,   dict<i8, str>) -> bool" ;

class StructurePrinter : horseIR::ast::ASTVisitor<void> {
public:
    void visit(horseIR::ast::ASTNode* ast) override {
        std::cout << typeid(*ast).name()  << std::endl ;
        ASTVisitor<void>::visit(ast) ;
    }
} ;

#include "./misc/Hasher.h"

int main(int argc, char *argv[])
{
    antlr4::ANTLRInputStream inStream(rawProgram) ;
    horseIR::HorseIRLexer lexer(&inStream) ;
    antlr4::CommonTokenStream tokenStream(&lexer) ;
    horseIR::HorseIRParser parser(&tokenStream) ;
    auto compilationUnitContext = parser.program() ;
    horseIR::ast::ASTNode::MemManagerType mem ;
    auto compilationUnit = new horseIR::ast::CompilationUnit(compilationUnitContext, mem) ;
    std::cout << compilationUnit->toString() << std::endl ;

    antlr4::ANTLRInputStream typeInStream(rawType) ;
    horseIR::HorseIRLexer typeLexer(&typeInStream) ;
    antlr4::CommonTokenStream typeTokenStream(&typeLexer) ;
    horseIR::HorseIRParser typeParser(&typeTokenStream) ;
    auto typeList = typeParser.typeSignatureList() ;

    auto signatureASTNodes = horseIR::ast::Type::makeTypeSignatureASTNodes(typeList, mem) ;
    auto signatureString = horseIR::misc::Collections::applyAndCollect(
        signatureASTNodes,
        [](horseIR::ast::Type* type) -> std::string {
            return type->toString() ;
        }) ;
    horseIR::misc::Collections::writeToStream(std::cout, signatureString, ", ", "\n") ;

    using namespace horseIR::interpreter ;
    ExternalMethod<void*,
                   std::uint16_t,
                   horseIR::misc::HasherDJB2<std::string, std::uint16_t>> method("package", "method", rawType) ;
    std::cout << method.toString() << std::endl ;

    std::cout << std::boolalpha
              << signatureASTNodes[0]->isGeneralizationOf(signatureASTNodes[1]) << std::endl 
              << signatureASTNodes[1]->isGeneralizationOf(signatureASTNodes[0]) << std::endl ;

    try {
        horseIR::ast::Type* type = horseIR::ast::Type::specificityJoin(
            signatureASTNodes[0],
            signatureASTNodes[1],
            mem) ;
        std::cout << type->toString() << std::endl ;
    } catch(const horseIR::ast::Type::SpecificityJoinAbortException& exception) {
        std::cout << exception.what() << std::endl
                  << exception.getLHSSite()->toString() << std::endl
                  << exception.getRHSSite()->toString() << std::endl ;
    }
    return 0;
}
