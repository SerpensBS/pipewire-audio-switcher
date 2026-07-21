#include <benchmark/benchmark.h>

#include <chrono>
#include <memory>
#include <mutex>
#include <optional>
#include <sstream>

#include "commands/get_sink_command.hh"
#include "pas-core/adapter/wpctl_adapter.hh"
#include "pas-core/process/process_executor.hh"

namespace {
class GetSinkCommandBenchmark : public benchmark::Fixture {
 protected:
  constexpr static std::chrono::milliseconds kTimeout{1000};

  const pas::core::adapter::WpctlApapter adapter_{
      std::make_shared<pas::core::process::ProcessExecutor>()};

  static auto CheckWpctlInstall(benchmark::State& state) {
    std::lock_guard<std::mutex> lock(mutex_);

    if (not wpctl_installed_.has_value()) {
      pas::core::process::ProcessExecutor process_executor;
      auto result = process_executor.Execute("/bin/wpctl", {"status"}, kTimeout);
      wpctl_installed_ = result.has_value();
    }

    if (not wpctl_installed_.value()) {
      state.SkipWithMessage("/bin/wpctl is not file or directory");
    }
  }

 private:
  static std::optional<bool> wpctl_installed_;
  static std::mutex mutex_;
};
std::mutex GetSinkCommandBenchmark::mutex_;
std::optional<bool> GetSinkCommandBenchmark::wpctl_installed_;

// NOLINTNEXTLINE(cert-err58-cpp)
BENCHMARK_F(GetSinkCommandBenchmark, Execute)(benchmark::State& state) {
  CheckWpctlInstall(state);

  std::stringstream error_stream;
  std::stringstream result_stream;

  pas::cli::commands::GetSinkCommand::Streams streams{.result_stream = result_stream,
                                                      .error_stream = error_stream};

  for (const auto& current_state : state) {
    pas::cli::commands::GetSinkCommand::Execute(adapter_, streams, kTimeout);
  }
}
}  // namespace
