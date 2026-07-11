#include "options/help_option.hh"

#include <boost/program_options/options_description.hpp>
#include <boost/program_options/variables_map.hpp>
#include <format>

#include "data/input_parameters.hh"
#include "options/base_option.hh"
#include "options/data/command_type.hh"

namespace pas::cli::options {

HelpOption::HelpOption() : BaseOption("help", 'h'), description_(nullptr) {}

void HelpOption::Register(boost::program_options::options_description& description) {
  description.add_options()(std::format("{},{}", GetFullCommand(), GetShortCommand()).c_str(),
                            "Help screen");
  description_ = &description;
};

void HelpOption::ParseArgument(const boost::program_options::variable_value& value,
                               InputParameters& out_parameters) {
  // Данный аргумент не принимает значений.
  (void)value;
  out_parameters.command_type = CommandType::Help;
}

}  // namespace pas::cli::options
