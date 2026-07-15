#include "options/command_option.hh"

#include <gtest/gtest.h>

#include <array>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

#include "controller/menu_controller.hh"
#include "options/data/command_type.hh"
#include "options/data/input_parameters.hh"
#include "options/help_option.hh"

namespace {
class CommandOptionTest : public ::testing::Test {
 protected:
  constexpr static std::string_view kAppName{"RegisterTest"};

  pas::cli::controller::MenuController menu_controller_{std::string(kAppName)};
  pas::cli::options::CommandOption command_option_;
};
}  // namespace

TEST_F(CommandOptionTest, Initialize) { EXPECT_NO_THROW(pas::cli::options::CommandOption{}); }

TEST_F(CommandOptionTest, Register) {
  constexpr std::string_view kOptionName = "command";

  auto is_option_registered = [&](std::string_view option_name) -> bool {
    std::stringstream stream;
    menu_controller_.ShowHelp(stream);
    return stream.str().contains(option_name);
  };

  EXPECT_FALSE(is_option_registered(kOptionName));

  menu_controller_.RegisterCommand(command_option_);

  EXPECT_TRUE(is_option_registered(kOptionName));
}

TEST_F(CommandOptionTest, HelpCommandNotChanged) {
  pas::cli::options::HelpOption help_option;

  menu_controller_.RegisterCommand(command_option_);
  menu_controller_.RegisterCommand(help_option);

  {
    const auto args = std::to_array<const char*>({"", "--help", "--command=get-sink"});

    auto parsed_args =
        menu_controller_.ProcessArguments(static_cast<int>(args.size()), args.data());
    EXPECT_EQ(pas::cli::options::CommandType::Help, parsed_args.command_type);
  }

  {
    const auto args = std::to_array<const char*>({"", "--command=get-sink", "--help"});

    auto parsed_args =
        menu_controller_.ProcessArguments(static_cast<int>(args.size()), args.data());
    EXPECT_EQ(pas::cli::options::CommandType::Help, parsed_args.command_type);
  }
}

namespace {
class CommandOptionParseTest : public CommandOptionTest,
                               public ::testing::WithParamInterface<std::vector<const char*>> {
 protected:
  pas::cli::options::CommandOption command_option_;
};
}  // namespace

TEST_P(CommandOptionParseTest, Parse) {
  menu_controller_.RegisterCommand(command_option_);

  const ParamType& argv = GetParam();

  auto parsed_args = menu_controller_.ProcessArguments(static_cast<int>(argv.size()), argv.data());
  EXPECT_EQ(pas::cli::options::CommandType::GetSink, parsed_args.command_type);
}

INSTANTIATE_TEST_SUITE_P(CommandFormats,
                         CommandOptionParseTest,
                         ::testing::Values(std::vector<const char*>{"", "--command", "get-sink"},
                                           std::vector<const char*>{"", "--command=get-sink"},
                                           std::vector<const char*>{"", "-c", "get-sink"},
                                           std::vector<const char*>{"", "-c=get-sink"}));
