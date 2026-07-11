#ifndef PAS_CLI_OPTIONS_BASEOPTION_HH
#define PAS_CLI_OPTIONS_BASEOPTION_HH

#include <boost/program_options/options_description.hpp>
#include <boost/program_options/variables_map.hpp>
#include <string>
#include <string_view>

#include "options/interfaces/ioption.hh"

namespace pas::cli::options {
class BaseOption : public IOption {
 public:
  explicit BaseOption(std::string&& long_command, char short_command);

  [[nodiscard]]
  auto GetFullCommand() const -> std::string_view override;

  [[nodiscard]]
  auto GetShortCommand() const -> char override;

  ~BaseOption() override = default;

  auto operator=(const BaseOption&) -> BaseOption& = delete;
  auto operator=(BaseOption&&) -> BaseOption& = delete;

 protected:
  BaseOption(const BaseOption&) = default;
  BaseOption(BaseOption&&) = default;

 private:
  std::string long_command_;
  char short_command_;
};

}  // namespace pas::cli::options
#endif
