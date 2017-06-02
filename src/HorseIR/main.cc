#include <stdio.h>
#include <stdexcept>
#include <ostream>
#include <sstream>
#include <iostream>

#include "antlr4-runtime.h"
#include "grammar/HorseIRLexer.h"
#include "grammar/HorseIRParser.h"
#include "grammar/HorseIRBaseVisitor.h" 

#include "util/CSTPrettyPrinter.h"

using antlr4::tree::ParseTree;
using antlr4::tree::ParseTreeWalker;

/*
 * https://github.com/antlr/antlr4/blob/master/doc/cpp-target.md
 */

int main(int argc, char *argv[]){
    if(argc != 2){
        printf("Usage: ./horse <path>\n");
        exit(1);
    }

    antlr4::ANTLRFileStream fileStream(argv[1]);
    HorseIRLexer lexer( &fileStream ) ; 
    antlr4::CommonTokenStream tokenStream( &lexer ) ;
    HorseIRParser parser( &tokenStream ) ;

    try{
        printf("entering\n");
        antlr4::tree::ParseTree* tree =  parser.program();
        std::ostream outStream(std::cout.rdbuf());
        horseIR::util::CSTPrettyPrinter printer(outStream);
        printer.prettyPrint(tree);
        //printer.visit(tree);
    }
    catch (antlr4::ParseCancellationException &exception){
        std::cerr<<"Exception captured"<<std::endl;
        return 1;
    }
    printf("successfully done.\n");
    return 0;
}

