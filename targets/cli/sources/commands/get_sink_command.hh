#ifndef PAS_CLI_COMMANDS_GET_SINK_COMMAND_HH
#define PAS_CLI_COMMANDS_GET_SINK_COMMAND_HH

#include <chrono>
#include <ostream>

#include "pas-core/adapter/interfaces/iwpctl_adapter.hh"

namespace pas::cli::commands {
/**
 * @class GetSinkCommand
 * @brief Команда на получения информации об активном sink'е.
 *
 */
class GetSinkCommand {
 public:
  // NOLINTBEGIN(cppcoreguidelines-avoid-const-or-ref-data-members)
  struct Streams {
    // Поток в который будет выведен положительный результат работы программы
    std::ostream& result_stream;
    // Поток в который будут выводиться ошибки
    std::ostream& error_stream;
  };
  // NOLINTEND(cppcoreguidelines-avoid-const-or-ref-data-members)

  /**
   * @brief Выполнить команду.
   * @param adapter Адаптер для работы с утилитой wpctl
   * @param streams Потоки для вывода результата работы и ошибок
   * @param timeout Таймаут операции
   */
  static void Execute(const pas::core::adapter::IWpctlApapter& adapter,
                      Streams streams,
                      std::chrono::milliseconds timeout);
};
}  // namespace pas::cli::commands

#endif
