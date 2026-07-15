#include "options/help_option.hh"

#include <gtest/gtest.h>

#include <sstream>
#include <string>
#include <string_view>
#include <vector>

#include "controller/menu_controller.hh"
#include "options/data/command_type.hh"
#include "options/data/input_parameters.hh"

namespace {
class HelpOptionTest : public ::testing::Test {
 protected:
  constexpr static std::string_view kAppName{"RegisterTest"};
  pas::cli::controller::MenuController menu_controller{std::string(kAppName)};
};
}  // namespace

TEST_F(HelpOptionTest, Initialize) { EXPECT_NO_THROW(pas::cli::options::HelpOption{}); }

TEST_F(HelpOptionTest, Register) {
  constexpr std::string_view kOptionName = "help";
  auto is_option_registered = [&](std::string_view option_name) -> bool {
    std::stringstream stream;
    menu_controller.ShowHelp(stream);
    return stream.str().contains(option_name);
  };

  EXPECT_FALSE(is_option_registered(kOptionName));

  pas::cli::options::HelpOption help_option;
  menu_controller.RegisterCommand(help_option);

  EXPECT_TRUE(is_option_registered(kOptionName));
}

TEST_F(HelpOptionTest, Parse) {
  pas::cli::options::HelpOption help_option;
  menu_controller.RegisterCommand(help_option);

  auto call_option = [](const pas::cli::controller::MenuController& menu_controller,
                        const std::string& command) -> pas::cli::options::InputParameters {
    std::vector<const char*> argv = {kAppName.data(), command.c_str()};
    return menu_controller.ProcessArguments(static_cast<int>(argv.size()), argv.data());
  };

  EXPECT_EQ(pas::cli::options::CommandType::Help,
            call_option(menu_controller, "--help").command_type);
  EXPECT_EQ(pas::cli::options::CommandType::Help, call_option(menu_controller, "-h").command_type);
}
