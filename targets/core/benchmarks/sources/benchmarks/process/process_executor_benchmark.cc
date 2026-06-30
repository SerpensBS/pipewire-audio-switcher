#include <benchmark/benchmark.h>

#include <chrono>
#include <string>

#include "pas-core/process/process_executor.hh"

namespace {

using namespace std::string_literals;

class ProcessExecutorBenchmark : public benchmark::Fixture {
 protected:
  static constexpr std::chrono::milliseconds kExecuteTimeout{200};

  utility::ProcessExecutor executor_;
};

// NOLINTNEXTLINE(cert-err58-cpp)
BENCHMARK_F(ProcessExecutorBenchmark, ExecuteSuccess)(benchmark::State& state) {
  for (auto current_state : state) {
    auto result = executor_.Execute("/bin/true"s, {}, kExecuteTimeout);
  }
}

// NOLINTNEXTLINE(cert-err58-cpp)
BENCHMARK_F(ProcessExecutorBenchmark, ExecuteError)(benchmark::State& state) {
  for (auto current_state : state) {
    auto result = executor_.Execute("/bin/false"s, {}, kExecuteTimeout);
  }
}

// NOLINTNEXTLINE(cert-err58-cpp)
BENCHMARK_F(ProcessExecutorBenchmark, ExecuteSuccessWithMessage)(benchmark::State& state) {
  static const std::string kEchoMessage(1000, 'a');

  for (auto current_state : state) {
    auto result = executor_.Execute("/bin/echo"s, {kEchoMessage}, kExecuteTimeout);
  }
}

// NOLINTNEXTLINE(cert-err58-cpp)
BENCHMARK_F(ProcessExecutorBenchmark, ExecuteNonExistProcess)(benchmark::State& state) {
  for (auto current_state : state) {
    try {
      auto result = executor_.Execute("wrong_application_name"s, {}, kExecuteTimeout);
      // NOLINTNEXTLINE(bugprone-empty-catch)
    } catch (...) {
    }
  }
}
}  // namespace
