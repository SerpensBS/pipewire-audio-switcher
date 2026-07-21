#include "parser/toml_parser.hh"

#include <string_view>
#include <toml++/toml.hpp>
#include <tuple>
#include <utility>

#include "config/configuration.hh"

namespace pas::core::parser {

auto TomlParser::ParseConfiguration(std::string_view configuration) -> config::Configuration {
  config::Configuration result;

  // Получаем список устройств:
  auto table = toml::parse(configuration);
  auto* devices = table["devices"].as_array();

  if (nullptr == devices) {
    return result;
  }

  // Перебираем все найденные устройства.
  for (const auto& item : *devices) {
    const auto* device = item.as_table();

    if (nullptr == device) {
      continue;
    }

    result.devices.emplace(std::piecewise_construct,
                           std::forward_as_tuple(device->get("name")->value_or("")),
                           std::forward_as_tuple(device->get("name")->value_or(""),
                                                 device->get("icon")->value_or("")));
  };

  return result;
}

}  // namespace pas::core::parser
