#include "temp_file.hh"

#include <filesystem>
#include <format>
#include <fstream>
#include <ios>
#include <span>
#include <stdexcept>
#include <string_view>

namespace pas::benchmark::utility {

static constexpr std::string_view kTempFsPath = "/dev/shm/";

TempFile::TempFile(std::string_view file_name, std::span<const char> file_content)
    : file_path_(std::filesystem::path(kTempFsPath) / file_name) {
  // Проверка на существование файла.
  if (std::filesystem::exists(file_path_)) {
    throw std::invalid_argument(std::format("File {} is laready exists", file_path_.c_str()));
  }

  // Создаем файл и пишем в него данные.
  std::ofstream file(file_path_, std::ios::trunc);

  file.exceptions(std::ios::failbit | std::ios::badbit);
  file.write(file_content.data(), static_cast<std::streamsize>(file_content.size()));
}

[[nodiscard]]
auto TempFile::GetFullPath() const -> const std::filesystem::path& {
  return file_path_;
}

TempFile::~TempFile() {
  if (std::filesystem::exists(file_path_)) {
    std::filesystem::remove(file_path_);
  }
}
}  // namespace pas::benchmark::utility
