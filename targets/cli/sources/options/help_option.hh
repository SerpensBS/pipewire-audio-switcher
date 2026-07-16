#ifndef PAS_CLI_OPTIONS_HELP_OPTION_HH
#define PAS_CLI_OPTIONS_HELP_OPTION_HH

#include <boost/program_options/options_description.hpp>
#include <boost/program_options/variables_map.hpp>

#include "data/input_parameters.hh"
#include "options/base_option.hh"

namespace pas::cli::options {

class HelpOption final : public BaseOption<void> {
 public:
  HelpOption();

  void ParseArgument(const boost::program_options::variable_value& value,
                     InputParameters& out_parameters) override;
};

}  // namespace pas::cli::options

#endif
