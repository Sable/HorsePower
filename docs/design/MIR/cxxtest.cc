#include <stdexcept>
#include <ostream>
#include <sstream>
#include <cxxtest/TestSuite.h>

#include "antlr4-runtime.h"
#include "MIRLexer.h"
#include "MIRParser.h"

#include "PrettyPrinter.h"

using antlr4::tree::ParseTree ;

class LexerParserSuite : public CxxTest::TestSuite {
public:
    virtual void setUp() override {
        errorFlag = false ;
    }
    
    virtual void tearDown() override {
        if (errorFlag) std::cerr << errorMessage << std::endl ;
    }
    
    void testFibonacci (void) {
        TS_ASSERT( testParse( "./valid/fibonacci.mir" ) ) ;
    }

    void testGlobalVariable (void) {
        TS_ASSERT( testParse( "./valid/globalVariable.mir" ) ) ;
    }

    void testSQL1 (void) {
        TS_ASSERT( testParse( "./valid/sql1.mir" ) ) ;
    }

    void testPhi (void) {
        TS_ASSERT( testParse( "./valid/phi.mir" ) ) ;
    }
protected:
    bool errorFlag ;
    std::string errorMessage ;
    class LexerParserErrorHandler : public antlr4::BaseErrorListener {
    public:
        LexerParserErrorHandler() = delete ;
        LexerParserErrorHandler(const std::string& path, std::ostream& strm)
            : filepath( path ),
              BaseErrorListener()
            {}
        
        virtual void syntaxError(antlr4::Recognizer *recognizer,
                                 antlr4::Token * offendingSymbol, size_t line,
                                 size_t charPositionInLine,
                                 const std::string &msg,
                                 std::exception_ptr e) override {
            std::ostringstream stringBuilder ;
            stringBuilder << "In file "
                          << filepath
                          << " at ["
                          << std::setw( 3 ) << line
                          << ", "
                          << std::setw( 3 ) << charPositionInLine
                          << "], "
                          << msg
                          << std::endl ;
            throw antlr4::ParseCancellationException( stringBuilder.str() ) ;
        }
    private:
        const std::string filepath ;
    };
    
    bool testParse(const std::string& path) noexcept {
        antlr4::ANTLRFileStream fileStream( path ) ;
        MIRLexer lexer( &fileStream ) ;
        antlr4::CommonTokenStream tokenStream( &lexer ) ;
        MIRParser parser( &tokenStream ) ;

        LexerParserErrorHandler errorHandler( path, std::cout );
        lexer.removeErrorListeners() ;
        parser.removeErrorListeners() ;
        lexer.addErrorListener( &errorHandler ) ;
        parser.addErrorListener( &errorHandler ) ;

        try {
            antlr4::tree::ParseTree* ptr =  parser.program() ;

            (void) ptr;
            return true ;
        } catch (antlr4::ParseCancellationException &exception) {
            errorFlag = true ;
            errorMessage = exception.what() ;
            return false ;
        }
    }
};
