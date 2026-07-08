#ifndef PAS_CLI_PARSER_CLIPARSER_HH
#define PAS_CLI_PARSER_CLIPARSER_HH

#include "data/input_parameters.hh"

namespace pas::cli::parser {
/**
 * @class CLIParser
 * @brief Парсер параметров, передаваемых из/в CLI.
 *
 */
class CLIParser {
 public:
  auto ParseParameters(int argc, const char* const* argv) const -> InputParameters;
};
}  // namespace pas::cli::parser

#endif
