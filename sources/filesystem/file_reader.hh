#ifndef PAS_FILESYSTEM_FILEREADER_HH
#define PAS_FILESYSTEM_FILEREADER_HH

#include <cstddef>
#include <filesystem>
#include <string>

namespace filesystem {

class FileReader {
 public:
  static auto Read(const std::filesystem::path& file_path, std::size_t max_output_string_size = 0)
      -> std::string;
};

}  // namespace filesystem

#endif  // PAS_FILESYSTEM_FILEREADER_HH
