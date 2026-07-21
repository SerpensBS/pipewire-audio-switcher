#ifndef PAS_CORE_ADAPTER_DATA_SINK_HH
#define PAS_CORE_ADAPTER_DATA_SINK_HH

#include <string>

namespace pas::core::adapter {
/**
 * @class Sink
 * @brief Аудиоустройство.
 *
 */
struct Sink {
  // Идентификатор
  int id;
  // Имя
  std::string name;
  // Громкость (от 0 до 1)
  double volume;
  // Статус
  bool is_active;
};
}  // namespace pas::core::adapter
#endif
