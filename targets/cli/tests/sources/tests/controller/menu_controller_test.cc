#include "controller/menu_controller.hh"

#include <gtest/gtest.h>

#include <boost/program_options/options_description.hpp>
#include <boost/program_options/variables_map.hpp>
#include <format>
#include <functional>
#include <sstream>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "options/base_option.hh"
#include "options/data/input_parameters.hh"

namespace {
class MenuControllerTest : public ::testing::Test {};

class MockOption final : public pas::cli::options::BaseOption {
 public:
  using RegisterFunction = std::function<void(boost::program_options::options_description&)>;
  using ParseArgumentFunction = std::function<void(const boost::program_options::variable_value&,
                                                   pas::cli::options::InputParameters&)>;
  struct CommandWritingStyle {
    std::string long_command;
    char short_command;
    std::string description;
  };

  MockOption(CommandWritingStyle command, RegisterFunction reg, ParseArgumentFunction parse)
      : pas::cli::options::BaseOption(
            std::move(command.long_command), command.short_command, std::move(command.description)),
        register_(std::move(reg)),
        parse_arg_(std::move(parse)) {}

  void Register(boost::program_options::options_description& description) override {
    description.add_options()(std::format("{},{}", GetName(), GetShortCommand()).c_str(), "");
    register_(description);
  }

  void ParseArgument(const boost::program_options::variable_value& value,
                     pas::cli::options::InputParameters& out_parameters) override {
    parse_arg_(value, out_parameters);
  }

 private:
  RegisterFunction register_;
  ParseArgumentFunction parse_arg_;
};
}  // namespace

TEST_F(MenuControllerTest, Initialize) {
  constexpr std::string_view kAppName{"InitializeTest"};
  pas::cli::controller::MenuController menu_controller{std::string(kAppName)};

  std::ostringstream oss;
  menu_controller.ShowHelp(oss);

  EXPECT_TRUE(oss.str().contains(kAppName));
}

TEST_F(MenuControllerTest, RegisterTest) {
  constexpr std::string_view kAppName{"RegisterTest"};
  pas::cli::controller::MenuController menu_controller{std::string(kAppName)};
  bool option_registered = false;

  MockOption option(
      {.long_command = "mock", .short_command = 'm', .description = "description"},
      [&option_registered](boost::program_options::options_description&) -> void {
        option_registered = true;
      },
      [](const boost::program_options::variable_value&,
         pas::cli::options::InputParameters&) -> void {});

  ASSERT_NO_THROW(menu_controller.RegisterCommand(option));

  std::stringstream stream;
  menu_controller.ShowHelp(stream);
  ASSERT_TRUE(stream.str().contains("mock"));
}

TEST_F(MenuControllerTest, RunTest) {
  constexpr std::string_view kAppName{"RunTest"};
  constexpr int kExpectedValue = 4;
  pas::cli::controller::MenuController menu_controller{std::string(kAppName)};
  bool option_called = false;

  MockOption option(
      {.long_command = "mock", .short_command = 'm', .description = "description"},
      [](boost::program_options::options_description&) -> void {},
      [&option_called, kExpectedValue](const boost::program_options::variable_value& value,
                                       pas::cli::options::InputParameters&) -> void {
        // ASSERT_EQ(kExpectedValue, value.as<int>());
        option_called = true;
      });

  menu_controller.RegisterCommand(option);
  ASSERT_FALSE(option_called);

  std::vector<const char*> argv = {kAppName.data(), "--mock"};

  menu_controller.ProcessArguments(static_cast<int>(argv.size()), argv.data());
  ASSERT_TRUE(option_called);
}
