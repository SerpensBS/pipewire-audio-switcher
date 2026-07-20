#include "commands/cycle_sink_command.hh"

#include <algorithm>
#include <chrono>
#include <format>
#include <memory>
#include <optional>
#include <ostream>
#include <system_error>
#include <vector>

#include "pas-core/adapter/data/sink.hh"
#include "pas-core/adapter/wpctl_adapter.hh"
#include "pas-core/process/process_executor.hh"
#include "pas-core/utility/error.hh"

namespace {
void ProceedError(std::ostream& error_stream, const pas::core::utility::Error& error) {
  error_stream << std::format(
      "Cycle sink command failed with code {}: {}\n", error.code.value(), error.message);
}

auto FindNextActive(const std::vector<pas::core::adapter::Sink>& sinks)
    -> std::optional<pas::core::adapter::Sink> {
  if (sinks.empty()) {
    return std::nullopt;
  }

  auto active_it = std::ranges::find_if(
      sinks, [](const pas::core::adapter::Sink& sink) -> bool { return sink.is_active; });

  if (active_it == sinks.end()) {
    return std::nullopt;
  }

  return (active_it == sinks.end() - 1) ? sinks.front() : *(active_it + 1);
}
}  // namespace

namespace pas::cli::commands {
void CycleSinkCommand::Execute(std::ostream& error_stream, std::chrono::milliseconds timeout) {
  pas::core::adapter::WpctlApapter wpctl_adapter(
      std::make_shared<pas::core::process::ProcessExecutor>());

  // Получаем список sink'ов.
  const auto get_sinks_result = wpctl_adapter.GetSinks(timeout);
  if (not get_sinks_result.has_value()) {
    ProceedError(error_stream, get_sinks_result.error());
    return;
  }

  const auto& sinks = get_sinks_result.value();

  // Проверяем что есть как минимум два sink'а, иначе выходим.
  if (sinks.size() < 2) {
    return;
  }

  auto next_sink = FindNextActive(sinks);

  if (not next_sink.has_value()) {
    ProceedError(error_stream,
                 {.code = std::make_error_code(std::errc::invalid_argument),
                  .message = "active sink not found"});
  }

  auto set_sink_result = wpctl_adapter.SetSink(next_sink->id, timeout);

  if (not set_sink_result.has_value()) {
    ProceedError(error_stream, set_sink_result.error());
  }
}
}  // namespace pas::cli::commands
