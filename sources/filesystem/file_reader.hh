#ifndef PAS_FILESYSTEM_FILEREADER_HH
#define PAS_FILESYSTEM_FILEREADER_HH

#include <filesystem>
#include <string>

namespace pas::filesystem {

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
  static auto Read(const std::filesystem::path& file_path) -> std::string;
};

}  // namespace pas::filesystem

#endif  // PAS_FILESYSTEM_FILEREADER_HH
