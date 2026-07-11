#include "options/command_option.hh"

#include <gtest/gtest.h>

#include <sstream>
#include <string>
#include <string_view>
#include <vector>

#include "controller/menu_controller.hh"
#include "options/data/command_type.hh"
#include "options/data/input_parameters.hh"

namespace {
class CommandOptionTest : public ::testing::Test {};
}  // namespace

TEST_F(CommandOptionTest, Initialize) { EXPECT_NO_THROW(pas::cli::options::CommandOption{}); }

TEST_F(CommandOptionTest, RegisterTest) {
  constexpr std::string_view kAppName{"RegisterTest"};
  pas::cli::controller::MenuController menu_controller{std::string(kAppName)};

  auto is_option_registered = [&menu_controller](std::string_view option_name) -> bool {
    std::stringstream stream;
    menu_controller.ShowHelp(stream);
    return stream.str().contains(option_name);
  };

  EXPECT_FALSE(is_option_registered("command"));

  pas::cli::options::CommandOption command_option;
  menu_controller.RegisterCommand(command_option);

  EXPECT_TRUE(is_option_registered("command"));
}

TEST_F(CommandOptionTest, ParseTest) {
  constexpr std::string_view kAppName{"RegisterTest"};
  pas::cli::controller::MenuController menu_controller{std::string(kAppName)};

  pas::cli::options::CommandOption command_option;
  menu_controller.RegisterCommand(command_option);

  std::vector<const char*> argv = {kAppName.data(), "--command=get-sink"};

  auto parsed_args = menu_controller.ProcessArguments(static_cast<int>(argv.size()), argv.data());
  EXPECT_EQ(pas::cli::options::CommandType::GetSink, parsed_args.command_type);
}
