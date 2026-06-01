#ifndef PAS_UTILITY_PROCESS_EXECUTOR_HH
#define PAS_UTILITY_PROCESS_EXECUTOR_HH

#include <chrono>
#include <cstddef>
#include <expected>
#include <filesystem>
#include <string>
#include <system_error>
#include <vector>

namespace utility {

class ProcessExecutor {
 public:
  struct Error {
    // Код ошибки
    std::error_code code;
    // Сообщение
    std::string message;
  };

  /**
   * @brief Запускает процесс и возвращает его вывод.
   *
   * @param application Вызываемое приложение
   * @param args Аргументы передаваемые вызываемому приложению
   * @param max_return_buffer_size Максимальный размер выходного буфера
   *
   * @return возвращает вывод приложения, в случае ошибки также возвращает код ошибки
   */
  [[nodiscard]]
  auto Execute(const std::filesystem::path& command,
               const std::vector<std::string>& args,
               std::size_t max_return_buffer_size,
               std::chrono::milliseconds timeout) const -> std::expected<std::string, Error>;
};

}  // namespace utility
#endif
