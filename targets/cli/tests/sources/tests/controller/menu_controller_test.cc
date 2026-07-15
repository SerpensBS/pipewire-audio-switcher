#include "controller/menu_controller.hh"

#include <gtest/gtest.h>

#include <boost/program_options/options_description.hpp>
#include <boost/program_options/variables_map.hpp>
#include <functional>
#include <sstream>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "options/base_option.hh"
#include "options/data/input_parameters.hh"

namespace {
class MenuControllerTest : public ::testing::Test {
 protected:
  constexpr static std::string_view kAppName{"InitializeTest"};
  pas::cli::controller::MenuController menu_controller{std::string(kAppName)};
};

template <typename ValueT>
class MockOption final : public pas::cli::options::BaseOption<ValueT> {
 public:
  using ParseArgumentFunction = std::function<void(const boost::program_options::variable_value&,
                                                   pas::cli::options::InputParameters&)>;

  MockOption(std::string name,
             char short_command,
             std::string description,
             ParseArgumentFunction parse)
      : pas::cli::options::BaseOption<void>(std::move(name), short_command, std::move(description)),
        parse_arg_(std::move(parse)) {}

  void ParseArgument(const boost::program_options::variable_value& value,
                     pas::cli::options::InputParameters& out_parameters) override {
    parse_arg_(value, out_parameters);
  }

 private:
  ParseArgumentFunction parse_arg_;
};
}  // namespace

TEST_F(MenuControllerTest, Initialize) {
  pas::cli::controller::MenuController tested_controller{std::string(kAppName)};

  std::ostringstream oss;
  tested_controller.ShowHelp(oss);

  EXPECT_TRUE(oss.str().contains(kAppName));
}

TEST_F(MenuControllerTest, Register) {
  MockOption<void> option("mock",
                          'm',
                          "description",
                          [](const boost::program_options::variable_value&,
                             pas::cli::options::InputParameters&) -> void {});

  ASSERT_NO_THROW(menu_controller.RegisterCommand(option));

  std::stringstream stream;
  menu_controller.ShowHelp(stream);
  ASSERT_TRUE(stream.str().contains("mock"));
}

TEST_F(MenuControllerTest, CallVoidOption) {
  bool option_called = false;

  MockOption<void> option(
      "mock",
      'm',
      "description",
      [&option_called](const boost::program_options::variable_value&,
                       pas::cli::options::InputParameters&) -> void { option_called = true; });

  menu_controller.RegisterCommand(option);
  ASSERT_FALSE(option_called);

  auto call_option = [](const pas::cli::controller::MenuController& menu_controller,
                        const std::string& command) -> void {
    std::vector<const char*> argv = {kAppName.data(), command.c_str()};
    menu_controller.ProcessArguments(static_cast<int>(argv.size()), argv.data());
  };

  call_option(menu_controller, "--mock");
  ASSERT_TRUE(option_called);

  option_called = false;

  call_option(menu_controller, "-m");
  ASSERT_TRUE(option_called);
}
