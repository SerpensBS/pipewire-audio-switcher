#ifndef PAS_CONFIG_CONFIGURATION_HH
#define PAS_CONFIG_CONFIGURATION_HH

#include <map>
#include <string>

#include "device_config.hh"

namespace pas::config {

struct Configuration {
  std::map<std::string, DeviceConfiguration> devices;
};

}  // namespace pas::config

#endif
