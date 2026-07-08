#include "pas-core/parser/toml_parser.hh"

#include <gtest/gtest.h>

#include <sstream>
#include <string>

#include "pas-core/config/configuration.hh"
#include "pas-core/config/device_config.hh"

using namespace std::string_literals;

namespace {

class TomlParserTest : public ::testing::Test {
 protected:
  static auto ConvertConfigurationToString(const pas::core::config::Configuration& config)
      -> std::string;
};

}  // namespace

TEST_F(TomlParserTest, ParseSuccess) {
  const pas::core::config::Configuration expected_config{
      {{"Device1", pas::core::config::DeviceConfiguration{"icon1"s}},
       {"Device2", pas::core::config::DeviceConfiguration{"icon2"s}},
       {"Device3", pas::core::config::DeviceConfiguration{"icon3"s}}}};

  const std::string config = ConvertConfigurationToString(expected_config);

  auto parsed_config = pas::core::parser::TomlParser::ParseConfiguration(config);

  ASSERT_EQ(expected_config.devices.size(), parsed_config.devices.size());

  for (const auto& [key, device] : expected_config.devices) {
    ASSERT_EQ(device.icon, parsed_config.devices.at(key).icon);
  }
}

TEST_F(TomlParserTest, EmptyConfigurationParse) {
  auto parsed_config = pas::core::parser::TomlParser::ParseConfiguration(""s);

  ASSERT_TRUE(parsed_config.devices.empty());
}

auto TomlParserTest::ConvertConfigurationToString(const pas::core::config::Configuration& config)
    -> std::string {
  std::stringstream raw_configuration_stream;
  for (const auto& [key, device] : config.devices) {
    raw_configuration_stream << "[[devices]]"s;
    raw_configuration_stream << "\nname = \""s << key << "\"";
    raw_configuration_stream << "\nicon = \"" << device.icon << "\"";
    raw_configuration_stream << "\n\n ";
  }

  return raw_configuration_stream.str();
}
