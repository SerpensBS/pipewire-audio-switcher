#ifndef PAS_PARSERS_TOML_PARSER_HH
#define PAS_PARSERS_TOML_PARSER_HH

#include <string_view>

#include "config/configuration.hh"

namespace pas::parser {

class TomlParser {
 public:
  static auto ParseConfiguration(std::string_view configuration) -> config::Configuration;
};

};  // namespace pas::parser

#endif
