#include <benchmark/benchmark.h>

#include <sstream>
#include <string>

#include "pas-core/config/configuration.hh"
#include "pas-core/config/device_config.hh"
#include "pas-core/parser/toml_parser.hh"

namespace {

using namespace std::string_literals;

class TomlParserBenchmark : public benchmark::Fixture {
 protected:
  static auto ConvertConfigurationToString(const pas::core::config::Configuration& config)
      -> std::string;
};

// NOLINTNEXTLINE(cert-err58-cpp)
BENCHMARK_F(TomlParserBenchmark, ParseConfiguration)(benchmark::State& state) {
  const pas::core::config::Configuration expected_config{
      {{"Device1", pas::core::config::DeviceConfiguration{"icon1"s}},
       {"Device2", pas::core::config::DeviceConfiguration{"icon2"s}},
       {"Device3", pas::core::config::DeviceConfiguration{"icon3"s}}}};

  const std::string config = ConvertConfigurationToString(expected_config);

  for (auto current_state : state) {
    auto result = pas::core::parser::TomlParser::ParseConfiguration(config);
    benchmark::DoNotOptimize(result);
  }
}

// NOLINTNEXTLINE(cert-err58-cpp)
BENCHMARK_F(TomlParserBenchmark, ParseEmptyConfiguration)(benchmark::State& state) {
  const std::string emptyConfig;

  for (auto current_state : state) {
    auto result = pas::core::parser::TomlParser::ParseConfiguration(emptyConfig);
    benchmark::DoNotOptimize(result);
  }
}

auto TomlParserBenchmark::ConvertConfigurationToString(
    const pas::core::config::Configuration& config) -> std::string {
  std::stringstream raw_configuration_stream;
  for (const auto& [key, device] : config.devices) {
    raw_configuration_stream << "[[devices]]"s;
    raw_configuration_stream << "\nname = \""s << key << "\"";
    raw_configuration_stream << "\nicon = \"" << device.icon << "\"";
    raw_configuration_stream << "\n\n ";
  }

  return raw_configuration_stream.str();
}
}  // namespace
