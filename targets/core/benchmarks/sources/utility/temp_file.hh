#ifndef PAS_BENCHMARK_UTILITY_TEMP_FILE_HH
#define PAS_BENCHMARK_UTILITY_TEMP_FILE_HH

#include <filesystem>
#include <span>
#include <string_view>

namespace pas::benchmark::utility {

class TempFile {
 public:
  /**
   * @param file_name Имя файла
   * @param file_content Содержимое файла
   */
  explicit TempFile(std::string_view file_name, std::span<const char> file_content);

  TempFile(TempFile&&) = default;

  TempFile(const TempFile&) = delete;
  auto operator=(const TempFile&) -> TempFile& = delete;
  auto operator=(TempFile&&) -> TempFile& = delete;

  /**
   * @brief Путь к файлу.
   * @return Путь к файлу.
   */
  [[nodiscard]]
  auto GetFullPath() const -> const std::filesystem::path&;

  ~TempFile();

 private:
  // Путь к файлу.
  std::filesystem::path file_path_;
};

}  // namespace pas::benchmark::utility

#endif
