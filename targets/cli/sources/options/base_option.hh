#ifndef PAS_CLI_OPTIONS_BASEOPTION_HH
#define PAS_CLI_OPTIONS_BASEOPTION_HH

#include <boost/program_options/options_description.hpp>
#include <boost/program_options/value_semantic.hpp>
#include <boost/program_options/variables_map.hpp>
#include <format>
#include <string>
#include <string_view>
#include <utility>

#include "options/interfaces/ioption.hh"

namespace pas::cli::options {
class BaseOption : public IOption {
 public:
  explicit BaseOption(std::string&& long_command, char short_command, std::string&& description);

  [[nodiscard]]
  auto GetName() const -> std::string_view override;

  void Register(boost::program_options::options_description& description) override {
    description.add_options()(std::format("{},{}", GetName(), GetShortCommand()).c_str(),
                              GetDescription().c_str());
  }
  ~BaseOption() override = default;

  auto operator=(const BaseOption&) -> BaseOption& = delete;
  auto operator=(BaseOption&&) -> BaseOption& = delete;

 protected:
  BaseOption(const BaseOption&) = default;
  BaseOption(BaseOption&&) = default;

  [[nodiscard]]
  auto GetShortCommand() const -> char;

  [[nodiscard]]
  auto GetDescription() const -> std::string;

 private:
  std::string name_;
  std::string description_;
  char short_command_;
};

template <typename T>
class BaseOptionWithParameter : public BaseOption {
 public:
  explicit BaseOptionWithParameter(std::string&& long_command,
                                   char short_command,
                                   std::string&& description)
      : BaseOption(std::move(long_command), short_command, std::move(description)) {};

  void Register(boost::program_options::options_description& description) override {
    description.add_options()(std::format("{},{}", GetName(), GetShortCommand()).c_str(),
                              boost::program_options::value<T>(),
                              GetDescription().c_str());
  };

  auto operator=(const BaseOption&) -> BaseOption& = delete;
  auto operator=(BaseOption&&) -> BaseOption& = delete;
};
}  // namespace pas::cli::options
#endif
