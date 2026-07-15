#ifndef PAS_CLI_OPTIONS_BASEOPTION_HH
#define PAS_CLI_OPTIONS_BASEOPTION_HH

#include <boost/program_options/options_description.hpp>
#include <boost/program_options/value_semantic.hpp>
#include <boost/program_options/variables_map.hpp>
#include <string>
#include <string_view>
#include <utility>

#include "options/interfaces/ioption.hh"

namespace pas::cli::options {
template <typename T>
class BaseOption : public IOption {
 public:
  explicit BaseOption(std::string&& long_command, char short_command, std::string&& description)
      : name_(std::move(long_command)),
        description_(std::move(description)),
        short_command_(short_command) {};

  [[nodiscard]]
  auto GetName() const -> std::string_view override {
    return name_;
  }

  [[nodiscard]]
  auto GetShortCommand() const -> char {
    return short_command_;
  }

  [[nodiscard]]
  auto GetDescription() const -> std::string {
    return description_;
  }

  ~BaseOption() override = default;

  auto operator=(const BaseOption&) -> BaseOption& = delete;
  auto operator=(BaseOption&&) -> BaseOption& = delete;

 protected:
  BaseOption(const BaseOption&) = default;
  BaseOption(BaseOption&&) = default;

 private:
  std::string name_;
  std::string description_;
  char short_command_;
};
}  // namespace pas::cli::options
#endif
