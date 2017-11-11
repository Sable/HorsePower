#include <stdexcept>
#include <vector>
#include <string>
#include <sstream>
#include <chrono>

#include <yaml-cpp/yaml.h>
#include <spdlog/spdlog.h>

#include "../ast/AST.h"
#include "../ast/CSTConverter.h"
#include "../interpreter/Interpreter.h"
#include "../misc/StringUtils.h"

#include "../../backend/h_global.h"
#include "../../backend/h_primitive.h"

L parseTableTypes (const std::string &str)
{
  assert (str.length () == 1);
  const char character = str[0];
  if (character == 'B') return H_B;        /* boolean    */
  else if (character == 'H') return H_H;   /* integer16  */
  else if (character == 'I') return H_I;   /* integer32  */
  else if (character == 'L') return H_L;   /* integer64  */
  else if (character == 'F') return H_F;   /* FP32       */
  else if (character == 'E') return H_E;   /* FP64       */
  else if (character == 'C') return H_C;   /* Character  */
  else if (character == 'Q') return H_Q;   /* Symbol     */
  else if (character == 'S') return H_S;   /* String     */
  else if (character == 'M') return H_M;   /* Month      */
  else if (character == 'D') return H_D;   /* Date       */
  else if (character == 'Z') return H_Z;   /* Date Time  */
  else if (character == 'U') return H_U;   /* Minute     */
  else if (character == 'W') return H_W;   /* Second     */
  else if (character == 'T') return H_T;   /* Time       */
  else if (character == 'X') return H_X;   /* Complex    */
  else throw std::invalid_argument ("invalid argument");
}

std::string typeToString (L type)
{
  if (type == H_B) return "B (boolean)";
  else if (type == H_H) return "H (integer16)";
  else if (type == H_I) return "I (integer32)";
  else if (type == H_L) return "L (integer64)";
  else if (type == H_F) return "F (FP32)";
  else if (type == H_E) return "E (FP64)";
  else if (type == H_C) return "C (character)";
  else if (type == H_Q) return "Q (symbol)";
  else if (type == H_S) return "S (string)";
  else if (type == H_M) return "M (month)";
  else if (type == H_D) return "D (date)";
  else if (type == H_Z) return "Z (date time)";
  else if (type == H_U) return "U (minute)";
  else if (type == H_W) return "W (second)";
  else if (type == H_T) return "T (time)";
  else if (type == H_X) return "X (complex)";
  else throw std::invalid_argument ("invalid argument");
}

int main (int argc, const char *argv[])
{

  if (argc != 4)
    {
      std::cout << "usage:" << std::endl
                << argv[0] << " [database config(.yml)]" << ' '
                << "[key relationship(.yml)]" << ' '
                << "[hir file]" << std::endl;
      return EXIT_FAILURE;
    }

  auto logger = spdlog::stdout_color_mt ("console");
  logger->set_pattern ("[%n] [%l] %v");

  initMain ();
  initSys ();
  logger->info ("initializing system ...");
  initSym ();
  logger->info ("initializing symbol pool ...");

  const std::string tableYamlPath = argv[1];
  const std::string keyYamlPath = argv[2];

  YAML::Node configYAMLNode = YAML::LoadFile (tableYamlPath);
  logger->info ("loading configuration file at {0}", tableYamlPath);
  for (std::size_t tableIndex = 0;
       tableIndex < configYAMLNode.size (); ++tableIndex)
    {
      logger->info ("loading #{} table", tableIndex + 1);
      const std::string path = horseIR::misc::trim_copy (
          configYAMLNode[tableIndex]["table"]["path"].as<std::string> ());
      const std::string name = horseIR::misc::trim_copy (
          configYAMLNode[tableIndex]["table"]["name"].as<std::string> ());
      logger->info ("table #{} CSV path: {}", tableIndex + 1, path);
      logger->info ("table #{} name: {}", tableIndex + 1, name);

      auto contentNode = configYAMLNode[tableIndex]["table"]["content"];
      std::vector<std::string> fieldHeaders;
      fieldHeaders.reserve (contentNode.size ());
      std::unique_ptr<L[]> fieldTypes (new L[contentNode.size ()]);
      for (std::size_t fieldIndex = 0;
           fieldIndex < contentNode.size (); ++fieldIndex)
        {
          std::string fieldHeader =
              contentNode[fieldIndex]["name"].as<std::string> ();
          fieldHeaders.emplace_back (std::move (fieldHeader));
          const std::string fieldType =
              contentNode[fieldIndex]["type"].as<std::string> ();
          fieldTypes[fieldIndex] = parseTableTypes (fieldType);
          logger->info ("table #{} field #{} header {} type {}",
                        tableIndex + 1, fieldIndex + 1,
                        fieldHeaders[fieldIndex],
                        typeToString (fieldTypes[fieldIndex])
          );
        }
      std::unique_ptr<C *[]> fieldHeadersCStr (new C *[contentNode.size ()]);
      for (std::size_t index = 0; index < contentNode.size (); ++index)
        fieldHeadersCStr[index] = (C *) fieldHeaders[index].c_str ();
      std::unique_ptr<Q[]> fieldHeadersSym (new Q[contentNode.size ()]);
      for (std::size_t index = 0; index < contentNode.size (); ++index)
        {
          insertSym (createSymbol (fieldHeadersCStr[index]));
          fieldHeadersSym[index] = getSymbol (fieldHeadersCStr[index]);
        }
      V table = readCSV ((S) path.c_str (),
                         (L) contentNode.size (),
                         &fieldTypes[0],
                         &fieldHeadersSym[0]);
      registerTable ((S) name.c_str (), table);
    }
  logger->info ("table loaded");
  logger->info ("loading key relationship at {}", keyYamlPath);
  YAML::Node keyConfigNode = YAML::LoadFile (keyYamlPath);
  for (std::size_t relationIndex = 0;
       relationIndex < keyConfigNode.size (); ++relationIndex)
    {
      const std::string xTable =
          keyConfigNode[relationIndex]["xTable"].as<std::string> ();
      const std::string xKey =
          keyConfigNode[relationIndex]["xKey"].as<std::string> ();
      const std::string yTable =
          keyConfigNode[relationIndex]["yTable"].as<std::string> ();
      const std::string yKey =
          keyConfigNode[relationIndex]["yKey"].as<std::string> ();
      logger->info ("loading {}.{} -> {}.{} ...", xTable, xKey, yTable, yKey);


      V xTableNode = allocNode (); initV (xTableNode, H_Q, 1);
      V xKeyNode = allocNode (); initV (xKeyNode, H_Q, 1);
      V yTableNode = allocNode (); initV (yTableNode, H_Q, 1);
      V yKeyNode = allocNode (); initV (yKeyNode, H_Q, 1);

      vq (xTableNode) = getSymbol ((S) xTable.c_str ());
      vq (xKeyNode) = getSymbol ((S) xKey.c_str ());
      vq (yTableNode) = getSymbol ((S) yTable.c_str ());
      vq (yKeyNode) = getSymbol ((S) yKey.c_str ());

      pfnAddFKey (xTableNode, xKeyNode, yTableNode, yKeyNode);
    }

  std::ifstream stream (argv[3]);
  antlr4::ANTLRInputStream inputStream (stream);
  horseIR::HorseIRLexer lexer (&inputStream);
  antlr4::CommonTokenStream tokenStream (&lexer);
  horseIR::HorseIRParser parser (&tokenStream);

  horseIR::ast::ASTNode::ASTNodeMemory memory;

  auto startParse = std::chrono::steady_clock::now ();
  auto programCST = parser.program ();
  auto programAST = horseIR::ast::CSTConverter::convert (memory, programCST);
  auto endParse = std::chrono::steady_clock::now ();
  logger->info ("parsed ... ({}ms)",
                std::chrono::duration_cast<std::chrono::milliseconds> (
                    endParse - startParse).count ());

  auto startExec = std::chrono::steady_clock::now ();
  horseIR::interpreter::Interpreter interpreter (programAST);
  V result = interpreter.interpret ("default", "main");
  auto endExec = std::chrono::steady_clock::now ();
  printV (result);
  logger->info ("done. (parsing: {}ms, execution: {}ms)",
                std::chrono::duration_cast<std::chrono::milliseconds> (
                    endParse - startParse).count (),
                std::chrono::duration_cast<std::chrono::milliseconds> (
                    endExec - startExec).count ());

  return EXIT_SUCCESS;
}

