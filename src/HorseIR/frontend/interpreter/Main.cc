#include <stdexcept>
#include <vector>
#include <string>
#include <sstream>

#include "boost/program_options.hpp"
#include "boost/algorithm/string.hpp"

#include "../ast/AST.h"
#include "../ast/CSTConverter.h"
#include "../interpreter/Interpreter.h"

#include "../../backend/h_global.h"
#include "../../backend/h_primitive.h"

#define HORSEIR_VERSION "dev"

using namespace boost::program_options;

std::vector<L> parseTableTypes (const std::string typeString)
{
  std::vector<L> parsedTypes;
  parsedTypes.reserve (typeString.size ());
  for (const std::string::value_type &character : typeString)
    {
      if (character == 'B') parsedTypes.push_back (H_B);        /* boolean    */
      else if (character == 'H') parsedTypes.push_back (H_H);   /* integer16  */
      else if (character == 'I') parsedTypes.push_back (H_I);   /* integer32  */
      else if (character == 'L') parsedTypes.push_back (H_L);   /* integer64  */
      else if (character == 'F') parsedTypes.push_back (H_F);   /* FP32       */
      else if (character == 'E') parsedTypes.push_back (H_E);   /* FP64       */
      else if (character == 'C') parsedTypes.push_back (H_C);   /* Character  */
      else if (character == 'Q') parsedTypes.push_back (H_Q);   /* Symbol     */
      else if (character == 'S') parsedTypes.push_back (H_S);   /* String     */
      else if (character == 'M') parsedTypes.push_back (H_M);   /* Month      */
      else if (character == 'D') parsedTypes.push_back (H_D);   /* Date       */
      else if (character == 'Z') parsedTypes.push_back (H_Z);   /* Date Time  */
      else if (character == 'U') parsedTypes.push_back (H_U);   /* Minute     */
      else if (character == 'W') parsedTypes.push_back (H_W);   /* Second     */
      else if (character == 'T') parsedTypes.push_back (H_T);   /* Time       */
      else if (character == 'X') parsedTypes.push_back (H_X);   /* Complex    */
      else throw std::invalid_argument ("invalid argument");
    }
  parsedTypes.shrink_to_fit ();
  return parsedTypes;
}

std::vector<Q> parseTableHeaders (const std::string &headerString)
{
  std::vector<Q> parsedHeaders;
  std::istringstream stream (headerString);
  std::string buffer;
  while (std::getline (stream, buffer, ','))
    {
      boost::trim (buffer);
      insertSym (createSymbol ((S) buffer.c_str ()));
      parsedHeaders.push_back (getSymbol ((S) buffer.c_str ()));
    }
  return parsedHeaders;
}

class ErrorListener : public antlr4::BaseErrorListener {
 public:
  void syntaxError (antlr4::Recognizer *recognizer,
                    antlr4::Token *offendingSymbol,
                    std::size_t line, std::size_t charPositionInLine,
                    const std::string &msg, std::exception_ptr e) override
  {
    std::cerr << '[' << line << ", " << charPositionInLine << ']'
              << msg << std::endl;
    exit (EXIT_FAILURE);
  }
};

int main (int argc, const char *argv[])
{
  using ArgType = std::vector<std::string>;
  std::string moduleNameArg, methodNameArg;
  options_description description ("Allowed options");
  description.add_options ()
      ("help,h", "produce help message")
      ("version,v", "version message")
      ("table-path,p", value<ArgType> (), "load table from given CSV file")
      ("table-name,n", value<ArgType> (), "specify the table name")
      ("table-type,t", value<ArgType> (), "specify the table column types")
      ("table-header,h", value<ArgType> (), "specify the table column headers")
      ("entry-module,M",
       value<std::string> (&moduleNameArg)->default_value ("default"),
       "specify HorseIR entry module name")
      ("entry-method,m",
       value<std::string> (&methodNameArg)->default_value ("main"),
       "specify HorseIR entry method name");
  variables_map argMap;
  store (parse_command_line (argc, argv, description), argMap);
  notify (argMap);

  if (argMap.count ("help") != 0)
    {
      std::cout << description << std::endl;
      return EXIT_SUCCESS;
    }
  if (argMap.count ("version") != 0)
    {
      std::cout << "HorseIR " HORSEIR_VERSION << std::endl;
      return EXIT_SUCCESS;
    }

  initMain ();
  initSym ();
  initSys ();

  /* loading table from CSV file */
  if (argMap.count ("table-path") != 0 || argMap.count ("table-name") != 0 ||
      argMap.count ("table-type") != 0 || argMap.count ("table-header") != 0)
    {
      if (argMap.count ("table-path") == 0 ||
          argMap.count ("table-name") == 0 ||
          argMap.count ("table-type") == 0 ||
          argMap.count ("table-header") == 0)
        {
          std::cerr << "Not Enough Number of Arguments" << std::endl;
          std::cerr << description << std::endl;
          return EXIT_FAILURE;
        }
      const ArgType rawTablePaths = argMap["table-path"].as<ArgType> ();
      const ArgType rawTableNames = argMap["table-name"].as<ArgType> ();
      const ArgType rawTableTypes = argMap["table-type"].as<ArgType> ();
      const ArgType rawTableHeaders = argMap["table-header"].as<ArgType> ();

      if (rawTablePaths.size () != rawTableNames.size () ||
          rawTableNames.size () != rawTableTypes.size () ||
          rawTableTypes.size () != rawTableHeaders.size () ||
          rawTableHeaders.size () != rawTablePaths.size ())
        {
          std::cerr << "Invalid Number of Arguments. Abort." << std::endl;
          std::cerr << description << std::endl;
          return EXIT_FAILURE;
        }

      const std::size_t numArgs = rawTablePaths.size ();
      assert (rawTableNames.size () == numArgs);
      assert (rawTableTypes.size () == numArgs);
      assert (rawTableHeaders.size () == numArgs);
      for (std::size_t pos = 0; pos < numArgs; ++pos)
        {
          ArgType::value_type tablePath = rawTablePaths[pos];
          boost::trim (tablePath);

          ArgType::value_type rawTableName = rawTableNames[pos];
          boost::trim (rawTableName);

          const ArgType::value_type &rawTableType = rawTableTypes[pos];
          std::vector<L> tableType;
          try
            { tableType = parseTableTypes (rawTableType); }
          catch (std::invalid_argument &exception)
            {
              std::cerr << "Invalid Table Type Specifier. Abort." << std::endl;
              std::cerr << description << std::endl;
              return EXIT_FAILURE;
            }
          const auto tableHeader = parseTableHeaders (rawTableHeaders[pos]);
          const std::size_t numCol = tableType.size ();
          if (tableHeader.size () != numCol)
            {
              std::cerr << "Error Occured During Loading Table" << tablePath
                        << ". Number of Columns Does Not Agree. Abort.";
              std::cerr << description << std::endl;
              return EXIT_FAILURE;
            }

          auto tableTypeArray = new L[tableType.size ()];
          for (std::size_t iter = 0; iter < tableType.size (); ++iter)
            { tableTypeArray[iter] = tableType[iter]; }
          auto tableHeaderArray = new Q[tableHeader.size ()];
          for (std::size_t iter = 0; iter < tableHeader.size (); ++iter)
            { tableHeaderArray[iter] = tableHeader[iter]; }
          V table = readCSV ((S) tablePath.c_str (),
                             static_cast<L>(numCol),
                             tableTypeArray, tableHeaderArray);
          registerTable ((S) rawTableName.c_str (), table);
          delete[] tableTypeArray;
          delete[] tableHeaderArray;
        }
    }

  std::string line;
  std::string programString;
  while (std::getline (std::cin, line))
    {
      programString.append (line);
      programString.append ("\n");
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
  horseIR::ast::ASTNode::ASTNodeMemory memory;
  auto ast = horseIR::ast::CSTConverter::convert (memory, context);

  horseIR::interpreter::Interpreter interpreter (ast);
  boost::trim (moduleNameArg);
  boost::trim (methodNameArg);
  printV (interpreter.interpret (moduleNameArg, methodNameArg));

  return EXIT_SUCCESS;
}

