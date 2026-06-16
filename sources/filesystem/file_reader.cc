#include "file_reader.hh"

#include <algorithm>
#include <cstddef>
#include <filesystem>
#include <format>
#include <fstream>
#include <ios>
#include <stdexcept>
#include <string>

namespace pas::filesystem {

auto FileReader::Read(const std::filesystem::path& file_path, std::size_t max_readed_bytes_count)
    -> std::string {
  std::ifstream file(file_path, std::ios::binary);
  file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

  if (not file.is_open()) {
    throw std::runtime_error(std::format("Failed to open file {}", file_path.string()));
  }

  std::string result(std::min(max_readed_bytes_count, std::filesystem::file_size(file_path)), '\0');
  file.read(result.data(), static_cast<std::streamsize>(result.size()));

  return result;
}
}  // namespace pas::filesystem
