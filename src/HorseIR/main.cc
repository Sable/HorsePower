#include <stdio.h>
#include <unistd.h>
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

bool flagVersion = false;
bool flagUsage   = false;
char *inputPath  = NULL;

void usage();
int getOption(int argc, char *argv[]);

int main(int argc, char *argv[]){
    int r = getOption(argc, argv);
    if(r){
        std::cout<<"Error: check ./horse -h" <<std::endl;
        exit(1);
    }
    else if(flagUsage){
        usage();
    }
    else if(flagVersion){
        printf("HorseIR version: 0.1\n");
    }
    else if(inputPath!=NULL){
        antlr4::ANTLRFileStream fileStream(inputPath);
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
    }
    else {
        std::cout<<"Error: check ./horse -h" <<std::endl;
        exit(1);
    }
    return 0;
}

int getOption(int argc, char *argv[]){
    int c;
    while((c = getopt(argc, argv, "hvp:")) != -1){
        switch(c){
            case 'h': flagUsage   = true; break;
            case 'v': flagVersion = true; break;
            case 'p': inputPath = optarg; break;
            default : return 1;
        }
    }
    return 0;
}

void usage(){
    std::cout << "Usage: ./horse <option> [parameter]  " << std::endl;
    std::cout << "  -h           Print this information" << std::endl;
    std::cout << "  -p <path>    Set an input file"      << std::endl;
    std::cout << "  -v           Print HorseIR version"  << std::endl;
}
