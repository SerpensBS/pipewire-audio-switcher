#include "options/base_option.hh"

#include <string>
#include <string_view>
#include <utility>

namespace pas::cli::options {
BaseOption::BaseOption(std::string&& long_command, char short_command, std::string&& description)
    : name_(std::move(long_command)),
      description_(std::move(description)),
      short_command_(short_command) {};

auto BaseOption::GetName() const -> std::string_view { return name_; }

auto BaseOption::GetShortCommand() const -> char { return short_command_; }

auto BaseOption::GetDescription() const -> std::string { return description_; }
}  // namespace pas::cli::options
