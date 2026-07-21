#ifndef PAS_CLI_COMMANDS_CYCLECOMMAND_HH
#define PAS_CLI_COMMANDS_CYCLECOMMAND_HH

#include <chrono>
#include <ostream>

#include "pas-core/adapter/interfaces/iwpctl_adapter.hh"

namespace pas::cli::commands {
/**
 * @class CycleSinkCommand
 * @brief Команда на переключение audio sink'а.
 *
 */
class CycleSinkCommand {
 public:
  /**
   * @brief Выполнить команду.
   *
   * @param error_stream Поток в который будут выводиться ошибки
   * @param timeout Таймаут операции
   */
  static void Execute(const pas::core::adapter::IWpctlApapter& adapter,
                      std::ostream& error_stream,
                      std::chrono::milliseconds timeout);
};
}  // namespace pas::cli::commands

#endif
