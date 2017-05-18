#include <stdexcept>
#include <ostream>
#include <cxxtest/TestSuite.h>

#include "antlr4-runtime.h"
#include "MIRLexer.h"
#include "MIRParser.h"

using antlr4::tree::ParseTree ;

class LexerParserSuite : public CxxTest::TestSuite {
public:
    void testFibonacci (void) {
        TS_ASSERT( testParse( "./valid/fibonacci.mir" ) ) ;
    }

    void testGlobalVariable (void) {
        TS_ASSERT( testParse( "./valid/globalVariable.mir" ) ) ;
    }
protected:
    class LexerParserErrorHandler : public antlr4::BaseErrorListener {
    public:
        LexerParserErrorHandler() = delete ;
        LexerParserErrorHandler(const std::string& path, std::ostream& strm)
            : filepath( path ),
              errorPrintStream( strm ),
              BaseErrorListener()
            {}
        
        virtual void syntaxError(antlr4::Recognizer *recognizer,
                                 antlr4::Token * offendingSymbol, size_t line,
                                 size_t charPositionInLine,
                                 const std::string &msg,
                                 std::exception_ptr e) override {
            errorPrintStream << "In file "
                             << filepath
                             << " at ["
                             << std::setw( 3 ) << line
                             << ", "
                             << std::setw( 3 ) << charPositionInLine
                             << "], "
                             << msg
                             << std::endl ;
            throw antlr4::ParseCancellationException() ;
        }
    private:
        const std::string filepath ;
        std::ostream& errorPrintStream ;
    };
    
    bool testParse(const std::string& path) noexcept {
        antlr4::ANTLRFileStream fileStream( path ) ;
        MIRLexer lexer( &fileStream ) ;
        antlr4::CommonTokenStream tokenStream( &lexer ) ;
        MIRParser parser( &tokenStream ) ;

        LexerParserErrorHandler errorHandler( path, std::cerr );
        lexer.removeErrorListeners() ;
        parser.removeErrorListeners() ;
        lexer.addErrorListener( &errorHandler ) ;
        parser.addErrorListener( &errorHandler ) ;

        try {
            (void) parser.program() ;
            return true ;
        } catch (antlr4::ParseCancellationException &exception) {
            return false ;
        }
    }
};
