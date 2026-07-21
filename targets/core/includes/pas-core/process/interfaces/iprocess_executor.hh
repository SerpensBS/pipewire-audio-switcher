#ifndef PAS_CORE_PROCESS_INTERFACES_IPROCESSEXECUTOR_HH
#define PAS_CORE_PROCESS_INTERFACES_IPROCESSEXECUTOR_HH

#include <chrono>
#include <expected>
#include <filesystem>
#include <string>
#include <vector>

#include "pas-core/utility/error.hh"

namespace pas::core::process {
// NOLINTNEXTLINE(hicpp-special-member-functions, cppcoreguidelines-special-member-functions)
class IProcessExecutor {
 public:
  /**
   * @brief Запускает процесс и возвращает его вывод.
   *
   * @param application Вызываемое приложение
   * @param args Аргументы передаваемые вызываемому приложению
   * @param timeout Таймаут операции
   *
   * @return возвращает вывод приложения, в случае ошибки также возвращает код
   * ошибки
   */
  [[nodiscard]]
  virtual auto Execute(const std::filesystem::path& command,
                       const std::vector<std::string>& args,
                       std::chrono::milliseconds timeout) const
      -> std::expected<std::string, utility::Error> = 0;

  virtual ~IProcessExecutor() = default;
};
}  // namespace pas::core::process

#endif
