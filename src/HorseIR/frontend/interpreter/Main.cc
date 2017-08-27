#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <stdexcept>
#include <boost/program_options.hpp>
#include <boost/algorithm/string.hpp>

#include "../ast/AST.h"
#include "../ast/CSTConverter.h"
#include "Interpreter.h"

#include "h_global.h"
#include "h_primitive.h"

#define HORSE_IR_INTERPRETER_VERSION "dev"

using namespace std;
using namespace boost;
using namespace horseIR::ast;
using namespace horseIR::interpreter;

class BadTableTypeArgument : public runtime_error {
 public:
  BadTableTypeArgument () : runtime_error ("bad table type argument")
  {};
};

class ErrorListener : public antlr4::BaseErrorListener {
 public:
  void syntaxError (antlr4::Recognizer *recognizer,
                    antlr4::Token *offendingSymbol,
                    std::size_t line, std::size_t charPositionInLine,
                    const std::string &msg, std::exception_ptr e) override
  {
    cerr << '[' << line << ", " << charPositionInLine << ']'
         << msg << endl;
    exit (EXIT_FAILURE);
  }
};

vector<L> checkTableType (const string &rawTableType)
{
  vector<L> tableTypes{};
  transform (
      rawTableType.cbegin (), rawTableType.cend (), back_inserter (tableTypes),
      [] (const char &character) -> L
      {
        switch (character)
          {
            case 'B': return H_B;
            case 'H': return H_H;
            case 'I': return H_I;
            case 'L': return H_L;
            case 'F': return H_F;
            case 'E': return H_E;
            case 'C': return H_C;
            case 'Q': return H_Q;
            case 'S': return H_S;
            case 'M': return H_M;
            case 'D': return H_D;
            case 'Z': return H_Z;
            case 'U': return H_U;
            case 'W': return H_W;
            case 'T': return H_T;
            case 'X': return H_X;
            default: throw BadTableTypeArgument ();
          }
      });
  return tableTypes;
}

vector<Q> checkTableHeader (const string &rawTableHeader)
{
  istringstream stream (rawTableHeader);
  vector<Q> tableHeaders;
  std::string buffer;
  while (getline (stream, buffer, ','))
    {
      trim (buffer);
      insertSym (createSymbol ((S) buffer.c_str ()));
      tableHeaders.push_back (getSymbol ((S) buffer.c_str ()));
    }
  return tableHeaders;
}

int main (int argc, const char *argv[])
{
  string entryModule;
  string entryMethod;

  program_options::options_description des ("Generic options");
  des.add_options ()
      ("version,v", "print version information")
      ("help,h", "print help message")
      ("table-path",
       program_options::value<vector<string>> (),
       "load CSV table at a given path")
      ("table-name",
       program_options::value<vector<string>> (),
       "specify the name of a loaded table")
      ("table-type",
       program_options::value<vector<string>> (),
       "specify the types for each column in a loaded table")
      ("table-header",
       program_options::value<vector<string>> (),
       "specify the headers for each column via a comma separated list")
      ("entry-module",
       program_options::value<string> (&entryModule)->default_value ("default"),
       "specify the entry module for the HorseIR program")
      ("entry-method",
       program_options::value<string> (&entryMethod)->default_value ("main"),
       "specify the entry method for the HorseIR program");

  program_options::variables_map optionMap;
  program_options::store (
      program_options::parse_command_line (argc, argv, des), optionMap
  );
  program_options::notify (optionMap);
  if (optionMap.count ("help") != 0)
    {
      cout << des << endl;
      return EXIT_SUCCESS;
    }
  if (optionMap.count ("version") != 0)
    {
      cout << "HorseIRInterpreter-" HORSE_IR_INTERPRETER_VERSION << endl;
      return EXIT_SUCCESS;
    }

  initMain ();
  initSym ();
  initSys ();

  if (optionMap.count ("table-path") != 0 ||
      optionMap.count ("table-type") != 0 ||
      optionMap.count ("table-header") != 0 ||
      optionMap.count ("table-name") != 0)
    {
      if (optionMap.count ("table-path") != optionMap.count ("table-type") ||
          optionMap.count ("table-type") != optionMap.count ("table-header") ||
          optionMap.count ("table-header") != optionMap.count ("table-name"))
        {
          cerr << "Invalid load table operation. Abort." << endl;
          return EXIT_FAILURE;
        }

      const auto tablePaths = optionMap["table-path"].as<vector<string>> ();
      const auto tableTypes = optionMap["table-type"].as<vector<string>> ();
      const auto tableHeaders = optionMap["table-header"].as<vector<string>> ();
      const auto tableNames = optionMap["table-name"].as<vector<string>> ();

      try
        {
          const std::size_t numTable = tablePaths.size ();
          for (std::size_t pos = 0; pos < numTable; ++pos)
            {
              const std::string path = tablePaths.at (pos);
              const vector<L> type = checkTableType (tableTypes.at (pos));
              const vector<Q> header = checkTableHeader (tableHeaders.at (pos));
              std::string name = tableNames.at (pos);
              trim (name);
              insertSym (createSymbol ((S) name.c_str ()));

              const std::size_t numCol = type.size ();
              if (header.size () != numCol)
                {
                  cerr << "Invalid load table operation. Abort." << endl;
                  return EXIT_FAILURE;
                }

              L typeArray[numCol];
              Q headerArray[numCol];
              for (std::size_t iter = 0; iter < numCol; ++iter)
                {
                  typeArray[iter] = type.at (iter);
                  headerArray[iter] = header.at (iter);
                }

              V table = readCSV ((S) path.c_str (), (L) numCol,
                                 (L *) typeArray, (Q *) headerArray);
              registerTable ((S) name.c_str (), table);
            }
        }
      catch (const BadTableTypeArgument &exception)
        {
          cerr << "Invalid load table operation. Abort." << endl;
          return EXIT_FAILURE;
        }
    }
  std::string line;
  std::string programString;
  while (std::getline (std::cin, line))
    {
      programString.append (line);
    }

  antlr4::ANTLRInputStream antlrStream (programString);
  horseIR::HorseIRLexer lexer (&antlrStream);
  antlr4::CommonTokenStream tokenStream (&lexer);
  horseIR::HorseIRParser parser (&tokenStream);

  lexer.removeErrorListeners ();
  parser.removeErrorListeners ();

  ErrorListener errorListener;
  lexer.addErrorListener (&errorListener);
  parser.addErrorListener (&errorListener);

  auto context = parser.program ();
  ASTNode::ASTNodeMemory memory;
  auto ast = CSTConverter::convert (memory, context);

  Interpreter interpreter (ast);
  printV (interpreter.interpret (entryModule, entryMethod));

  return EXIT_SUCCESS;
}
