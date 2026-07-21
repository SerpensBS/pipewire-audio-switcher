#include "commands/get_sink_command.hh"

#include <algorithm>
#include <chrono>
#include <expected>
#include <format>
#include <ostream>
#include <system_error>

#include "pas-core/adapter/data/sink.hh"
#include "pas-core/adapter/interfaces/iwpctl_adapter.hh"
#include "pas-core/utility/error.hh"

namespace {
void ShowErrorMessage(std::ostream& error_stream, const pas::core::utility::Error& error) {
  error_stream << std::format(
      "Get sink command failed with code {}: {}\n", error.code.value(), error.message);
}
}  // namespace

namespace pas::cli::commands {
void GetSinkCommand::Execute(const pas::core::adapter::IWpctlApapter& adapter,
                             Streams streams,
                             std::chrono::milliseconds timeout) {
  // Получаем список sink'ов.
  const auto get_sinks_result = adapter.GetSinks(timeout);

  if (not get_sinks_result.has_value()) {
    ShowErrorMessage(streams.error_stream, get_sinks_result.error());
    return;
  }

  const auto& sinks = get_sinks_result.value();
  auto sink_it = std::ranges::find_if(
      sinks, [](const pas::core::adapter::Sink& sink) -> bool { return sink.is_active; });

  if (sinks.end() == sink_it) {
    ShowErrorMessage(streams.error_stream,
                     {.code = std::make_error_code(std::errc::invalid_seek),
                      .message = "Active sink not found"});
    return;
  }

  const auto& active_sink = *sink_it;
  streams.result_stream
      << R"({"id":)" << active_sink.id << R"(,"name":")" << active_sink.name
      << "\","
      // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
      << R"("volume":)" << static_cast<unsigned int>(active_sink.volume * 100) << "}\n";
}
}  // namespace pas::cli::commands
