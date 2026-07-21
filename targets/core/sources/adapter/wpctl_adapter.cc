#include "pas-core/adapter/wpctl_adapter.hh"

#include <chrono>
#include <cstddef>
#include <expected>
#include <memory>
#include <regex>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include "adapter/data/sink.hh"
#include "process/interfaces/iprocess_executor.hh"
#include "utility/error.hh"

namespace {
auto GetSinks(const std::regex& regex,
              const pas::core::process::IProcessExecutor& process_executor,
              std::chrono::milliseconds timeout)
    -> std::expected<std::vector<pas::core::adapter::Sink>, pas::core::utility::Error> {
  auto wpctl_status_result = process_executor.Execute("/bin/wpctl", {"status"}, timeout);

  if (not wpctl_status_result.has_value()) {
    return std::unexpected(wpctl_status_result.error());
  }

  std::istringstream stream(wpctl_status_result.value());
  std::string line;

  while (std::getline(stream, line)) {
    if (line.contains("Sinks")) {
      break;
    }
  }

  std::vector<pas::core::adapter::Sink> result;

  // Ищем только активные sinks (со звездочкой)
  while (std::getline(stream, line)) {
    constexpr std::size_t kMinSinkStringSize = 10;
    if (kMinSinkStringSize > line.size()) {
      break;
    }

    std::smatch matches;
    if (std::regex_search(line, matches, regex)) {
      // Очищаем имя от лишних пробелов
      std::string name = matches[2].str();
      size_t start = name.find_first_not_of(" \t");
      size_t end = name.find_last_not_of(" \t");
      if (start != std::string::npos) {
        name = name.substr(start, end - start + 1);
      }

      result.emplace_back(pas::core::adapter::Sink{.id = std::stoi(matches[1].str()),
                                                   .name = std::move(name),
                                                   .volume = std::stod(matches[3].str()),
                                                   .is_active = line.contains('*')});
    }
  }

  return result;
}
}  // namespace
namespace pas::core::adapter {

WpctlApapter::WpctlApapter(std::shared_ptr<pas::core::process::IProcessExecutor> process_executor)
    : process_executor_(std::move(process_executor)) {}

auto WpctlApapter::GetSinks(std::chrono::milliseconds timeout) const
    -> std::expected<std::vector<Sink>, utility::Error> {
  return ::GetSinks(std::regex{R"(\s*\s*(\d+)\.\s+(.+?)\s+\[vol:\s*([\d.]+)\])"},
                    *process_executor_.get(),
                    timeout);
}

auto WpctlApapter::SetSink(std::size_t sink_id, std::chrono::milliseconds timeout) const
    -> std::expected<void, utility::Error> {
  auto wpctl_status_result =
      process_executor_->Execute("/bin/wpctl", {"set-default", std::to_string(sink_id)}, timeout);

  if (not wpctl_status_result.has_value()) {
    return std::unexpected(wpctl_status_result.error());
  }

  return {};
}
}  // namespace pas::core::adapter
