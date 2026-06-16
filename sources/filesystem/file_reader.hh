#ifndef PAS_FILESYSTEM_FILEREADER_HH
#define PAS_FILESYSTEM_FILEREADER_HH

#include <filesystem>
#include <string>

namespace pas::filesystem {

class FileReader {
 public:
  static auto Read(const std::filesystem::path& file_path) -> std::string;
};

}  // namespace pas::filesystem

#endif  // PAS_FILESYSTEM_FILEREADER_HH
