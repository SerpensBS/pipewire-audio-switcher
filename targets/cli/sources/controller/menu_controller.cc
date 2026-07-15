#include "controller/menu_controller.hh"

#include <boost/program_options.hpp>
#include <boost/program_options/cmdline.hpp>
#include <boost/program_options/detail/parsers.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/variables_map.hpp>
#include <ostream>
#include <string>

#include "options/data/input_parameters.hh"
#include "options/interfaces/ioption.hh"

namespace pas::cli::controller {

MenuController::MenuController(const std::string& app_description)
    : description_(app_description) {}

void MenuController::RegisterCommand(options::IOption& option) {
  option.Register(description_);
  commands_.emplace(option.GetName(), option);
}

auto MenuController::ProcessArguments(int argc, const char* const* argv) const
    -> options::InputParameters {
  boost::program_options::variables_map variables_map;

  boost::program_options::command_line_parser parser(argc, argv);
  parser.options(description_);
  parser.style(boost::program_options::command_line_style::unix_style |
               boost::program_options::command_line_style::allow_long_disguise);

  boost::program_options::store(parser.run(), variables_map);
  boost::program_options::notify(variables_map);

  options::InputParameters input_parameters{};

  for (const auto& [key, value] : variables_map) {
    commands_.at(key).ParseArgument(value, input_parameters);
  }

  return input_parameters;
}

void MenuController::ShowHelp(std::ostream& stream) const { stream << description_; }

}  // namespace pas::cli::controller
