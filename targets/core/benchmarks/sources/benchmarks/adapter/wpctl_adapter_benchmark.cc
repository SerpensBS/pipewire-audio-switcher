#include <benchmark/benchmark.h>

#include <chrono>
#include <mutex>
#include <optional>

#include "pas-core/adapter/wpctl_adapter.hh"
#include "pas-core/process/process_executor.hh"

namespace {
class WpctlAdapterBenchmark : public benchmark::Fixture {
 protected:
  constexpr static std::chrono::milliseconds kTimeout{1000};

  const pas::core::adapter::WpctlApapter adapter_;

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
std::mutex WpctlAdapterBenchmark::mutex_;
std::optional<bool> WpctlAdapterBenchmark::wpctl_installed_;

// NOLINTNEXTLINE(cert-err58-cpp)
BENCHMARK_F(WpctlAdapterBenchmark, GetSinks)(benchmark::State& state) {
  CheckWpctlInstall(state);

  for (const auto& current_state : state) {
    auto result = adapter_.GetSinks(kTimeout);
    benchmark::DoNotOptimize(result);
  }
}
}  // namespace
