#include <benchmark/benchmark.h>

#include <array>

#include "parser/argument_parser.hh"

namespace {

class ArgumentParserBenchmark : public benchmark::Fixture {
 protected:
  pas::cli::parser::CLIParser parser_;
};

// NOLINTNEXTLINE(cert-err58-cpp)
BENCHMARK_F(ArgumentParserBenchmark, ParseFullArgumentsWithEqual)(benchmark::State& state) {
  for (auto current_state : state) {
    constexpr auto kArgv = std::to_array<const char*>({"--command=get-sink"});

    auto result = parser_.ParseParameters(kArgv.size(), kArgv.data());
    benchmark::DoNotOptimize(result);
  }
}

// NOLINTNEXTLINE(cert-err58-cpp)
BENCHMARK_F(ArgumentParserBenchmark, ParseFullArgumentsWithoutEqual)(benchmark::State& state) {
  for (auto current_state : state) {
    constexpr auto kArgv = std::to_array<const char*>({"--command get-sink"});

    auto result = parser_.ParseParameters(kArgv.size(), kArgv.data());
    benchmark::DoNotOptimize(result);
  }
}

// NOLINTNEXTLINE(cert-err58-cpp)
BENCHMARK_F(ArgumentParserBenchmark, ParseShortArgumentsWithEqual)(benchmark::State& state) {
  for (auto current_state : state) {
    constexpr auto kArgv = std::to_array<const char*>({"-c=get-sink"});

    auto result = parser_.ParseParameters(kArgv.size(), kArgv.data());
    benchmark::DoNotOptimize(result);
  }
}

// NOLINTNEXTLINE(cert-err58-cpp)
BENCHMARK_F(ArgumentParserBenchmark, ParseShortArgumentsWithoutEqual)(benchmark::State& state) {
  for (auto current_state : state) {
    constexpr auto kArgv = std::to_array<const char*>({"-c get-sink"});

    auto result = parser_.ParseParameters(kArgv.size(), kArgv.data());
    benchmark::DoNotOptimize(result);
  }
}
}  // namespace
