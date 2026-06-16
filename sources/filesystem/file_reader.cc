#include "file_reader.hh"

#include <filesystem>
#include <format>
#include <fstream>
#include <iterator>
#include <stdexcept>
#include <string>

namespace pas::filesystem {

auto FileReader::Read(const std::filesystem::path& file_path) -> std::string {
  if (not std::filesystem::exists(file_path)) {
    throw std::runtime_error(std::format("File not exitst: {}", file_path.c_str()));
  }

  std::ifstream file(file_path);
  if (not file.is_open()) {
    throw std::runtime_error(std::format("Failed to open file {}", file_path.c_str()));
  }

  file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

  return std::string{std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>()};
}
}  // namespace pas::filesystem
