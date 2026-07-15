#include "options/command_option.hh"

#include <boost/program_options/options_description.hpp>
#include <boost/program_options/value_semantic.hpp>
#include <boost/program_options/variables_map.hpp>
#include <format>
#include <map>
#include <string>
#include <string_view>

#include "data/input_parameters.hh"
#include "options/base_option.hh"
#include "options/data/command_type.hh"

namespace pas::cli::options {

CommandOption::CommandOption()
    : BaseOptionWithParameter("command", 'c', "Command to be executed by the application") {}

void CommandOption::ParseArgument(const boost::program_options::variable_value& value,
                                  InputParameters& out_parameters) {
  static const std::map<const std::string_view, CommandType> command_type_map{
      {"get-sink", CommandType::GetSink}};

  out_parameters.command_type = command_type_map.at(value.as<const std::string&>());
}

}  // namespace pas::cli::options
