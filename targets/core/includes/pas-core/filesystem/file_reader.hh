#ifndef PAS_CORE_FILESYSTEM_FILEREADER_HH
#define PAS_CORE_FILESYSTEM_FILEREADER_HH

#include <cstddef>
#include <filesystem>
#include <limits>
#include <string>

namespace pas::core::filesystem {

class FileReader {
 public:
  /**
   * @brief Читает файл.
   *
   * @param file_path Путь к файлу
   *
   * @return Содержимое файла
   *
   * @throws std::runtime_error Если файл отсутствует или не открывается
   * @throws std::ios_base::failure в случае ошибок FAILBIT и BADBIT при чтение файла
   */
  static auto Read(const std::filesystem::path& file_path,
                   std::size_t max_readed_bytes_count = std::numeric_limits<std::size_t>::max())
      -> std::string;
};

}  // namespace pas::core::filesystem

#endif  // PAS_FILESYSTEM_FILEREADER_HH
