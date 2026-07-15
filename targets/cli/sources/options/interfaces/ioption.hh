#ifndef PAS_CLI_OPTIONS_INTERFACES_IOPTION_HH
#define PAS_CLI_OPTIONS_INTERFACES_IOPTION_HH

#include <boost/program_options/options_description.hpp>
#include <boost/program_options/variables_map.hpp>
#include <string_view>

#include "options/data/input_parameters.hh"

namespace pas::cli::options {

class IOption {
 public:
  [[nodiscard]]
  virtual auto GetName() const -> std::string_view = 0;

  virtual void Register(boost::program_options::options_description& description) = 0;

  virtual void ParseArgument(const boost::program_options::variable_value& value,
                             InputParameters& out_parameters) = 0;

  virtual ~IOption() = default;

 protected:
  IOption() = default;
  IOption(const IOption&) = default;
  IOption(IOption&&) = default;
  auto operator=(const IOption&) -> IOption& = default;
  auto operator=(IOption&&) -> IOption& = default;
};

}  // namespace pas::cli::options

#endif
