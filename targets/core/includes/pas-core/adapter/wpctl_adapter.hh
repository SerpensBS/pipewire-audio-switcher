#ifndef PAS_CORE_ADAPTER_WPCTLADAPTER_HH
#define PAS_CORE_ADAPTER_WPCTLADAPTER_HH

#include <chrono>
#include <cstddef>
#include <expected>
#include <memory>
#include <vector>

#include "pas-core/adapter/data/sink.hh"
#include "pas-core/process/process_executor.hh"
#include "pas-core/utility/error.hh"

namespace pas::core::adapter {

/**
 * @class WpctlApapter
 * @brief Адаптер для работы с утилитой wpctl,
 *
 */
class WpctlApapter {
 public:
  /**
   * @param process_executor Объект для работы с внешними процессами
   */
  explicit WpctlApapter(std::shared_ptr<pas::core::process::ProcessExecutor> process_executor);

  /**
   * @brief Получить все готовые к работе sink'и.
   *
   * @param timeout Таймаут операции
   */
  [[nodiscard]]
  auto GetSinks(std::chrono::milliseconds timeout) const
      -> std::expected<std::vector<Sink>, utility::Error>;

  /**
   * @brief Установить активный sink.
   *
   * @param sink_id Идентификатор sink'а
   * @param timeout Таймаут операции
   */
  [[nodiscard]]
  auto SetSink(std::size_t sink_id, std::chrono::milliseconds timeout) const
      -> std::expected<void, utility::Error>;

  /**
   * @brief Получить активный sink
   *
   * @param timeout Таймаут операции
   */
  [[nodiscard]]
  auto GetActiveSink(std::chrono::milliseconds timeout) const
      -> std::expected<Sink, utility::Error>;

 private:
  // Объект для работы с внешними процессами
  std::shared_ptr<pas::core::process::ProcessExecutor> process_executor_;
};
}  // namespace pas::core::adapter

#endif
