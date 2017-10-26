#include <stdio.h>
#include <unistd.h>
#include <stdexcept>
#include <ostream>
#include <sstream>
#include <iostream>

// #include "antlr4-runtime.h"

// using antlr4::tree::ParseTree;
// using antlr4::tree::ParseTreeWalker;

#include "backend/h_global.h"

/*
 * https://github.com/antlr/antlr4/blob/master/doc/cpp-target.md
 */

bool flagVersion = false;
bool flagUsage   = false;
bool flagQuery   = false;
bool flagOpt     = false;
bool flagPL      = false;
int  queryId     = -1;
int  queryScale  = 1;
int  plId        = -1;
char delimiter   = '|';

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
    else if(flagQuery){
        printf("...Testing...\n");
        // testInputFile(inputPath);
        testMain(0, queryId, queryScale, flagOpt, delimiter);
    }
    else if(flagPL){
        testMain(1, plId, -1, flagOpt, delimiter);
    }
    else {
        usage();
        exit(1);
    }
    return 0;
}

int getOption(int argc, char *argv[]){
    int c;
    while((c = getopt(argc, argv, "hvop:q:s:d:")) != -1){
        switch(c){
            case 'h': flagUsage   = true; break;
            case 'v': flagVersion = true; break;
            case 'p': flagPL      = true; \
                      plId        = atoi(optarg); break;
            case 'q': flagQuery   = true; \
                      queryId     = atoi(optarg); break;
            case 's': queryScale  = atoi(optarg); break;
            case 'o': flagOpt     = true;         break;
            case 'd': delimiter   = optarg[0];    break;
            default : return 1;
        }
    }
    return 0;
}

/*
 * For example,
 *   ./horse -q 1 -s 1 -o
 *
 * -> TPC-H query 1, scale 1, and optimizations enabled
 */
void usage(){
    std::cout << "Usage: ./horse <option> [parameter]  " << std::endl;
    std::cout << "  -h           Print this information" << std::endl;
    std::cout << "  -p <pid>     Set PL test id"         << std::endl;
    std::cout << "  -v           Print HorseIR version"  << std::endl;
    std::cout << "  -q <qid>     TPC-H query id"         << std::endl;
    std::cout << "  -s <sid>     TPC-H query scale"      << std::endl;
    std::cout << "  -o           TPC-H query opt on/off" << std::endl;
    std::cout << "  -d 'del'     Set delimiter"          << std::endl;
}

