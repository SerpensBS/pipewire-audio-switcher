#ifndef PAS_CLI_OPTIONS_COMMAND_OPTION_HH
#define PAS_CLI_OPTIONS_COMMAND_OPTION_HH

#include <boost/program_options/options_description.hpp>
#include <boost/program_options/variables_map.hpp>
#include <string>

#include "data/input_parameters.hh"
#include "options/base_option.hh"

namespace pas::cli::options {

class CommandOption final : public BaseOption<std::string> {
 public:
  CommandOption();

  void ParseArgument(const boost::program_options::variable_value& value,
                     InputParameters& out_parameters) override;
};

}  // namespace pas::cli::options

#endif
