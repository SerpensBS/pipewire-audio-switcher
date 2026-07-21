#ifndef PAS_CORE_ADAPTER_INTERFACES_IWPCTLADAPTER_HH
#define PAS_CORE_ADAPTER_INTERFACES_IWPCTLADAPTER_HH

#include <chrono>
#include <cstddef>
#include <expected>
#include <vector>

#include "pas-core/adapter/data/sink.hh"
#include "pas-core/utility/error.hh"

namespace pas::core::adapter {
/**
 * @class IWpctlApapter
 * @brief Интерфейс для работы с утилитой wpctl,
 *
 */
// NOLINTNEXTLINE(hicpp-special-member-functions, cppcoreguidelines-special-member-functions)
class IWpctlApapter {
 public:
  /**
   * @brief Получить все готовые к работе sink'и.
   *
   * @param timeout Таймаут операции
   */
  [[nodiscard]]
  virtual auto GetSinks(std::chrono::milliseconds timeout) const
      -> std::expected<std::vector<Sink>, utility::Error> = 0;

  /**
   * @brief Установить активный sink.
   *
   * @param sink_id Идентификатор sink'а
   * @param timeout Таймаут операции
   */
  [[nodiscard]]
  virtual auto SetSink(std::size_t sink_id, std::chrono::milliseconds timeout) const
      -> std::expected<void, utility::Error> = 0;

  virtual ~IWpctlApapter() = default;
};
}  // namespace pas::core::adapter

#endif
