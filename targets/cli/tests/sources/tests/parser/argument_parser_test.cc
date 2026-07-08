#include "parser/argument_parser.hh"

#include <gtest/gtest.h>

#include <array>
#include <utility>
#include <vector>

#include "data/input_parameters.hh"
#include "pas-core/command/data/command_type.hh"

using namespace std::string_literals;

namespace {
class ArgumentParserTest : public ::testing::Test {
 protected:
  pas::cli::parser::CLIParser parser_;
};

constexpr char const* kAppName{"pipewire-audio-switcher"};
}  // namespace

namespace {
class ArgumentPassingForm : public ArgumentParserTest,
                            public ::testing::WithParamInterface<
                                std::pair<std::vector<const char*>, pas::cli::InputParameters>> {};

TEST_P(ArgumentPassingForm, ParseArgumentsWithOrWithoutEqualSign) {
  const auto& [argv, expected_parsed_params] = GetParam();

  auto result = parser_.ParseParameters(static_cast<int>(argv.size()), argv.data());
  EXPECT_EQ(expected_parsed_params.command_type, result.command_type);
}

constexpr pas::cli::InputParameters kInputParameters{.command_type =
                                                         pas::core::command::CommandType::GetSink};
INSTANTIATE_TEST_SUITE_P(
    ArgumentParserTest,
    ArgumentPassingForm,
    testing::Values(
        std::make_pair(std::vector<const char*>{kAppName, "--command=get-sink"}, kInputParameters),
        std::make_pair(std::vector<const char*>{kAppName, "-c=get-sink"}, kInputParameters),
        std::make_pair(std::vector<const char*>{kAppName, "--command", "get-sink"},
                       kInputParameters),
        std::make_pair(std::vector<const char*>{kAppName, "-c", "get-sink"}, kInputParameters)));
}  // namespace

namespace {
class ParseCommand : public ArgumentParserTest,
                     public ::testing::WithParamInterface<
                         std::pair<std::vector<const char*>, pas::core::command::CommandType>> {};

TEST_P(ParseCommand, ParseCommand) {
  const auto& [argv, expected_command] = GetParam();

  auto result = parser_.ParseParameters(static_cast<int>(argv.size()), argv.data());
  EXPECT_EQ(expected_command, result.command_type);
}

INSTANTIATE_TEST_SUITE_P(
    ArgumentParserTest,
    ParseCommand,
    ::testing::Values(std::make_pair(std::vector<const char*>{kAppName, "--command", "get-sink"},
                                     pas::core::command::CommandType::GetSink)));
}  // namespace
