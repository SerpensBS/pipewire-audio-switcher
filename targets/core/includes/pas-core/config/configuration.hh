#ifndef PAS_CORE_CONFIG_CONFIGURATION_HH
#define PAS_CORE_CONFIG_CONFIGURATION_HH

#include <map>
#include <string>

#include "device.hh"

namespace pas::core::config {

/**
 * @class Configuration
 * @brief Конфигурация.
 *
 */
struct Configuration {
  // Список конфигураций устройств
  std::map<std::string, Device> devices;
};

}  // namespace pas::core::config

#endif
