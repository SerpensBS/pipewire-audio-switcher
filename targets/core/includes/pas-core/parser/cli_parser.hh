#ifndef PAS_CORE_PARSER_CLIPARSER_HH
#define PAS_CORE_PARSER_CLIPARSER_HH

#include "pas-core/cli/input_parameters.hh"

namespace pas::core::parser {

/**
 * @class CLIParser
 * @brief Парсер параметров, передаваемых из/в CLI.
 *
 */
class CLIParser {
 public:
  auto ParseParameters(int argc, char** argv) -> cli::InputParameters;
};
}  // namespace pas::core::parser

#endif
