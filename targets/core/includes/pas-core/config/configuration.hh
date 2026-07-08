#ifndef PAS_CORE_CONFIG_CONFIGURATION_HH
#define PAS_CORE_CONFIG_CONFIGURATION_HH

#include <map>
#include <string>

#include "device_config.hh"

namespace pas::core::config {

/**
 * @class Configuration
 * @brief Конфигурация.
 *
 */
struct Configuration {
  // Список конфигураций устройств
  std::map<std::string, DeviceConfiguration> devices;
};

}  // namespace pas::core::config

#endif
