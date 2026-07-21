#ifndef PAS_CORE_CONFIG_DEVICE_CONFIGURATION_HH
#define PAS_CORE_CONFIG_DEVICE_CONFIGURATION_HH

#include <string>

namespace pas::core::config {

/**
 * @class DeviceConfiguration
 * @brief Конфигурация устройства.
 *
 */
struct Device {
  // Имя
  std::string name;
  // Иконка
  std::string icon;
};

}  // namespace pas::core::config

#endif
