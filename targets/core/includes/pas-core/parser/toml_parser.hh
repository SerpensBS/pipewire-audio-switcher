#ifndef PAS_CORE_PARSER_TOML_PARSER_HH
#define PAS_CORE_PARSER_TOML_PARSER_HH

#include <string_view>

#include "pas-core/config/configuration.hh"

namespace pas::core::parser {

class TomlParser {
 public:
  /**
   * @brief Парсинг конфигурации.
   *
   * @param configuration конфигурация
   */
  static auto ParseConfiguration(std::string_view configuration) -> config::Configuration;
};

};  // namespace pas::core::parser

#endif
