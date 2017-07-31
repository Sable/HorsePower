#include "antlr4-runtime.h"

#include "./ast/AST.h"
#include "./ast/ASTVisitor.h"

#include "./misc/Collections.h"
#include "./interpreter/ExternalMethod.h"

const char* rawProgram = ""
    "/*                                                                                        \n"
    " * varchar(99) -> sym                                                                     \n"
    " * int         -> i64                                                                     \n"
    " */                                                                                       \n"
    "module default {                                                                          \n"
    "    import Builtin.*;                                                                     \n"
    "    def find_valid_index(colVal :list<i64>, indexBool :list<i64>) : list<i64> {           \n"
    "        colSize   :? =  @len(colVal);                                                     \n"
    "        validBool :? = @lt(indexBool,colSize);                                            \n"
    "        indexSize :? = @len(indexBool);                                                   \n"
    "        indexRange:? = @range(indexSize);                                                 \n"
    "        validIndex:? = @reduce(validBool, indexRange);                                    \n"
    "        return validIndex;                                                                \n"
    "    }                                                                                     \n"
    "    def find_valid_item(colVal :list<i64>, indexBool :list<i64>)  : list<i64> {           \n"
    "        colSize   :? = @len(colVal);                                                      \n"
    "        validBool :? = @lt(indexBool,colSize);                                            \n"
    "        validItem :? = @reduce(validBool, indexBool);                                     \n"
    "        return validItem;                                                                 \n"
    "    }                                                                                     \n"
    "    def main() : table {                                                                  \n"
    "        s0:list<sym> = check_cast(@column_value(`Employee,   `LastName)      , list<sym>);\n"
    "        s1:list<i64> = check_cast(@column_value(`Employee,   `DepartmentID)  , list<i64>);\n"
    "        s2:list<i64> = check_cast(@column_value(`Department, `DepartmentID)  , list<i64>);\n"
    "        s3:list<sym> = check_cast(@column_value(`Department, `DepartmentName), list<sym>);\n"
    "                                                                                          \n"
    "        t0:list<i64> = @index_of       (s2,s1);                                           \n"
    "        t1:list<i64> = find_valid_index(s2,t0);                                           \n"
    "        t2:list<i64> = find_valid_item (s2,t0);                                           \n"
    "                                                                                          \n"
    "        r0:list<sym> = @index(s0,t1);                                                     \n"
    "        r1:list<i64> = @index(s1,t1);                                                     \n"
    "        r2:list<sym> = @index(s3,t2);                                                     \n"
    "                                                                                          \n"
    "        d0:dict<sym,sym> = @dict(`LastName      , r0);                                    \n"
    "        d1:dict<sym,i64> = @dict(`DepartmentID  , r1);                                    \n"
    "        d2:dict<sym,sym> = @dict(`DepartmentName, r2);                                    \n"
    "                                                                                          \n"
    "        z0:?    = @list(d0,d1,d2);                                                        \n"
    "        z:table = @table(z0);                                                             \n"
    "        return z;                                                                         \n"
    "    }                                                                                     \n"
    "                                                                                          \n"
    "}                                                                                         \n"
    "" ;

const char* rawType = "func<func<?, i32, ...:?>, func<?, ?, ... :bool> ,   dict<i8, str> :bool>" ;

class StructurePrinter : horseIR::ast::ASTVisitor<void> {
public:
    void visit(horseIR::ast::ASTNode* ast) override {
        std::cout << typeid(*ast).name()  << std::endl ;
        ASTVisitor<void>::visit(ast) ;
    }
} ;

#include "./misc/Hasher.h"
#include "./interpreter/Dispatcher.h"
#include "./interpreter/Exception.h"

#include <memory>

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

    using namespace horseIR::interpreter ;
    std::unique_ptr<ExternalMethod<void*>> bindedMethod (
        ExternalMethod<void*>::bindExternalMethod(
            "MyModule", "foo", rawType, [] (std::size_t argc, void* argv[]) -> void* {
                std::cout << "Method Invoked !" << std::endl ;
                return nullptr ;
            })) ;
    std::cout << bindedMethod->toString() << std::endl ;
    const auto modules = compilationUnit->getModules() ;
    
    std::vector<InternalMethod<void*>*> internalMethod ;
    for (auto iter = modules.cbegin(); iter != modules.cend(); ++iter) {
        const auto methods = (*iter)->getMethods() ;
        const std::string moduleName = (*iter)->getModuleName() ;
        for (auto methodIter = methods.cbegin(); methodIter != methods.cend(); ++methodIter) {
            InternalMethod<void*>* ptr = new InternalMethod<void*>(*methodIter) ;
            internalMethod.emplace_back(ptr) ;
        }
    }

    VectorDispatcher<void*>::ContainerType c ;
    try{
    for (auto iter = internalMethod.cbegin(); iter != internalMethod.cend(); ++iter) {
        VectorDispatcher<void*>::manage(c, *iter); 
    }
    VectorDispatcher<void*>::manage(c, ExternalMethod<void*>::bindExternalMethod(
                                        "default", "main", "func<?, i64 :table>",
                                        [](std::size_t argc, void* argv[]) -> void* {
                                            return nullptr ;
                                        })) ;
    VectorDispatcher<void*>::manage(c, ExternalMethod<void*>::bindExternalMethod(
                                        "default", "main", "func<i32, i32, ... :table>",
                                        [](std::size_t argc, void* argv[]) -> void* {
                                            return nullptr ;
                                        })) ;
    VectorDispatcher<void*>::manage(c, ExternalMethod<void*>::bindExternalMethod(
                                        "default", "main", "func<?, i32 :ktable>",
                                        [](std::size_t argc, void* argv[]) -> void* {
                                            return nullptr ;
                                        })) ;
    VectorDispatcher<void*>::manage(c, ExternalMethod<void*>::bindExternalMethod(
                                        "default", "main", "func<?, ? :bool>",
                                        [](std::size_t argc, void* argv[]) -> void* {
                                            return nullptr ;
                                        })) ;
    std::cout << VectorDispatcher<void*>::containerToString(c) << std::endl ;
    } catch (const VectorDispatcher<void*>::OverloadOverlapException & except) {
        std::cout << except.toString() << std::endl ;
    } catch (const VectorDispatcher<void*>::OverloadDuplicateException& except) {
        std::cout << except.toString() << std::endl ;
    } catch (const VectorDispatcher<void*>::InvalidSignatureStringException& except) {
        std::cout << except.toString() << std::endl ;
    }

    auto ptr = VectorDispatcher<void*>::fetch(c, "default", "main", "i32, ?") ;
    if (ptr == nullptr) {
        std::cout << "no viable overload found" << std::endl ;
    } else {
        std::cout << ptr->toString() << std::endl ;
    }
    return 0;
}
