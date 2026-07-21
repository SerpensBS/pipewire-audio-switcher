#ifndef PAS_CORE_UTILITY_ERROR_HH
#define PAS_CORE_UTILITY_ERROR_HH

#include <string>
#include <system_error>

namespace pas::core::utility {
/**
 * @class Error
 * @brief Хранит сообщение об ошибке.
 *
 */
struct Error {
  // Код ошибки
  std::error_code code;
  // Сообщение
  std::string message;
};
}  // namespace pas::core::utility

#endif
