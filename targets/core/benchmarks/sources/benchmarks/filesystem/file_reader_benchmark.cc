#include <benchmark/benchmark.h>

#include <cstddef>
#include <filesystem>
#include <string>
#include <string_view>
#include <vector>

#include "pas-core/filesystem/file_reader.hh"
#include "utility/temp_file.hh"

namespace {

class FileReaderBenchmark : public benchmark::Fixture {
 public:
  static constexpr std::size_t kContentSize{1000};
  static const std::vector<char> kContent;
};

// NOLINTNEXTLINE(cert-err58-cpp)
const std::vector<char> FileReaderBenchmark::kContent(kContentSize, 'a');

// NOLINTNEXTLINE(cert-err58-cpp)
BENCHMARK_F(FileReaderBenchmark, ReadFileFromRAM)(benchmark::State& state) {
  auto file = pas::benchmark::utility::TempFile("ReadSuccess", kContent);

  for (auto current_state : state) {
    auto result = pas::core::filesystem::FileReader::Read(file.GetFullPath());
    benchmark::DoNotOptimize(result);
  }
}

// NOLINTNEXTLINE(cert-err58-cpp)
BENCHMARK_F(FileReaderBenchmark, ReadEmptyFileFromRAM)(benchmark::State& state) {
  auto file = pas::benchmark::utility::TempFile("ReadSuccess", {});

  for (auto current_state : state) {
    auto result = pas::core::filesystem::FileReader::Read(file.GetFullPath());
    benchmark::DoNotOptimize(result);
  }
}

// NOLINTNEXTLINE(cert-err58-cpp)
BENCHMARK_F(FileReaderBenchmark, ReadNonExistFileFromRAM)(benchmark::State& state) {
  const std::filesystem::path kNonExistFileName = "NonExistFileName";

  for (auto current_state : state) {
    try {
      auto result = pas::core::filesystem::FileReader::Read(kNonExistFileName);
      benchmark::DoNotOptimize(result);
      // NOLINTNEXTLINE(bugprone-empty-catch)
    } catch (...) {
    }
  }
}
}  // namespace
