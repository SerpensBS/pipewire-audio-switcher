#include "options/base_option.hh"

#include <string>
#include <string_view>
#include <utility>

namespace pas::cli::options {
BaseOption::BaseOption(std::string&& long_command, char short_command)
    : long_command_(std::move(long_command)), short_command_(short_command) {};

auto BaseOption::GetFullCommand() const -> std::string_view { return long_command_; }

auto BaseOption::GetShortCommand() const -> char { return short_command_; }

}  // namespace pas::cli::options
