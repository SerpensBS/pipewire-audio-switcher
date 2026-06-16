#include "temp_file.hh"

#include <filesystem>
#include <format>
#include <fstream>
#include <ios>
#include <iterator>
#include <span>
#include <stdexcept>
#include <string>
#include <string_view>

namespace pas::tests::utility {

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
auto TempFile::Read() const -> std::string {
  std::ifstream file(file_path_);

  if (not file.is_open()) {
    throw std::ios_base::failure(
        std::format("Error during attempt open file {}", file_path_.c_str()));
  }

  file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

  return std::string{std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>()};
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
}  // namespace pas::tests::utility
